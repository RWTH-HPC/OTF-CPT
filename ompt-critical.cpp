/*
 * critPathAnalysis.cpp -- Critical path analysis runtime library, build for
 * hybrid OpenMp and MPI applications
 */

//===----------------------------------------------------------------------===//
//
// Based on the ompt-tsan.cpp of the LLVM Project
// version as of 06/24/2021
// parent 82e4e50 commit 08d8f1a958bd8be681e3e1f346be80818a83a556
// See https://llvm.org/LICENSE.txt for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "criticalPath.h"
#include "omp-tools.h"
#include "omp.h"

// Define attribute that indicates that the fall through from the previous
// case label is intentional and should not be diagnosed by a compiler
//   Code from libcxx/include/__config
// Use a function like macro to imply that it must be followed by a semicolon
#if __cplusplus > 201402L && __has_cpp_attribute(fallthrough)
#define KMP_FALLTHROUGH() [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
#define KMP_FALLTHROUGH() [[clang::fallthrough]]
#elif __has_attribute(fallthrough) || __GNUC__ >= 7
#define KMP_FALLTHROUGH() __attribute__((__fallthrough__))
#else
#define KMP_FALLTHROUGH() ((void)0)
#endif

/// Required OMPT inquiry functions.
static ompt_get_parallel_info_t critical_ompt_get_parallel_info{};
static ompt_get_thread_data_t critical_ompt_get_thread_data{};

static int hasTaskCreation = 0;

template <bool always = true> struct ompTimer {
  const char *loc;
  bool stopped{false};
  ompTimer(const char *loc = NULL) : loc(loc) {
    if (thread_local_clock == nullptr)
      thread_local_clock = new THREAD_CLOCK(my_next_id(), 0);
    else {
      if (always || !thread_local_clock->stopped_clock)
        thread_local_clock->Stop(CLOCK_OMP, loc);
      else
        stopped = true;
    }
  }
  ~ompTimer() {
    if (always || !stopped)
      thread_local_clock->Start(CLOCK_OMP, loc);
  }
};

static int pagesize{0};

// Data structure to provide a threadsafe pool of reusable objects.
// DataPool<Type of objects>
template <typename T> struct DataPool final {
  static __thread DataPool<T> *ThreadDataPool;
  std::mutex DPMutex{};

  // store unused objects
  std::vector<T *> DataPointer{};
  std::vector<T *> RemoteDataPointer{};

  // store all allocated memory to finally release
  std::list<void *> memory;

  // count remotely returned data (RemoteDataPointer.size())
  std::atomic<int> remote{0};

  // totally allocated data objects in pool
  int total{0};
#ifdef DEBUG_DATA
  int remoteReturn{0};
  int localReturn{0};

  int getRemote() { return remoteReturn + remote; }
  int getLocal() { return localReturn; }
#endif
  int getTotal() { return total; }
  int getMissing() {
    return total - DataPointer.size() - RemoteDataPointer.size();
  }

  // fill the pool by allocating a page of memory
  void newDatas() {
    if (remote > 0) {
      const std::lock_guard<std::mutex> lock(DPMutex);
      // DataPointer is empty, so just swap the vectors
      DataPointer.swap(RemoteDataPointer);
      remote = 0;
      return;
    }
    // calculate size of an object including padding to cacheline size
    size_t elemSize = sizeof(T);
    size_t paddedSize = (((elemSize - 1) / 64) + 1) * 64;
    // number of padded elements to allocate
    int ndatas = pagesize / paddedSize;
    char *datas = (char *)malloc(ndatas * paddedSize);
    memory.push_back(datas);
    for (int i = 0; i < ndatas; i++) {
      DataPointer.push_back(new (datas + i * paddedSize) T(this));
    }
    total += ndatas;
  }

  // get data from the pool
  T *getData() {
    T *ret;
    if (DataPointer.empty())
      newDatas();
    ret = DataPointer.back();
    DataPointer.pop_back();
    return ret;
  }

  // accesses to the thread-local datapool don't need locks
  void returnOwnData(T *data) {
    DataPointer.emplace_back(data);
#ifdef DEBUG_DATA
    localReturn++;
#endif
  }

  // returning to a remote datapool using lock
  void returnData(T *data) {
    const std::lock_guard<std::mutex> lock(DPMutex);
    RemoteDataPointer.emplace_back(data);
    remote++;
#ifdef DEBUG_DATA
    remoteReturn++;
#endif
  }

  ~DataPool() {
    // we assume all memory is returned when the thread finished / destructor is
    // called
    if (analysis_flags->report_data_leak && getMissing() != 0) {
      printf("ERROR: While freeing DataPool (%s) we are missing %i data "
             "objects.\n",
             __PRETTY_FUNCTION__, getMissing());
      exit(-3);
    }
    for (auto i : DataPointer)
      if (i)
        i->~T();
    for (auto i : RemoteDataPointer)
      if (i)
        i->~T();
    for (auto i : memory)
      if (i)
        free(i);
  }
};

template <typename T> struct DataPoolEntry {
  DataPool<T> *owner;

  static T *New() { return DataPool<T>::ThreadDataPool->getData(); }

  void Delete() {
    static_cast<T *>(this)->Reset();
    if (owner == DataPool<T>::ThreadDataPool)
      owner->returnOwnData(static_cast<T *>(this));
    else
      owner->returnData(static_cast<T *>(this));
  }

  DataPoolEntry(DataPool<T> *dp) : owner(dp) {}
};

struct DependencyData;
typedef DataPool<DependencyData> DependencyDataPool;
template <>
__thread DependencyDataPool *DependencyDataPool::ThreadDataPool = nullptr;

/// Data structure to store additional information for task dependency.
struct DependencyData final : DataPoolEntry<DependencyData> {
  ompt_tsan_clockid in;
  ompt_tsan_clockid out;
  ompt_tsan_clockid inoutset;
  ompt_tsan_clockid *GetInPtr() { return &in; }
  ompt_tsan_clockid *GetOutPtr() { return &out; }
  ompt_tsan_clockid *GetInoutsetPtr() { return &inoutset; }

  void Reset() {}

  static DependencyData *New() { return DataPoolEntry<DependencyData>::New(); }

  DependencyData(DataPool<DependencyData> *dp)
      : DataPoolEntry<DependencyData>(dp) {}
};

struct TaskDependency {
  ompt_tsan_clockid *inPtr;
  ompt_tsan_clockid *outPtr;
  ompt_tsan_clockid *inoutsetPtr;
  ompt_dependence_type_t type;
  TaskDependency(DependencyData *depData, ompt_dependence_type_t type)
      : inPtr(depData->GetInPtr()), outPtr(depData->GetOutPtr()),
        inoutsetPtr(depData->GetInoutsetPtr()), type(type) {}
  void AnnotateBegin() {
    if (type == ompt_dependence_type_out ||
        type == ompt_dependence_type_inout ||
        type == ompt_dependence_type_mutexinoutset) {
      OmpHappensAfter(inPtr);
      OmpHappensAfter(outPtr);
      OmpHappensAfter(inoutsetPtr);
    } else if (type == ompt_dependence_type_in) {
      OmpHappensAfter(outPtr);
      OmpHappensAfter(inoutsetPtr);
    } else if (type == ompt_dependence_type_inoutset) {
      OmpHappensAfter(inPtr);
      OmpHappensAfter(outPtr);
    }
  }
  void AnnotateEnd() {
    if (type == ompt_dependence_type_out ||
        type == ompt_dependence_type_inout ||
        type == ompt_dependence_type_mutexinoutset) {
      OmpHappensBefore(outPtr);
    } else if (type == ompt_dependence_type_in) {
      OmpHappensBefore(inPtr);
    } else if (type == ompt_dependence_type_inoutset) {
      OmpHappensBefore(inoutsetPtr);
    }
  }
};

struct ParallelData;
typedef DataPool<ParallelData> ParallelDataPool;
template <>
__thread ParallelDataPool *ParallelDataPool::ThreadDataPool = nullptr;

/// Data structure to store additional information for parallel regions.
struct ParallelData final : DataPoolEntry<ParallelData> {

  // Parallel fork is just another barrier, use Barrier[1]

  /// Two addresses for relationships with barriers.
  ompt_tsan_clockid Barrier[3];

  const void *codePtr;

  ompt_tsan_clockid *GetParallelPtr() { return &(Barrier[1]); }

  ompt_tsan_clockid *GetBarrierPtr(unsigned Index) { return &(Barrier[Index]); }

  ParallelData *Init(const void *codeptr) {
    codePtr = codeptr;
    return this;
  }

  void Reset() {}

  static ParallelData *New(const void *codeptr) {
    return DataPoolEntry<ParallelData>::New()->Init(codeptr);
  }

  ParallelData(DataPool<ParallelData> *dp) : DataPoolEntry<ParallelData>(dp) {}
};

static inline ParallelData *ToParallelData(ompt_data_t *parallel_data) {
  return reinterpret_cast<ParallelData *>(parallel_data->ptr);
}

struct Taskgroup;
typedef DataPool<Taskgroup> TaskgroupPool;
template <> __thread TaskgroupPool *TaskgroupPool::ThreadDataPool = nullptr;

/// Data structure to support stacking of taskgroups and allow synchronization.
struct Taskgroup final : DataPoolEntry<Taskgroup> {
  /// Its address is used for relationships of the taskgroup's task set.
  ompt_tsan_clockid Ptr;

  /// Reference to the parent taskgroup.
  Taskgroup *Parent;

  ompt_tsan_clockid *GetPtr() { return &Ptr; }

  Taskgroup *Init(Taskgroup *parent) {
    Parent = parent;
    return this;
  }

  void Reset() {}

  static Taskgroup *New(Taskgroup *Parent) {
    return DataPoolEntry<Taskgroup>::New()->Init(Parent);
  }

  Taskgroup(DataPool<Taskgroup> *dp) : DataPoolEntry<Taskgroup>(dp) {}
};

struct TaskData;
typedef DataPool<TaskData> TaskDataPool;
template <> __thread TaskDataPool *TaskDataPool::ThreadDataPool = nullptr;

/// Data structure to store additional information for tasks.
struct TaskData final : DataPoolEntry<TaskData> {
  /// Its address is used for relationships of this task.
  ompt_tsan_clockid Task{0};

  /// Child tasks use its address to declare a relationship to a taskwait in
  /// this task.
  ompt_tsan_clockid Taskwait{0};

  /// Whether this task is currently executing a barrier.
  bool InBarrier{false};

  /// Whether this task is an included task.
  int TaskType{0};

  int ThreadNum{-1};

  /// count execution phase
  int execution{0};

  /// Index of which barrier to use next.
  char BarrierIndex{0};

  /// Count how often this structure has been put into child tasks + 1.
  std::atomic_int RefCount{1};

  /// Reference to the parent that created this task.
  TaskData *Parent{nullptr};

  /// Reference to the implicit task in the stack above this task.
  TaskData *ImplicitTask{nullptr};

  /// Reference to the team of this task.
  ParallelData *Team{nullptr};

  /// Reference to the current taskgroup that this task either belongs to or
  /// that it just created.
  Taskgroup *TaskGroup{nullptr};

  /// Dependency information for this task.
  TaskDependency *Dependencies{nullptr};

  /// Number of dependency entries.
  unsigned DependencyCount{0};

  // The dependency-map stores DependencyData objects representing
  // the dependency variables used on the sibling tasks created from
  // this task
  // We expect a rare need for the dependency-map, so alloc on demand
  std::unordered_map<void *, DependencyData *> *DependencyMap{nullptr};

#ifdef DEBUG
  int freed{0};
#endif

  bool isIncluded() { return TaskType & ompt_task_undeferred; }
  bool isUntied() { return TaskType & ompt_task_untied; }
  bool isFinal() { return TaskType & ompt_task_final; }
  bool isMergable() { return TaskType & ompt_task_mergeable; }
  bool isMerged() { return TaskType & ompt_task_merged; }

  bool isExplicit() { return TaskType & ompt_task_explicit; }
  bool isImplicit() { return TaskType & ompt_task_implicit; }
  bool isInitial() { return TaskType & ompt_task_initial; }
  bool isTarget() { return TaskType & ompt_task_target; }

  ompt_tsan_clockid *GetTaskPtr() { return &Task; }

  ompt_tsan_clockid *GetTaskwaitPtr() { return &Taskwait; }

  TaskData *Init(TaskData *parent, int taskType) {
    TaskType = taskType;
    Parent = parent;
    Team = Parent->Team;
    if (Parent != nullptr) {
      Parent->RefCount++;
      // Copy over pointer to taskgroup. This task may set up its own stack
      // but for now belongs to its parent's taskgroup.
      TaskGroup = Parent->TaskGroup;
    }
    return this;
  }

  TaskData *Init(ParallelData *team, int taskType) {
    TaskType = taskType;
    execution = 1;
    ImplicitTask = this;
    Team = team;
    return this;
  }

  TaskData *Init(ParallelData *team, int threadNum, int taskType) {
    TaskType = taskType;
    execution = 1;
    ImplicitTask = this;
    Team = team;
    ThreadNum = threadNum;
    return this;
  }

  void Reset() {
    InBarrier = false;
    TaskType = 0;
    execution = 0;
    BarrierIndex = 0;
    RefCount = 1;
    Parent = nullptr;
    ImplicitTask = nullptr;
    Team = nullptr;
    ThreadNum = -1;
    TaskGroup = nullptr;
    if (DependencyMap) {
      for (auto i : *DependencyMap)
        i.second->Delete();
      delete DependencyMap;
    }
    DependencyMap = nullptr;
    if (Dependencies)
      free(Dependencies);
    Dependencies = nullptr;
    DependencyCount = 0;
#ifdef DEBUG
    freed = 0;
#endif
  }

  static TaskData *New(TaskData *parent, int taskType) {
    return DataPoolEntry<TaskData>::New()->Init(parent, taskType);
  }

  static TaskData *New(ParallelData *team, int taskType) {
    return DataPoolEntry<TaskData>::New()->Init(team, taskType);
  }

  static TaskData *New(ParallelData *team, int threadNum, int taskType) {
    return DataPoolEntry<TaskData>::New()->Init(team, threadNum, taskType);
  }

  TaskData(DataPool<TaskData> *dp) : DataPoolEntry<TaskData>(dp) {}
};

static inline TaskData *ToTaskData(ompt_data_t *task_data) {
  return reinterpret_cast<TaskData *>(task_data->ptr);
}

/*
 * Threads
 */

/// Store a mutex for each wait_id to resolve race condition with callbacks.
std::unordered_map<ompt_wait_id_t, std::pair<std::mutex, ompt_tsan_clockid>>
    Locks;
std::mutex LocksMutex;
std::mutex tcmutex;

static void ompt_tsan_thread_begin(ompt_thread_t thread_type,
                                   ompt_data_t *thread_data) {
  ParallelDataPool::ThreadDataPool = new ParallelDataPool;
  TaskgroupPool::ThreadDataPool = new TaskgroupPool;
  TaskDataPool::ThreadDataPool = new TaskDataPool;
  DependencyDataPool::ThreadDataPool = new DependencyDataPool;

  if (!thread_local_clock)
    thread_local_clock = new THREAD_CLOCK(my_next_id(), 0, true);
  thread_data->ptr = thread_local_clock;
  {
    const std::lock_guard<std::mutex> lock(tcmutex);
    thread_clocks.push_back(thread_local_clock);
  }
}

static void ompt_tsan_thread_end(ompt_data_t *thread_data) {

  delete ParallelDataPool::ThreadDataPool;
  delete TaskgroupPool::ThreadDataPool;
  delete TaskDataPool::ThreadDataPool;
  delete DependencyDataPool::ThreadDataPool;
}

/// OMPT event callbacks for handling parallel regions.

static void ompt_tsan_parallel_begin(ompt_data_t *parent_task_data,
                                     const ompt_frame_t *parent_task_frame,
                                     ompt_data_t *parallel_data,
                                     uint32_t requested_team_size, int flag,
                                     const void *codeptr_ra) {
  ParallelData *Data = ParallelData::New(codeptr_ra);
  parallel_data->ptr = Data;

  // end of computation
  thread_local_clock->Stop(CLOCK_OMP, "ParallelBegin");
  OmpHappensBefore(Data->GetParallelPtr());
}

static void ompt_tsan_parallel_end(ompt_data_t *parallel_data,
                                   ompt_data_t *task_data, int flag,
                                   const void *codeptr_ra) {
  ParallelData *Data = ToParallelData(parallel_data);
  thread_local_clock->Start(CLOCK_OMP, "ParallelEnd");

  Data->Delete();

#if (LLVM_VERSION >= 40)
  if (&__archer_get_omp_status) {
    if (__archer_get_omp_status() == 0 && analysis_flags->flush_shadow)
      __tsan_flush_memory();
  }
#endif
}

static void ompt_tsan_implicit_task(ompt_scope_endpoint_t endpoint,
                                    ompt_data_t *parallel_data,
                                    ompt_data_t *task_data,
                                    unsigned int team_size,
                                    unsigned int thread_num, int type) {
  switch (endpoint) {
  case ompt_scope_begin:
    if (type & ompt_task_initial) {
      parallel_data->ptr = ParallelData::New(nullptr);
    } else {
      if (thread_local_clock->stopped_mpi_clock) {
        thread_local_clock->Start(CLOCK_MPI_ONLY, __func__);
      }
      OmpHappensAfter(ToParallelData(parallel_data)->GetParallelPtr());
    }
    task_data->ptr =
        TaskData::New(ToParallelData(parallel_data), thread_num, type);

    if (!(type & ompt_task_initial) || thread_local_clock->stopped_clock) {
      if (thread_local_clock->stopped_mpi_clock) {
        thread_local_clock->Start(CLOCK_ALL, __func__);
      } else {
        thread_local_clock->Start(CLOCK_OMP, __func__);
      }
    }
    // start of the task, useful computation start
    break;
  case ompt_scope_end: {
    // end of the task, useful computation stop
    TaskData *Data = ToTaskData(task_data);
#ifdef DEBUG
    assert(Data->freed == 0 && "Implicit task end should only be called once!");
    Data->freed = 1;
#endif
    assert(Data->RefCount == 1 &&
           "All tasks should have finished at the implicit barrier!");
    Data->Delete();
    if (type & ompt_task_initial) {
      ToParallelData(parallel_data)->Delete();
    }
    if (team_size == 1 && !(type & ompt_task_initial))
      thread_local_clock->Stop(CLOCK_OMP, __func__);

    assert(thread_local_clock->stopped_clock == true);

    break;
  }
  case ompt_scope_beginend:
    // Should not occur according to OpenMP 5.1
    // Tested in OMPT tests
    break;
  }
}

static void ompt_tsan_sync_region(ompt_sync_region_t kind,
                                  ompt_scope_endpoint_t endpoint,
                                  ompt_data_t *parallel_data,
                                  ompt_data_t *task_data,
                                  const void *codeptr_ra) {
  TaskData *Data = ToTaskData(task_data);
  switch (endpoint) {
  case ompt_scope_begin:
  case ompt_scope_beginend:
    // runtime overhead, stop useful
    Data->InBarrier = true;
    thread_local_clock->Stop(CLOCK_OMP, "SyncRegionBegin");
    switch (kind) {
    case ompt_sync_region_barrier_implementation:
    case ompt_sync_region_barrier_implicit:
    case ompt_sync_region_barrier_explicit:
    case ompt_sync_region_barrier_implicit_parallel:
    case ompt_sync_region_barrier_implicit_workshare:
    case ompt_sync_region_barrier_teams:
    case ompt_sync_region_barrier: {
      char BarrierIndex = Data->BarrierIndex;
      if (Data->ThreadNum == 0)
        OmpClockReset(Data->Team->GetBarrierPtr((BarrierIndex + 1) % 3));
      OmpHappensBefore(Data->Team->GetBarrierPtr(BarrierIndex));
      break;
    }

    case ompt_sync_region_taskwait:
      break;

    case ompt_sync_region_taskgroup:
      Data->TaskGroup = Taskgroup::New(Data->TaskGroup);
      break;

    case ompt_sync_region_reduction:
      // should never be reached
      break;
    }
    if (endpoint == ompt_scope_begin)
      break;
    KMP_FALLTHROUGH();
  case ompt_scope_end:
    switch (kind) {
    case ompt_sync_region_barrier_implementation:
    case ompt_sync_region_barrier_implicit:
    case ompt_sync_region_barrier_explicit:
    case ompt_sync_region_barrier_implicit_parallel:
    case ompt_sync_region_barrier_implicit_workshare:
    case ompt_sync_region_barrier_teams:
    case ompt_sync_region_barrier: {
      char BarrierIndex = Data->BarrierIndex;
      // Barrier will end after it has been entered by all threads.
      if (parallel_data || Data->ThreadNum == 0)
        OmpHappensAfter(Data->Team->GetBarrierPtr(BarrierIndex));

      // It is not guaranteed that all threads have exited this barrier before
      // we enter the next one. So we will use a different address.
      // We are however guaranteed that this current barrier is finished
      // by the time we exit the next one. So we can then reuse the first
      // address.
      Data->BarrierIndex = (BarrierIndex + 1) % 3;
      break;
    }

    case ompt_sync_region_taskwait: {
      if (Data->execution > 1)
        OmpHappensAfter(Data->GetTaskwaitPtr());
      break;
    }

    case ompt_sync_region_taskgroup: {
      assert(Data->TaskGroup != nullptr &&
             "Should have at least one taskgroup!");

      OmpHappensAfter(Data->TaskGroup->GetPtr());

      // Delete this allocated taskgroup, all descendent task are finished by
      // now.
      Taskgroup *Parent = Data->TaskGroup->Parent;
      Data->TaskGroup->Delete();
      Data->TaskGroup = Parent;
      break;
    }

    case ompt_sync_region_reduction:
      // Should not occur according to OpenMP 5.1
      // Tested in OMPT tests
      break;
    }
    Data->InBarrier = false;
    if (parallel_data && kind != ompt_sync_region_barrier_implicit_parallel)
      thread_local_clock->Start(CLOCK_OMP, "SyncRegionEnd");
    break;
  }
}

static int ompt_tsan_control_tool(uint64_t command, uint64_t modifier,
                                  void *arg, const void *codeptr_ra) {
  if (command == omp_control_tool_start) {
    startTool();
  } else if (command == omp_control_tool_pause) {
    return 1;
  } else if (command == omp_control_tool_flush) {
    return 1;
  } else if (command == omp_control_tool_end) {
    stopTool();
  }
  return 0;
}

/// OMPT event callbacks for handling tasks.

static void ompt_tsan_task_create(
    ompt_data_t *parent_task_data,    /* id of parent task            */
    const ompt_frame_t *parent_frame, /* frame data for parent task   */
    ompt_data_t *new_task_data,       /* id of created task           */
    int type, int has_dependences,
    const void *codeptr_ra) /* pointer to outlined function */
{
  TaskData *Data;
  assert(new_task_data->ptr == NULL &&
         "Task data should be initialized to NULL");
  if (type & ompt_task_initial) {
    ompt_data_t *parallel_data;
    int team_size = 1;
    critical_ompt_get_parallel_info(0, &parallel_data, &team_size);
    ParallelData *PData = ParallelData::New(nullptr);
    parallel_data->ptr = PData;

    Data = TaskData::New(PData, type);
    new_task_data->ptr = Data;
  } else if (type & ompt_task_undeferred) {
    Data = TaskData::New(ToTaskData(parent_task_data), type);
    new_task_data->ptr = Data;
  } else if (type & ompt_task_explicit || type & ompt_task_target) {
    Data = TaskData::New(ToTaskData(parent_task_data), type);
    new_task_data->ptr = Data;

    // Use the newly created address. We cannot use a single address from the
    // parent because that would declare wrong relationships with other
    // sibling tasks that may be created before this task is started!
    ompTimer<false> ot{"TaskCreate"};
    OmpHappensBefore(Data->GetTaskPtr());
    ToTaskData(parent_task_data)->execution++;
  }
}

static void
ompt_tsan_task_creation(ompt_scope_endpoint_t endpoint,
                        ompt_data_t *parent_task_data, /* id of parent task */
                        ompt_data_t *new_task_data) /* id of created task    */
{
  switch (endpoint) {
  case ompt_scope_begin:
    // runtime overhead, stop useful
    thread_local_clock->Stop(CLOCK_OMP, "TaskCreationBegin");
    break;
  case ompt_scope_end:
    // runtime overhead, stop useful
    thread_local_clock->Start(CLOCK_OMP, "TaskCreationEnd");
    break;
  case ompt_scope_beginend:
    // Should not occur according to OpenMP 5.1
    // Tested in OMPT tests
    break;
  }
}

static void freeTask(TaskData *task) {
  while (task != nullptr && --task->RefCount == 0) {
    TaskData *Parent = task->Parent;
    task->Delete();
    task = Parent;
  }
}

static void releaseDependencies(TaskData *task) {
  for (unsigned i = 0; i < task->DependencyCount; i++) {
    task->Dependencies[i].AnnotateEnd();
  }
}

static void acquireDependencies(TaskData *task) {
  for (unsigned i = 0; i < task->DependencyCount; i++) {
    task->Dependencies[i].AnnotateBegin();
  }
}

static void ompt_tsan_task_schedule(ompt_data_t *first_task_data,
                                    ompt_task_status_t prior_task_status,
                                    ompt_data_t *second_task_data) {

  //
  //  The necessary action depends on prior_task_status:
  //
  //    ompt_task_early_fulfill = 5,
  //     -> ignored
  //
  //    ompt_task_late_fulfill  = 6,
  //     -> first completed, first freed, second ignored
  //
  //    ompt_task_complete      = 1,
  //    ompt_task_cancel        = 3,
  //     -> first completed, first freed, second starts
  //
  //    ompt_task_detach        = 4,
  //    ompt_task_yield         = 2,
  //    ompt_task_switch        = 7
  //     -> first suspended, second starts
  //

  if (prior_task_status == ompt_task_early_fulfill)
    return;

  TaskData *FromTask = ToTaskData(first_task_data);

  if (thread_local_clock == nullptr)
    thread_local_clock = new THREAD_CLOCK(my_next_id(), 0);

  // Legacy handling for missing reduction callback
  if (!FromTask->InBarrier) {
    thread_local_clock->Stop(CLOCK_OMP, "TaskEnd");
  }

  // The late fulfill happens after the detached task finished execution
  if (prior_task_status == ompt_task_late_fulfill)
    OmpHappensAfter(FromTask->GetTaskPtr());

  // task completed execution
  if (prior_task_status == ompt_task_complete ||
      prior_task_status == ompt_task_cancel ||
      prior_task_status == ompt_task_late_fulfill) {
    // Included tasks are executed sequentially, no need to track
    // synchronization
    if (!FromTask->isIncluded()) {
      // Task will finish before a barrier in the surrounding parallel region
      // ...
      ParallelData *PData = FromTask->Team;
      OmpHappensBefore(
          PData->GetBarrierPtr(FromTask->ImplicitTask->BarrierIndex));

      // ... and before an eventual taskwait by the parent thread.
      OmpHappensBefore(FromTask->Parent->GetTaskwaitPtr());

      if (FromTask->TaskGroup != nullptr) {
        // This task is part of a taskgroup, so it will finish before the
        // corresponding taskgroup_end.
        OmpHappensBefore(FromTask->TaskGroup->GetPtr());
      }
    }

    // release dependencies
    releaseDependencies(FromTask);
    // free the previously running task
    freeTask(FromTask);
  }

  // For late fulfill of detached task, there is no task to schedule to
  if (prior_task_status == ompt_task_late_fulfill) {
    OmpClockReset(thread_local_clock);
    return;
  }

  TaskData *ToTask = ToTaskData(second_task_data);

  // task suspended
  if (prior_task_status == ompt_task_switch ||
      prior_task_status == ompt_task_yield ||
      prior_task_status == ompt_task_detach) {
    // Task may be resumed at a later point in time.
    OmpHappensBefore(FromTask->GetTaskPtr());
    ToTask->ImplicitTask = FromTask->ImplicitTask;
    assert(ToTask->ImplicitTask != NULL &&
           "A task belongs to a team and has an implicit task on the stack");
  }

  // Handle dependencies on first execution of the task
  if (ToTask->execution == 0) {
    ToTask->execution++;
    acquireDependencies(ToTask);
  }
  // 1. Task will begin execution after it has been created.
  // 2. Task will resume after it has been switched away.
  OmpHappensAfter(ToTask->GetTaskPtr());
  // only start the clock if the next task is not in a barrier
  if (!ToTask->InBarrier) {
    thread_local_clock->Start(CLOCK_OMP, "TaskBegin");
  }
}

static void ompt_tsan_dependences(ompt_data_t *task_data,
                                  const ompt_dependence_t *deps, int ndeps) {
  if (ndeps > 0) {
    ompTimer<false> ot{"TaskDepend"};
    // Copy the data to use it in task_switch and task_end.
    TaskData *Data = ToTaskData(task_data);
    if (!Data->Parent->DependencyMap)
      Data->Parent->DependencyMap =
          new std::unordered_map<void *, DependencyData *>();
    Data->Dependencies =
        (TaskDependency *)malloc(sizeof(TaskDependency) * ndeps);
    Data->DependencyCount = ndeps;
    for (int i = 0; i < ndeps; i++) {
      auto ret = Data->Parent->DependencyMap->insert(
          std::make_pair(deps[i].variable.ptr, nullptr));
      if (ret.second) {
        ret.first->second = DependencyData::New();
      }
      new ((void *)(Data->Dependencies + i))
          TaskDependency(ret.first->second, deps[i].dependence_type);
    }

    // This callback is executed before this task is first started.
    OmpHappensBefore(Data->GetTaskPtr());
  }
}

static void ompt_tsan_mutex_acquire(ompt_mutex_t kind, unsigned int hint,
                                    unsigned int impl, ompt_wait_id_t wait_id,
                                    const void *codeptr_ra) {
  thread_local_clock->Stop(CLOCK_OMP, "MutexAcquire");
}

/// OMPT event callbacks for handling locking.
static void ompt_tsan_mutex_acquired(ompt_mutex_t kind, ompt_wait_id_t wait_id,
                                     const void *codeptr_ra) {

  //    ompTimer ot{};
  // Acquire our own lock to make sure that
  // 1. the previous release has finished.
  // 2. the next acquire doesn't start before we have finished our release.
  LocksMutex.lock();
  auto &Lock = Locks[wait_id];
  LocksMutex.unlock();

  Lock.first.lock();
  OmpHappensAfter(&Lock.second);
  thread_local_clock->Start(CLOCK_OMP, "MutexAcquired");
}

static void ompt_tsan_mutex_released(ompt_mutex_t kind, ompt_wait_id_t wait_id,
                                     const void *codeptr_ra) {
  ompTimer<> ot{"MutexRelease"};
  LocksMutex.lock();
  auto &Lock = Locks[wait_id];
  LocksMutex.unlock();
  OmpHappensBefore(&Lock.second);

  Lock.first.unlock();
}

// callback , signature , variable to store result , required support level
#define SET_OPTIONAL_CALLBACK_T(event, type, result, level)                    \
  do {                                                                         \
    ompt_callback_##type##_t tsan_##event = &ompt_tsan_##event;                \
    result = ompt_set_callback(ompt_callback_##event,                          \
                               (ompt_callback_t)tsan_##event);                 \
    if (result < level)                                                        \
      printf("Registered callback '" #event "' is not supported at " #level    \
             " (%i)\n",                                                        \
             result);                                                          \
  } while (0)

#define SET_CALLBACK_T(event, type)                                            \
  do {                                                                         \
    int res;                                                                   \
    SET_OPTIONAL_CALLBACK_T(event, type, res, ompt_set_always);                \
  } while (0)

#define SET_CALLBACK(event) SET_CALLBACK_T(event, event)

static int ompt_tsan_initialize(ompt_function_lookup_t lookup, int device_num,
                                ompt_data_t *tool_data) {

  ompt_set_callback_t ompt_set_callback =
      (ompt_set_callback_t)lookup("ompt_set_callback");
  if (ompt_set_callback == NULL) {
    std::cerr << "Could not set callback, exiting..." << std::endl;
    std::exit(1);
  }
  critical_ompt_get_parallel_info =
      (ompt_get_parallel_info_t)lookup("ompt_get_parallel_info");
  critical_ompt_get_thread_data =
      (ompt_get_thread_data_t)lookup("ompt_get_thread_data");
  critical_ompt_finalize_tool =
      (ompt_finalize_tool_t)lookup("ompt_finalize_tool");

  if (critical_ompt_get_parallel_info == NULL) {
    fprintf(stderr, "Could not get inquiry function 'ompt_get_parallel_info', "
                    "exiting...\n");
    exit(1);
  }

  SET_CALLBACK(thread_begin);
  SET_CALLBACK(thread_end);
  SET_CALLBACK(parallel_begin);
  SET_CALLBACK(implicit_task);
  SET_CALLBACK(sync_region);
  SET_CALLBACK(parallel_end);
  SET_CALLBACK(control_tool);

  SET_CALLBACK(task_create);
  // SET_OPTIONAL_CALLBACK_T(task_creation, task_creation, hasTaskCreation,
  // ompt_set_never);
  SET_CALLBACK(task_schedule);
  SET_CALLBACK(dependences);

  SET_CALLBACK(mutex_acquire);
  SET_CALLBACK_T(mutex_acquired, mutex);
  SET_CALLBACK_T(mutex_released, mutex);

  thread_local_clock = new THREAD_CLOCK(my_next_id(), 0);
  startTool(false);
  return 1; // success
}

static void ompt_tsan_finalize(ompt_data_t *tool_data) {
  if (!useMpi) {
    if (analysis_flags->verbose)
      std::cout << "Max Useful Computation -- " << crit_path_useful_time
                << std::endl;
    finishMeasurement();
  }
  if (analysis_flags->print_max_rss) {
    struct rusage end;
    getrusage(RUSAGE_SELF, &end);
    printf("MAX RSS[KBytes] during execution: %ld\n", end.ru_maxrss);
  }

  if (!useMpi && analysis_flags)
    delete analysis_flags;
}
extern "C" ompt_start_tool_result_t *__attribute__((visibility("default")))
ompt_start_tool(unsigned int omp_version, const char *runtime_version) {
  if (!analysis_flags) {
    const char *options = getenv(ANALYSIS_FLAGS);
    analysis_flags = new AnalysisFlags(options);
  }
  if (analysis_flags->verbose)
    std::cout << "Starting critPathAnalysis OMPT tool" << std::endl;
  if (!analysis_flags->enabled) {
    if (analysis_flags->verbose)
      std::cout << "Tool disabled, stopping operation" << std::endl;

    delete analysis_flags;
    return NULL;
  }

  pagesize = getpagesize();

  static ompt_start_tool_result_t ompt_start_tool_result = {
      &ompt_tsan_initialize, &ompt_tsan_finalize, {0}};

  // The OMPT start-up code uses dlopen with RTLD_LAZY. Therefore, we cannot
  // rely on dlopen to fail if TSan is missing, but would get a runtime error
  // for the first TSan call. We use RunningOnValgrind to detect whether
  // an implementation of the Annotation interface is available in the
  // execution or disable the tool (by returning NULL).

  if (analysis_flags->verbose && analysis_flags->stopped)
    std::cout << "Tool enabled, collecting critical path." << std::endl;
  if (analysis_flags->verbose && !analysis_flags->stopped)
    std::cout << "Tool enabled, waiting to get started." << std::endl;
  return &ompt_start_tool_result;
}

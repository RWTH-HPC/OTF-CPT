#ifndef CRITICALPATH_H
#define CRITICALPATH_H 1

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <atomic>
#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include <sys/resource.h>
#include <unistd.h>

#include "containers.h"
#include "debug.h"
#include "parse_flags.h"

#if (defined __APPLE__ && defined __MACH__)
#include <dlfcn.h>
#endif

using namespace __otfcpt;

#include <stdio.h>
#include <stdlib.h>

#include <omp-tools.h>
#include <omp.h>

#define LINESTR1(file, line) file ":" #line
#define LINESTR(file, line) LINESTR1(file, line)
#define GET_FILELINE LINESTR(__FILE__, __LINE__)

#ifdef __GNUC__
#define G_GNUC_CHECK_VERSION(major, minor)                                     \
  ((__GNUC__ <= (major)) && (__GNUC_MINOR__ <= (minor)))
// Mitigation necessary for 12.3 and bellow
#if G_GNUC_CHECK_VERSION(12, 3)
#define ATEXIT_MITIGATION
#endif
#endif

#ifdef DEBUG_CLOCKS
inline std::mutex debugClockMutex;
#endif

extern int myProcId;
extern bool useMpi;
extern double localTimeOffset;
extern long long startTimeOffset;

double getTime();

struct THREAD_CLOCK;
extern thread_local THREAD_CLOCK *thread_local_clock;
extern ompt_finalize_tool_t critical_ompt_finalize_tool;

int my_get_tid();

template <typename T>
static void update_maximum(std::atomic<T> &maximum_value,
                           T const &value) noexcept {
  T prev_value = maximum_value;
  while (prev_value < value &&
         !maximum_value.compare_exchange_weak(prev_value, value)) {
  }
}

template <typename value_type>
value_type atomic_add(std::atomic<value_type> &operand,
                      value_type value_to_add) {
  value_type old = operand.load(std::memory_order_consume);
  value_type desired = old + value_to_add;
  while (!operand.compare_exchange_weak(old, desired, std::memory_order_release,
                                        std::memory_order_consume))
    desired = old + value_to_add;

  return desired;
}

enum ClockState {
  STATE_UNINIT = -1,
  STATE_INIT = 0,
  STATE_NONE = 1,
  STATE_USEFUL = 2,
  STATE_MPI = 3,
  STATE_OMP = 4,
  STATE_GPU = 5,
  STATE_LAST = 6
};

enum ClockType {
  CLOCK_USEFUL = 0,
  CLOCK_OMPI = 1,
  CLOCK_OOMP = 2,
  CLOCK_OGPU = 3,
  CLOCK_LAST = 4
};

extern const char *debug_clock_state_string[];

#define STRING_CLOCK_STATE(a) debug_clock_state_string[((int)(a) + 1)]

#ifdef DEBUG_CLOCKS
#define CLOCK_DEBUG(a, b, c) DebugClocksRAII dcr = DebugClocksRAII(a, b, c)
class DebugClocksRAII {
  THREAD_CLOCK *tc;
  const char *loc;
  const char *func;

public:
  DebugClocksRAII(THREAD_CLOCK *_tc, const char *_loc, const char *_func);
  ~DebugClocksRAII();
};
#else
#define CLOCK_DEBUG(a, b, c)
#endif

struct CP_CLOCKS {
  std::atomic<double> thread{0};
  std::atomic<double> proc{0};
  std::atomic<double> critical{0};

  CP_CLOCKS &operator=(const CP_CLOCKS &other) {
    if (this != &other) {
      thread.store(other.thread.load());
      proc.store(other.proc.load());
      critical.store(other.critical.load());
    }
    return *this;
  }

  void Reset(double time) {
    thread = time;
    proc = time;
    critical = time;
  }

  void AddAll(double time) {
    atomic_add(critical, time);
    atomic_add(thread, time);
    atomic_add(proc, time);
  }

  void OmpHBefore(CP_CLOCKS &cc) {
    update_maximum(proc, cc.proc.load());
    update_maximum(critical, cc.critical.load());
  }
  void OmpHAfter(CP_CLOCKS &cc) {
    update_maximum(cc.proc, proc.load());
    update_maximum(cc.critical, critical.load());
  }
};

struct SYNC_CLOCK {
  CP_CLOCKS clocks[CLOCK_LAST]{};
  ClockState sync_state{STATE_INIT};
  const char *init_loc{nullptr};
  const char *init_fileline{nullptr};
  SYNC_CLOCK(double _useful_computation) {
    clocks[CLOCK_USEFUL].critical = _useful_computation;
  }
  SYNC_CLOCK(double _useful_computation, double _mpi_start_time) {
    clocks[CLOCK_USEFUL].critical = _useful_computation;
    clocks[CLOCK_OMPI].proc = _mpi_start_time;
    clocks[CLOCK_OMPI].thread = _mpi_start_time;
    clocks[CLOCK_OMPI].critical = _mpi_start_time;
  }
  SYNC_CLOCK() {}
  void CheckArc(const char *loc, THREAD_CLOCK *tc = thread_local_clock);
  void CheckArc(const char *loc, const char *fileline,
                THREAD_CLOCK *tc = thread_local_clock);
  void OmpHBefore(const char *loc, THREAD_CLOCK *tc = thread_local_clock);
  void OmpHBefore(const char *loc, const char *fileline,
                  THREAD_CLOCK *tc = thread_local_clock);
  void OmpHAfter(const char *loc, THREAD_CLOCK *tc = thread_local_clock);
  void OmpHAfter(const char *loc, const char *fileline,
                 THREAD_CLOCK *tc = thread_local_clock);
  void Print(const char *prefix1, const char *prefix2 = "",
             const char *prefix3 = "") {
    fprintf(
        analysis_flags->output,
        "Thread %d: "
        "%s (%p) %s%s: "
        "uct=%lf, ucp=%lf, ucc=%lf, "
        "omt=%lf, omp=%lf, omc=%lf, "
        "oot=%lf, oop=%lf, ooc=%lf\n",
        my_get_tid(), prefix1, this, prefix2, prefix3,
        clocks[CLOCK_USEFUL].thread.load(), clocks[CLOCK_USEFUL].proc.load(),
        clocks[CLOCK_USEFUL].critical.load(), clocks[CLOCK_OMPI].thread.load(),
        clocks[CLOCK_OMPI].proc.load(), clocks[CLOCK_OMPI].critical.load(),
        clocks[CLOCK_OOMP].thread.load(), clocks[CLOCK_OOMP].proc.load(),
        clocks[CLOCK_OOMP].critical.load());
  }

  void *operator new(size_t size) { return malloc(size); }

  void operator delete(void *p) { free(p); }
};

enum ClockContext {
  CLOCK_OMP,
  CLOCK_OMP_ONLY,
  CLOCK_MPI,
  CLOCK_MPI_ONLY,
  CLOCK_ALL
};

static const bool State[STATE_LAST][CLOCK_LAST] = {
    {false, false, false, false}, // INIT
    {false, true, true, true},    // NONE
    {true, true, true, true},     // USEFUL
    {false, false, true, true},   // MPI
    {false, true, false, true},   // OMP
    {false, true, true, false}    // GPU
}; // USEFUL, OMPI, OOMP, OGPU

struct MPI_COUNTS {
  uint64_t send{0}, recv{0}, isend{0}, irecv{0}, coll{0}, icoll{0}, test{0},
      wait{0}, pers{0}, probe{0};
  void add(const MPI_COUNTS &o) {
    send += o.send;
    recv += o.recv;
    isend += o.isend;
    irecv += o.irecv;
    coll += o.coll;
    icoll += o.icoll;
    test += o.test;
    wait += o.wait;
    pers += o.pers;
    probe += o.probe;
  }

  void *operator new(size_t size) { return malloc(size); }

  void operator delete(void *p) { free(p); }
};

struct omptCounts {
  int taskCreate{0};
  int taskSchedule{0};
  int implTaskBegin{0};
  int implTaskEnd{0};
  int syncRegionBegin{0};
  int syncRegionEnd{0};
  int mutexAcquire{0};
  void add(const omptCounts &o) {
    taskCreate += o.taskCreate;
    taskSchedule += o.taskSchedule;
    implTaskBegin += o.implTaskBegin;
    implTaskEnd += o.implTaskEnd;
    syncRegionBegin += o.syncRegionBegin;
    syncRegionEnd += o.syncRegionEnd;
    mutexAcquire += o.mutexAcquire;
  }

  void *operator new(size_t size) { return malloc(size); }

  void operator delete(void *p) { free(p); }
};

struct THREAD_CLOCK : public SYNC_CLOCK, MPI_COUNTS {
  int thread_id{-1};
  bool openmp_thread{false};
  Vector<ClockState> clock_state_stack;

  THREAD_CLOCK(int threadid, double _useful_computation,
               bool _openmp_thread = false)
      : SYNC_CLOCK(_useful_computation,
                   (!analysis_flags->running) ? 0 : -getTime()),
        thread_id(threadid), openmp_thread(_openmp_thread) {
    clock_state_stack.PushBack(STATE_INIT);
  }
  THREAD_CLOCK() {}
  THREAD_CLOCK(const THREAD_CLOCK &other);

  void SwitchState(ClockState old_cs, ClockState new_cs, double time = 0,
                   const char *loc = NULL) {
    if (old_cs == new_cs)
      return;
    if (time == 0)
      time = getTime();

    for (int i = 0; i < CLOCK_LAST; i++) {
      if (State[old_cs][i] == State[new_cs][i]) {
        continue;
      } else if (!State[old_cs][i] && State[new_cs][i]) {
        clocks[i].AddAll(-time);
      } else {
        clocks[i].AddAll(time);
      }
    }
  }

#ifdef DEBUG_CLOCKS
  void inline printStateStack(const char *loc = "", const char *prefix = "") {
    fprintf(analysis_flags->output,
            "Thread %i: Clock State Stack at %s%s: ", my_get_tid(), loc,
            prefix);
    for (auto elem : clock_state_stack) {
      fprintf(analysis_flags->output, "%s ", STRING_CLOCK_STATE(elem));
    }
    fprintf(analysis_flags->output, "[back]\n");
  }
#endif

  void enterState(ClockState cs, const char *loc = NULL) {
    enterState(0, cs, loc);
  }

  void enterState(double time, ClockState cs, const char *loc = NULL) {
    if (!analysis_flags->running)
      return;
    CLOCK_DEBUG(this, loc, __func__);
    SwitchState(clock_state_stack.Back(), cs, time, loc);
    clock_state_stack.PushBack(cs);
  }

  void exitState(const char *loc = NULL, bool isRunning = true) {
    exitState(0, loc, isRunning);
  }

  void exitState(ClockState oldcs, ClockState nextcs, const char *loc = NULL,
                 bool isRunning = true) {
    if (!analysis_flags->running || !isRunning)
      return;
    DCHECK_EQ(oldcs, clock_state_stack.Back());
    exitState(0, loc, isRunning);
    DCHECK_EQ(nextcs, clock_state_stack.Back());
  }

  void exitState(double time, const char *loc = NULL, bool isRunning = true) {
    if (!analysis_flags->running || !isRunning)
      return;
    CLOCK_DEBUG(this, loc, __func__);
    ClockState old_cs = clock_state_stack.Back();
    // Having STATE_INIT as anything but the bottom most element is invalid
    DCHECK_OR(clock_state_stack.Size() > 1, old_cs == STATE_INIT);
    if (old_cs == STATE_INIT)
      return;
    clock_state_stack.PopBack();
    SwitchState(old_cs, clock_state_stack.Back(), time, loc);
  }

  void setState(ClockState cs, const char *loc = NULL) { setState(0, cs, loc); }

  void setState(double time, ClockState cs, const char *loc = NULL) {
    if (!analysis_flags->running)
      return;
    CLOCK_DEBUG(this, loc, __func__);
    SwitchState(clock_state_stack.Back(), cs, time, loc);
    clock_state_stack.Back() = cs;
  }

  void resetState() {
    for (int i = CLOCK_USEFUL; i < CLOCK_LAST; i++)
      clocks[i].Reset(0);

    clock_state_stack.Reset();
    clock_state_stack.PushBack(STATE_INIT);
  }

  bool compareState(ClockState cs) const {
    return clock_state_stack.getBack() == cs;
  }

  ClockState getState() const { return clock_state_stack.getBack(); }

  void *operator new(size_t size) { return malloc(size); }

  void operator delete(void *p) { free(p); }
};

typedef SYNC_CLOCK ompt_tsan_clockid;
extern thread_local THREAD_CLOCK *thread_local_clock;
extern Vector<THREAD_CLOCK *> *thread_clocks;
extern Vector<omptCounts *> *thread_counts;
extern double startProgrammTime;
extern double crit_path_useful_time;

uint64_t my_next_id();

void resetMpiClock(THREAD_CLOCK *thread_clock);

void startTool(bool toolControl = true, ClockState cs = STATE_USEFUL);
void stopTool();

#define OmpHappensBefore(cv, ...)                                              \
  (cv)->OmpHBefore(__PRETTY_FUNCTION__, GET_FILELINE, ##__VA_ARGS__)
#define OmpHappensAfter(cv, ...)                                               \
  (cv)->OmpHAfter(__PRETTY_FUNCTION__, GET_FILELINE, ##__VA_ARGS__)

void OmpClockReset(THREAD_CLOCK *cv);
void OmpClockReset(SYNC_CLOCK *cv);

void startMeasurement(double time = getTime());
void stopMeasurement(double time = getTime());

void finishMeasurement();

extern "C" void enterOpenMP(const char *loc);
extern "C" void exitOpenMP(const char *loc);

#endif
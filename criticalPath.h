#ifndef CRITICALPATH_H
#define CRITICALPATH_H 1

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <atomic>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include <sys/resource.h>
#include <unistd.h>

#include "containers.h"
#include "parse_flags.h"

#if (defined __APPLE__ && defined __MACH__)
#include <dlfcn.h>
#endif

using namespace __otfcpt;

#include <stdio.h>
#include <stdlib.h>

#include <omp-tools.h>

// #define DEBUG_CLOCKS 1

extern int myProcId;
extern bool useMpi;
extern double localTimeOffset;

double getTime();
double getStartTimeNoOffset(double time);
double getStopTimeNoOffset(double time);

struct THREAD_CLOCK;
extern thread_local THREAD_CLOCK *thread_local_clock;
extern ompt_finalize_tool_t critical_ompt_finalize_tool;

#define analysis_flags get_otfcpt_flags()

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

struct SYNC_CLOCK {
  std::atomic<double> useful_computation_thread{0};
  std::atomic<double> useful_computation_proc{0};
  std::atomic<double> useful_computation_critical{0};
  std::atomic<double> outsidempi_proc{-1e50};
  std::atomic<double> outsidempi_thread{-1e50};
  std::atomic<double> outsidempi_critical{-1e50};
  std::atomic<double> outsideomp_thread{0};
  std::atomic<double> outsideomp_proc{0};
  std::atomic<double> outsideomp_critical{0};
  std::atomic<double> outsideomp_critical_nooffset{0};
  SYNC_CLOCK(double _useful_computation)
      : useful_computation_critical(_useful_computation) {}
  SYNC_CLOCK(double _useful_computation, double _mpi_start_time)
      : useful_computation_critical(_useful_computation),
        outsidempi_proc(_mpi_start_time), outsidempi_thread(_mpi_start_time),
        outsidempi_critical(_mpi_start_time) {}
  SYNC_CLOCK() {}
  void OmpHBefore();
  void OmpHAfter();
  void Print(const char *prefix1, const char *prefix2 = "") {
    printf("%s %s: ucp=%lf, uct=%lf, ucc=%lf, omp=%lf, omt=%lf, omc=%lf, "
           "oop=%lf, oot=%lf, ooc=%lf, oocno=%lf\n",
           prefix1, prefix2, useful_computation_proc.load(),
           useful_computation_thread.load(), useful_computation_critical.load(),
           outsidempi_proc.load(), outsidempi_thread.load(),
           outsidempi_critical.load(), outsideomp_proc.load(),
           outsideomp_thread.load(), outsideomp_critical.load(),
           outsideomp_critical_nooffset.load());
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
  bool stopped_clock{true};
  //  bool stopped_mpi_clock{false};
  bool stopped_mpi_clock{true};
  bool stopped_omp_clock{true};

  THREAD_CLOCK(int threadid, double _useful_computation,
               bool _openmp_thread = false)
      : SYNC_CLOCK(_useful_computation,
                   (!analysis_flags->running) ? 0 : -getTime()),
        thread_id(threadid), openmp_thread(_openmp_thread),
        stopped_mpi_clock(!analysis_flags->running) {}
  THREAD_CLOCK() {}

  void Stop(ClockContext cc = CLOCK_OMP, const char *loc = NULL) {
    if (cc != CLOCK_OMP_ONLY && !analysis_flags->running)
      return;
    Stop(getTime(), cc, loc);
  }

  void Stop(double time, ClockContext cc = CLOCK_OMP, const char *loc = NULL) {

    assert(loc);
#if DEBUG_CLOCKS
    Print("Stopping at ", loc);
#endif
    if (cc != CLOCK_OMP_ONLY) {
      if (!analysis_flags->running)
        return;
      if (cc != CLOCK_MPI_ONLY) {
        assert(!openmp_thread || stopped_clock == false);
        stopped_clock = true;
        atomic_add(useful_computation_critical, time);
        atomic_add(useful_computation_thread, time);
        atomic_add(useful_computation_proc, time);
      }
      if (cc != CLOCK_OMP) {
        assert(stopped_mpi_clock == false);
        stopped_mpi_clock = true;
        atomic_add(outsidempi_proc, time);
        atomic_add(outsidempi_critical, time);
        atomic_add(outsidempi_thread, time);
      }
    }
    if (cc != CLOCK_MPI && cc != CLOCK_MPI_ONLY) {
      assert(!openmp_thread || stopped_omp_clock == false);
      stopped_omp_clock = true;
      atomic_add(outsideomp_thread, time);
      atomic_add(outsideomp_critical, time);
      atomic_add(outsideomp_critical_nooffset, getStopTimeNoOffset(time));
      atomic_add(outsideomp_proc, time);
    }
#if DEBUG_CLOCKS
    Print("Stopped at ", loc);
#endif
  }

  void Start(ClockContext cc = CLOCK_OMP, const char *loc = NULL) {
    if (!analysis_flags->running)
      return;
    Start(-getTime(), cc, loc);
  }

  void Start(double time, ClockContext cc = CLOCK_OMP, const char *loc = NULL) {

    assert(loc);
#if DEBUG_CLOCKS
    Print("Starting at ", loc);
#endif
    if (cc != CLOCK_OMP_ONLY) {
      if (!analysis_flags->running)
        return;
      if (cc != CLOCK_MPI_ONLY) {
        assert(!openmp_thread || stopped_clock == true);
        stopped_clock = false;
        atomic_add(useful_computation_critical, time);
        atomic_add(useful_computation_thread, time);
        atomic_add(useful_computation_proc, time);
      }
      if (cc != CLOCK_OMP) {
        assert(stopped_mpi_clock == true);
        stopped_mpi_clock = false;
        atomic_add(outsidempi_proc, time);
        atomic_add(outsidempi_critical, time);
        atomic_add(outsidempi_thread, time);
      }
    }
    if (cc != CLOCK_MPI && cc != CLOCK_MPI_ONLY) {
      assert(!openmp_thread || stopped_omp_clock == true);
      stopped_omp_clock = false;
      atomic_add(outsideomp_thread, time);
      atomic_add(outsideomp_critical, time);
      atomic_add(outsideomp_critical_nooffset, getStartTimeNoOffset(time));
      atomic_add(outsideomp_proc, time);
    }
#if DEBUG_CLOCKS
    Print("Started at ", loc);
#endif
  }

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

#define stopClock(t) (t)->Stop(CLOCK_OMP, __func__)
#define startClock(t) (t)->Start(CLOCK_OMP, __func__)
#define stopMpiClock(t) (t)->Stop(CLOCK_MPI, __func__)
#define startMpiClock(t) (t)->Start(CLOCK_MPI, __func__)
#define stopOmpClock(t) (t)->Stop(CLOCK_OMP_ONLY, __func__)
void resetMpiClock(THREAD_CLOCK *thread_clock);

void startTool(bool toolControl = true, ClockContext cc = CLOCK_ALL);
void stopTool();

#define OmpHappensBefore(cv) (cv)->OmpHBefore();
#define OmpHappensAfter(cv) (cv)->OmpHAfter();
void OmpClockReset(THREAD_CLOCK *cv);
void OmpClockReset(SYNC_CLOCK *cv);

void startMeasurement(double time = getTime());
void stopMeasurement(double time = getTime());

void finishMeasurement();

#endif
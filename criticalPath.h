#ifndef CRITICALPATH_H
#define CRITICALPATH_H 1

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <sys/resource.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#if (defined __APPLE__ && defined __MACH__)
#include <dlfcn.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <omp-tools.h>

//#define DEBUG_CLOCKS 1

#define ANALYSIS_FLAGS "OTFCPT_OPTIONS"

extern int myProcId;
extern bool useMpi;
extern double localTimeOffset;

double getTime();
double getStartTimeNoOffset(double time);
double getStopTimeNoOffset(double time);

struct THREAD_CLOCK;
extern thread_local THREAD_CLOCK *thread_local_clock;
extern ompt_finalize_tool_t critical_ompt_finalize_tool;

class AnalysisFlags {
public:
#if (LLVM_VERSION) >= 40
  int flush_shadow{0};
#endif
  int print_max_rss{0};
  int verbose{0};
  int enabled{1};
  int stopped{0};
  bool running{false};
  int report_data_leak{0};
  int ignore_serial{0};
  int analyze_pattern{1};
  int colorize{1};

  AnalysisFlags(const char *env) {
    if (env) {
      std::vector<std::string> tokens;
      std::string token;
      std::string str(env);
      std::istringstream iss(str);
      while (std::getline(iss, token, ' '))
        tokens.push_back(token);

      for (std::vector<std::string>::iterator it = tokens.begin();
           it != tokens.end(); ++it) {
#if (LLVM_VERSION) >= 40
        if (sscanf(it->c_str(), "flush_shadow=%d", &flush_shadow))
          continue;
#endif
        if (sscanf(it->c_str(), "print_max_rss=%d", &print_max_rss))
          continue;
        if (sscanf(it->c_str(), "verbose=%d", &verbose))
          continue;
        if (sscanf(it->c_str(), "start_stopped=%d", &stopped))
          continue;
        if (sscanf(it->c_str(), "report_data_leak=%d", &report_data_leak))
          continue;
        if (sscanf(it->c_str(), "enable=%d", &enabled))
          continue;
        if (sscanf(it->c_str(), "colorize=%d", &colorize))
          continue;
        if (sscanf(it->c_str(), "ignore_serial=%d", &ignore_serial))
          continue;
        std::cerr << "Illegal values for ANALYSIS_OPTIONS variable: " << token
                  << std::endl;
      }
    }
  }
};

extern AnalysisFlags *analysis_flags;

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
};

enum ClockContext {
  CLOCK_OMP,
  CLOCK_OMP_ONLY,
  CLOCK_MPI,
  CLOCK_MPI_ONLY,
  CLOCK_ALL
};

struct MPI_COUNTS {
  int send{0}, recv{0}, isend{0}, irecv{0}, coll{0}, icoll{0}, test{0}, wait{0},
      pers{0};
  void add(MPI_COUNTS o) {
    send += o.send;
    recv += o.recv;
    isend += o.isend;
    irecv += o.irecv;
    coll += o.coll;
    icoll += o.icoll;
    test += o.test;
    wait += o.wait;
    pers += o.pers;
  }
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
};

typedef SYNC_CLOCK ompt_tsan_clockid;
extern thread_local THREAD_CLOCK *thread_local_clock;
extern std::vector<THREAD_CLOCK *> thread_clocks;
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
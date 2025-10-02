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
#include "errorhandler.h"
#include "parse_flags.h"
#include <time.h>
#ifdef USE_MPI
#include <mpi.h>
#endif

extern "C" void __cxa_pure_virtual() {
  assert(false && "Pure virtual function must not be called");
  abort();
}

#ifdef USE_ERRHANDLER
#include <execinfo.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#define CALLSTACK_SIZE 20
static void print_stack(void) {
  int nptrs;
  void *buf[CALLSTACK_SIZE + 1];
  nptrs = backtrace(buf, CALLSTACK_SIZE);
  backtrace_symbols_fd(buf, nptrs, STDOUT_FILENO);
}

static void set_signalhandlers(sighandler_t handler) {
  signal(SIGSEGV, handler);
  signal(SIGINT, handler);
  signal(SIGHUP, handler);
  signal(SIGABRT, handler);
  signal(SIGTERM, handler);
  signal(SIGUSR2, handler);
  signal(SIGQUIT, handler);
  signal(SIGALRM, handler);
}

void disable_signalhandlers() { set_signalhandlers(SIG_DFL); }

void mySignalHandler(int signum) {
  disable_signalhandlers();
  printf("pid %i caught signal nr %i\n", getpid(), signum);
  if (signum == SIGINT || signum == SIGKILL) {
    print_stack();
    _exit(signum + 128);
  }
  if (signum == SIGTERM || signum == SIGUSR2) {
    print_stack();
    fflush(stdout);
    sleep(1);
    _exit(signum + 128);
  }
  print_stack();

  printf("Waiting up to %i seconds to attach with a debugger.\n", 20);
  sleep(20);
  _exit(1);
}

void init_signalhandlers() {
  if (!analysis_flags->stacktrace)
    return;
  if (analysis_flags->verbose)
    fprintf(analysis_flags->output, "Setting signal handlers\n");
  set_signalhandlers(mySignalHandler);
}

#endif

double localTimeOffset{0};
double getTime() {
  struct timespec curr;
  clock_gettime(CLOCK_REALTIME, &curr);
  return curr.tv_sec + curr.tv_nsec * 1e-9 - localTimeOffset;
}

double getStopTimeNoOffset(double time) { return time + localTimeOffset; }
double getStartTimeNoOffset(double time) { return time - localTimeOffset; }

int myProcId = 0;
bool useMpi = false;

ompt_finalize_tool_t critical_ompt_finalize_tool{nullptr};

uint64_t my_next_id() {
  static uint64_t ID = 0;
  uint64_t ret = __sync_fetch_and_add(&ID, 1);
  return ret;
}

double totalProgrammTime = 0;
double startProgrammTime = getTime(), endProgrammTime;
double crit_path_useful_time = 0;

Vector<THREAD_CLOCK *> *thread_clocks = nullptr;
Vector<omptCounts *> *thread_counts = nullptr;
thread_local THREAD_CLOCK *thread_local_clock = nullptr;

void resetMpiClock(THREAD_CLOCK *thread_clock) {
  if (!analysis_flags->running) {
    thread_clock->stopped_mpi_clock = true;
    thread_clock->outsidempi_proc = 0;
    thread_clock->outsidempi_critical = 0;
    thread_clock->outsidempi_thread = 0;
  } else if (!thread_clock->openmp_thread) {
    thread_clock->stopped_mpi_clock = true;
    thread_clock->stopped_clock = true;
    thread_clock->stopped_omp_clock = false;
    OmpClockReset(thread_clock);
  } else {
    assert(thread_clock->stopped_mpi_clock == false);
    thread_clock->stopped_mpi_clock = true;
    thread_clock->outsidempi_proc = 0;
    thread_clock->outsidempi_critical = 0;
    thread_clock->outsidempi_thread = 0;
    if (thread_clock->stopped_clock == false) {
      thread_clock->stopped_clock = true;
      thread_clock->useful_computation_proc = 0;
      thread_clock->useful_computation_critical = 0;
      thread_clock->useful_computation_thread = 0;
    }
    if (thread_local_clock->stopped_omp_clock == true)
      thread_local_clock->Start(CLOCK_OMP_ONLY, __func__);
  }
}

void startMeasurement(double time) {
  auto initialStart = startProgrammTime;
  startProgrammTime = time;
  if (analysis_flags->verbose)
    fprintf(analysis_flags->output, "Initialize after %lf s\n",
            startProgrammTime - initialStart);
}

void stopMeasurement(double time) { endProgrammTime = time; }

#define NUM_SHARED_METRICS 7

void finishMeasurement() {
  static int finished = 0;
  if (finished)
    return;
  finished = 1;
  int number_of_procs = 1;
  int total_threads = 0;
  int num_threads = 0;
  if (thread_clocks)
    total_threads = num_threads = thread_clocks->Size();

  double avgComputation[NUM_SHARED_METRICS] = {0};
  double maxComputation[NUM_SHARED_METRICS] = {0};
  double uc_avg[NUM_SHARED_METRICS] = {0};
  double uc_max[NUM_SHARED_METRICS] = {0};
  if (analysis_flags->running) {
    endProgrammTime = getTime();
    analysis_flags->running = false;
  }
  if (thread_local_clock->stopped_omp_clock == false)
    thread_local_clock->Stop(endProgrammTime, CLOCK_OMP_ONLY, __func__);

  double totalRuntimeReal = endProgrammTime - startProgrammTime;
  // get max and avg Computation accross all threads
  MPI_COUNTS proc_counts, total_counts;
  if (num_threads > 0) {
    for (int i = 0; i < num_threads; i++) {
      auto *tclock = (*thread_clocks)[i];
      proc_counts.add(*tclock);
      assert(tclock->stopped_clock == true);
      assert(tclock->stopped_omp_clock == true);
      double curr_uc = tclock->useful_computation_thread.load();
      double curr_oot = tclock->outsideomp_thread.load();
      if (curr_uc > uc_max[0]) {
        uc_max[0] = curr_uc;
      }
      if (curr_oot > uc_max[2]) {
        uc_max[2] = curr_oot;
      }
      uc_avg[0] += curr_uc;
      uc_avg[2] += curr_oot;
    }
    if (thread_counts)
      for (int i = 1; i < thread_counts->Size(); i++)
        (*thread_counts)[0]->add(*(*thread_counts)[i]);
  } else {
    num_threads = 1;
    uc_max[0] = uc_avg[0] =
        thread_local_clock->useful_computation_thread.load();
    uc_max[2] = uc_avg[2] = thread_local_clock->outsideomp_thread.load();
    proc_counts.add(*thread_local_clock);
  }
  uc_max[1] = uc_avg[1] = thread_local_clock->outsidempi_proc.load();
  uc_max[3] = uc_avg[3] = thread_local_clock->useful_computation_proc.load();
  uc_max[4] = uc_avg[4] = thread_local_clock->outsideomp_proc.load();
  uc_max[5] = uc_avg[5] = uc_avg[3] - uc_avg[4];
  uc_avg[5] = uc_avg[5] * num_threads;
  uc_max[6] = uc_max[0];
  uc_avg[6] = uc_max[0] * num_threads;

#ifdef USE_MPI
  if (useMpi) {
    // aggregate all max and avg computations on the master thread
    PMPI_Reduce(&uc_max, &maxComputation, NUM_SHARED_METRICS, MPI_DOUBLE,
                MPI_MAX, 0, MPI_COMM_WORLD);
    PMPI_Reduce(&uc_avg, &avgComputation, NUM_SHARED_METRICS, MPI_DOUBLE,
                MPI_SUM, 0, MPI_COMM_WORLD);
    PMPI_Reduce(&num_threads, &total_threads, 1, MPI_INT, MPI_SUM, 0,
                MPI_COMM_WORLD);
    PMPI_Reduce(&proc_counts, &total_counts,
                sizeof(proc_counts) / sizeof(uint64_t), MPI_UINT64_T, MPI_SUM,
                0, MPI_COMM_WORLD);
    double localRuntimeReal = totalRuntimeReal;
    PMPI_Reduce(&localRuntimeReal, &totalRuntimeReal, 1, MPI_DOUBLE, MPI_MAX, 0,
                MPI_COMM_WORLD);
    PMPI_Comm_size(MPI_COMM_WORLD, &number_of_procs);
    avgComputation[1] = avgComputation[1] / number_of_procs;
    avgComputation[3] = avgComputation[3] / number_of_procs;
    avgComputation[4] = avgComputation[4] / number_of_procs;
    avgComputation[0] = avgComputation[0] / total_threads;
    avgComputation[2] = avgComputation[2] / total_threads;
    avgComputation[5] = avgComputation[5] / total_threads;
    avgComputation[6] = avgComputation[6] / total_threads;
  } else
#endif
  {
    for (int i = 0; i < NUM_SHARED_METRICS; i++) {
      maxComputation[i] = uc_max[i];
      avgComputation[i] = uc_avg[i];
    }
    avgComputation[0] /= num_threads;
    avgComputation[2] /= num_threads;
    avgComputation[5] /= num_threads;
    avgComputation[6] /= num_threads;
  }
  if (myProcId == 0) { // display results on master thread
                       // calculate pop metrics
    double totalRuntimeIdeal =
        thread_local_clock->useful_computation_critical.load();
    double totalOutsideMPIIdeal =
        thread_local_clock->outsidempi_critical.load();
    double totalOutsideOMPIdeal =
        thread_local_clock->outsideomp_critical.load();
    double totalOutsideOMPIdealNoOffset =
        thread_local_clock->outsideomp_critical_nooffset.load();

    avgComputation[5] = avgComputation[5] + totalRuntimeReal;
    maxComputation[5] = maxComputation[5] + totalRuntimeReal;

    double CommE = maxComputation[0] / totalRuntimeReal;
    double TE = totalRuntimeIdeal / totalRuntimeReal;
    double SerE = maxComputation[0] / totalRuntimeIdeal;
    double LB = avgComputation[0] / maxComputation[0];
    double PE = LB * CommE;

    double mpiLB = avgComputation[6] / maxComputation[0];
    double ompLB = LB / mpiLB;

    double ompTE = totalOutsideOMPIdealNoOffset / totalRuntimeReal;
    double mpiTE = TE / ompTE;

    double mpiSerE = maxComputation[3] / totalRuntimeIdeal;
    double ompSerE = SerE / mpiSerE;

    double mpiCommE = mpiSerE * mpiTE;
    double ompCommE = ompSerE * ompTE;

    double ompPE = ompLB * ompCommE;
    double mpiPE = mpiLB * mpiCommE;

    FILE *of = stdout;
    bool openedFile{false};

    if (analysis_flags->data_path) {
      if (strcmp("stdout", analysis_flags->data_path) == 0) {
        of = stdout;
      } else if (strcmp("stderr", analysis_flags->data_path) == 0) {
        of = stderr;
      } else {
        auto len = strlen(analysis_flags->data_path);
        char *tempath = (char *)malloc(len + 15);
        sprintf(tempath, "%s-%ix%i.txt", analysis_flags->data_path,
                number_of_procs, num_threads);
        of = fopen(tempath, "w");
        if (analysis_flags->verbose) {
          fprintf(analysis_flags->output, "Opened file %s for output\n",
                  tempath);
        }
        free(tempath);
        openedFile = true;
      }
    }

    if (analysis_flags->verbose) {
      fprintf(
          of,
          "\n[pop] "
          "sere:%6.3lf:te:%6.3lf:comme:%6.3lf:lb:%6.3lf:pe:%6.3lf:crittime:%2."
          "5lf:totaltime:%6.3lf:avgcomputation:%6.3lf:maxcomputation:%6.3lf\n",
          SerE, TE, CommE, LB, PE, totalRuntimeIdeal, totalRuntimeReal,
          avgComputation[0], maxComputation[0]);
      fprintf(of, "\n\n--------MPI stats:--------\n");
      if (total_counts.send)
        fprintf(of, "MPI_*send: %lu\n", total_counts.send);
      if (total_counts.isend)
        fprintf(of, "MPI_I*send: %lu\n", total_counts.isend);
      if (total_counts.probe)
        fprintf(of, "MPI_*mprobe: %lu\n", total_counts.probe);
      if (total_counts.recv)
        fprintf(of, "MPI_Recv: %lu\n", total_counts.recv);
      if (total_counts.irecv)
        fprintf(of, "MPI_Irecv: %lu\n", total_counts.irecv);
      if (total_counts.coll)
        fprintf(of, "MPI_*coll: %lu\n", total_counts.coll);
      if (total_counts.coll)
        fprintf(of, "MPI_I*coll: %lu\n", total_counts.coll);
      if (total_counts.test)
        fprintf(of, "MPI_Test*: %lu\n", total_counts.test);
      if (total_counts.wait)
        fprintf(of, "MPI_Wait*: %lu\n", total_counts.wait);

      if (thread_counts) {
        fprintf(of, "\n\n--------OMPT stats:--------\n");
        auto *tCounts = (*thread_counts)[0];
        if (tCounts->taskCreate)
          fprintf(of, "taskCreate: %i\n", tCounts->taskCreate);
        if (tCounts->taskSchedule)
          fprintf(of, "taskSchedule: %i\n", tCounts->taskSchedule);
        if (tCounts->implTaskBegin)
          fprintf(of, "implTaskBegin: %i\n", tCounts->implTaskBegin);
        if (tCounts->implTaskEnd)
          fprintf(of, "implTaskEnd: %i\n", tCounts->implTaskEnd);
        if (tCounts->syncRegionBegin)
          fprintf(of, "syncRegionBegin: %i\n", tCounts->syncRegionBegin);
        if (tCounts->syncRegionEnd)
          fprintf(of, "syncRegionEnd: %i\n", tCounts->syncRegionEnd);
        if (tCounts->mutexAcquire)
          fprintf(of, "mutexAcquire: %i\n", tCounts->mutexAcquire);
      }

      fprintf(of, "\n\n--------CritPath Analysis Tool results:--------\n");
      fprintf(of, "=> Number of processes:          %i\n", number_of_procs);
      fprintf(of, "=> Number of threads:            %i\n", total_threads);
      fprintf(of, "=> Average Computation (in s):   %6.3lf\n",
              avgComputation[0]);
      fprintf(of, "=> Maximum Computation (in s):   %6.3lf\n",
              maxComputation[0]);
      fprintf(of, "=> Max crit. computation (in s): %6.3lf\n",
              totalRuntimeIdeal);
      fprintf(of, "=> Average crit. proc-local computation (in s):    %6.3lf\n",
              avgComputation[3]);
      fprintf(of, "=> Maximum crit. proc-local computation (in s):    %6.3lf\n",
              maxComputation[3]);
      fprintf(of, "=> Average crit. proc-local Outside OpenMP (in s): %6.3lf\n",
              avgComputation[4]);
      fprintf(of, "=> Maximum crit. proc-local Outside OpenMP (in s): %6.3lf\n",
              maxComputation[4]);
      fprintf(of, "=> Average proc-local rumtime (in s):   %6.3lf\n",
              avgComputation[5]);
      fprintf(of, "=> Maximum proc-local runtime (in s):   %6.3lf\n",
              maxComputation[5]);
      fprintf(of, "=> Average PL-max Computation (in s):   %6.3lf\n",
              avgComputation[6]);
      fprintf(of, "=> Maximum PL-max Computation (in s):   %6.3lf\n",
              maxComputation[6]);
      fprintf(of, "=> Average Outside OpenMP (in s):   %6.3lf\n",
              avgComputation[2]);
      fprintf(of, "=> Maximum Outside OpenMP (in s):   %6.3lf\n",
              maxComputation[2]);
      fprintf(of, "=> Max crit. Outside OpenMP (in s): %6.3lf\n",
              totalOutsideOMPIdeal);
      fprintf(of, "=> Max crit. Outside OpenMP w/o o,%6.3lf\n",
              totalOutsideOMPIdealNoOffset);
      fprintf(of, "=> Total runtime (in s):         %6.3lf\n",
              totalRuntimeReal);
    }

    fprintf(of, "\n----------------POP metrics----------------\n");
    fprintf(of, "Parallel Efficiency:                %6.3lf\n",
            PE * analysis_flags->metric_factor);
    fprintf(of, "  Load Balance:                     %6.3lf\n",
            LB * analysis_flags->metric_factor);
    fprintf(of, "  Communication Efficiency:         %6.3lf\n",
            CommE * analysis_flags->metric_factor);
    fprintf(of, "    Serialisation Efficiency:       %6.3lf\n",
            SerE * analysis_flags->metric_factor);
    fprintf(of, "    Transfer Efficiency:            %6.3lf\n",
            TE * analysis_flags->metric_factor);
    fprintf(of, "  MPI Parallel Efficiency:          %6.3lf\n",
            mpiPE * analysis_flags->metric_factor);
    fprintf(of, "    MPI Load Balance:               %6.3lf\n",
            mpiLB * analysis_flags->metric_factor);
    fprintf(of, "    MPI Communication Efficiency:   %6.3lf\n",
            mpiCommE * analysis_flags->metric_factor);
    fprintf(of, "      MPI Serialisation Efficiency: %6.3lf\n",
            mpiSerE * analysis_flags->metric_factor);
    fprintf(of, "      MPI Transfer Efficiency:      %6.3lf\n",
            mpiTE * analysis_flags->metric_factor);
    fprintf(of, "  OMP Parallel Efficiency:          %6.3lf\n",
            ompPE * analysis_flags->metric_factor);
    fprintf(of, "    OMP Load Balance:               %6.3lf\n",
            ompLB * analysis_flags->metric_factor);
    fprintf(of, "    OMP Communication Efficiency:   %6.3lf\n",
            ompCommE * analysis_flags->metric_factor);
    fprintf(of, "      OMP Serialisation Efficiency: %6.3lf\n",
            ompSerE * analysis_flags->metric_factor);
    fprintf(of, "      OMP Transfer Efficiency:      %6.3lf\n",
            ompTE * analysis_flags->metric_factor);
    fprintf(of, "-------------------------------------------\n");
    if (openedFile) {
      fclose(of);
    }
  }
}

void SYNC_CLOCK::OmpHBefore() {
  if (!analysis_flags->running)
    return;
  assert(thread_local_clock->stopped_clock == true);
  assert(thread_local_clock->stopped_omp_clock == true);
  assert(thread_local_clock->stopped_mpi_clock == false);
  update_maximum(useful_computation_critical,
                 thread_local_clock->useful_computation_critical.load());
  update_maximum(useful_computation_proc,
                 thread_local_clock->useful_computation_proc.load());

  update_maximum(outsidempi_critical,
                 thread_local_clock->outsidempi_critical.load());
  update_maximum(outsideomp_critical,
                 thread_local_clock->outsideomp_critical.load());
  update_maximum(outsideomp_critical_nooffset,
                 thread_local_clock->outsideomp_critical_nooffset.load());
  update_maximum(outsideomp_proc, thread_local_clock->outsideomp_proc.load());
  update_maximum(outsidempi_proc, thread_local_clock->outsidempi_proc.load());
}

void SYNC_CLOCK::OmpHAfter() {
  if (!analysis_flags->running)
    return;
  assert(thread_local_clock->stopped_clock == true);
  assert(thread_local_clock->stopped_omp_clock == true);
  assert(thread_local_clock->stopped_mpi_clock == false);
  update_maximum((thread_local_clock->useful_computation_critical),
                 useful_computation_critical.load());
  update_maximum((thread_local_clock->useful_computation_proc),
                 useful_computation_proc.load());
  update_maximum((thread_local_clock->outsidempi_critical),
                 outsidempi_critical.load());
  update_maximum((thread_local_clock->outsideomp_critical),
                 outsideomp_critical.load());
  update_maximum((thread_local_clock->outsideomp_critical_nooffset),
                 outsideomp_critical_nooffset.load());
  update_maximum((thread_local_clock->outsideomp_proc), outsideomp_proc.load());
  update_maximum((thread_local_clock->outsidempi_proc), outsidempi_proc.load());
}

void OmpClockReset(THREAD_CLOCK *cv) {
  if (!cv || cv->openmp_thread)
    return;
  OmpClockReset(static_cast<SYNC_CLOCK *>(cv));
}

void OmpClockReset(SYNC_CLOCK *cv) {
  if (!analysis_flags->running)
    return;
  if (cv == nullptr)
    fprintf(stderr, "%s: unexpected NULL arg\n", __PRETTY_FUNCTION__);
  else {
    cv->useful_computation_thread = -1e50;
    cv->useful_computation_proc = -1e50;
    cv->useful_computation_critical = -1e50;
    cv->outsidempi_proc = -1e50;
    cv->outsidempi_thread = -1e50;
    cv->outsidempi_critical = -1e50;
    cv->outsideomp_thread = -1e50;
    cv->outsideomp_critical = -1e50;
    cv->outsideomp_critical_nooffset = -1e50;
    cv->outsideomp_proc = -1e50;
  }
}

void startTool(bool toolControl, ClockContext cc) {
  if (analysis_flags->stopped && !toolControl)
    return;
  if (!analysis_flags->running) {
    assert(thread_local_clock->stopped_clock == true);
    assert(thread_local_clock->stopped_omp_clock == true);
    assert(thread_local_clock->stopped_mpi_clock == true);

    assert(thread_local_clock->useful_computation_thread == 0);
    assert(thread_local_clock->useful_computation_proc == 0);
    assert(thread_local_clock->useful_computation_critical == 0);
    assert(thread_local_clock->outsidempi_proc == 0);
    assert(thread_local_clock->outsidempi_thread == 0);
    assert(thread_local_clock->outsidempi_critical == 0);
    assert(thread_local_clock->outsideomp_thread == 0);
    assert(thread_local_clock->outsideomp_critical == 0);
    assert(thread_local_clock->outsideomp_critical_nooffset == 0);
    assert(thread_local_clock->outsideomp_proc == 0);

#if 0 && defined(USE_MPI)
    if (useMpi) {
      if (analysis_flags->barrier) {
        PMPI_Barrier(MPI_COMM_WORLD);
      }
    }
#endif
    if (analysis_flags->verbose)
      fprintf(analysis_flags->output, "starting tool\n");
    double time = getTime();
    analysis_flags->running = true;
    startMeasurement(time);
    thread_local_clock->Start(-time, cc, __func__);
  }
}

void stopTool() {
#if 0 && defined(USE_MPI)
  if (useMpi) {
    if (analysis_flags->barrier) {
      PMPI_Barrier(MPI_COMM_WORLD);
    }
  }
#endif
  if (analysis_flags->running) {
    if (analysis_flags->verbose)
      fprintf(analysis_flags->output, "ending tool\n");
    double time = getTime();
    thread_local_clock->Stop(time, CLOCK_ALL, __func__);
    analysis_flags->running = false;
    stopMeasurement(time);
  }
  if (analysis_flags->dump_on_stop) {
    finishMeasurement();
  }
}

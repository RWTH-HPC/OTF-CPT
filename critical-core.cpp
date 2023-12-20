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
#ifdef USE_MPI
#include <mpi.h>
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

AnalysisFlags *analysis_flags = nullptr;

ompt_finalize_tool_t critical_ompt_finalize_tool{nullptr};

uint64_t my_next_id() {
  static uint64_t ID = 0;
  uint64_t ret = __sync_fetch_and_add(&ID, 1);
  return ret;
}

double totalProgrammTime = 0;
double startProgrammTime = getTime(), endProgrammTime;
double crit_path_useful_time = 0;

std::vector<THREAD_CLOCK *> *thread_clocks = nullptr;
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
    printf("Initialize after %lf s\n", startProgrammTime - initialStart);
}

void stopMeasurement(double time) { endProgrammTime = time; }

#define NUM_SHARED_METRICS 7

std::string colorize(std::string text) {
  if (!analysis_flags->colorize)
    return text;
  return "\033[1;35m" + text + "\033[0m";
}

void finishMeasurement() {
  int number_of_procs = 1;
  int total_threads = 0;
  int num_threads = 0;
  if (thread_clocks)
    total_threads = num_threads = thread_clocks->size();

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
    uc_avg[0] = uc_avg[0];

    uc_avg[2] = uc_avg[2];
  } else {
    num_threads = 1;
    uc_max[0] = uc_avg[0] =
        thread_local_clock->useful_computation_thread.load();
    uc_max[2] = uc_avg[2] = thread_local_clock->outsideomp_thread.load();
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
    PMPI_Reduce(&proc_counts, &total_counts, sizeof(proc_counts) / sizeof(int),
                MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
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
    avgComputation[0]/=num_threads;
    avgComputation[2]/=num_threads;
    avgComputation[5]/=num_threads;
    avgComputation[6]/=num_threads;
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

    if (analysis_flags->verbose) {
      std::string pop_string_total =
          "\n[pop] sere:" + std::to_string(SerE) + ":te:" + std::to_string(TE) +
          ":comme:" + std::to_string(CommE) + ":lb:" + std::to_string(LB) +
          ":pe:" + std::to_string(PE) +
          ":crittime:" + std::to_string(totalRuntimeIdeal) +
          ":totaltime:" + std::to_string(totalRuntimeReal) +
          ":avgcomputation:" + std::to_string(avgComputation[0]) +
          ":maxcomputation:" + std::to_string(maxComputation[0]) + "\n";
      std::cout << pop_string_total;
      std::cout << "\n\n--------MPI stats:--------\n";
      if (total_counts.send)
        std::cout << "MPI_*send: " << total_counts.send << std::endl;
      if (total_counts.isend)
        std::cout << "MPI_I*send: " << total_counts.isend << std::endl;
      if (total_counts.recv)
        std::cout << "MPI_Recv: " << total_counts.recv << std::endl;
      if (total_counts.irecv)
        std::cout << "MPI_Irecv: " << total_counts.irecv << std::endl;
      if (total_counts.coll)
        std::cout << "MPI_*coll: " << total_counts.coll << std::endl;
      if (total_counts.coll)
        std::cout << "MPI_I*coll: " << total_counts.coll << std::endl;
      if (total_counts.test)
        std::cout << "MPI_Test*: " << total_counts.test << std::endl;
      if (total_counts.wait)
        std::cout << "MPI_Wait*: " << total_counts.wait << std::endl
                  << std::endl;

      std::cout << "\n\n--------CritPath Analysis Tool results:--------\n";
      std::cout << "=> Number of processes:          " +
                       std::to_string(number_of_procs) + "\n";
      std::cout << "=> Number of threads:            " +
                       std::to_string(total_threads) + "\n";
      std::cout << "=> Average Computation (in s):   " +
                       std::to_string(avgComputation[0]) + "\n";
      std::cout << "=> Maximum Computation (in s):   " +
                       std::to_string(maxComputation[0]) + "\n";
      std::cout << "=> Max crit. computation (in s): " +
                       std::to_string(totalRuntimeIdeal) + "\n";
      std::cout << "=> Average crit. proc-local computation (in s):   " +
                       std::to_string(avgComputation[3]) + "\n";
      std::cout << "=> Maximum crit. proc-local computation (in s):   " +
                       std::to_string(maxComputation[3]) + "\n";
      std::cout << "=> Average crit. proc-local Outside OpenMP (in s):   " +
                       std::to_string(avgComputation[4]) + "\n";
      std::cout << "=> Maximum crit. proc-local Outside OpenMP (in s):   " +
                       std::to_string(maxComputation[4]) + "\n";
      std::cout << "=> Average proc-local rumtime (in s):   " +
                       std::to_string(avgComputation[5]) + "\n";
      std::cout << "=> Maximum proc-local runtime (in s):   " +
                       std::to_string(maxComputation[5]) + "\n";
      std::cout << "=> Average PL-max Computation (in s):   " +
                       std::to_string(avgComputation[6]) + "\n";
      std::cout << "=> Maximum PL-max Computation (in s):   " +
                       std::to_string(maxComputation[6]) + "\n";
      /*    std::cout << "=> Average Outside MPI (in s):   " +
                          std::to_string(avgComputation[1]) + "\n";
          std::cout << "=> Maximum Outside MPI (in s):   " +
                          std::to_string(maxComputation[1]) + "\n";
          std::cout << "=> Max crit. Outside MPI (in s): " +
                          std::to_string(totalOutsideMPIIdeal) + "\n";*/
      std::cout << "=> Average Outside OpenMP (in s):   " +
                       std::to_string(avgComputation[2]) + "\n";
      std::cout << "=> Maximum Outside OpenMP (in s):   " +
                       std::to_string(maxComputation[2]) + "\n";
      std::cout << "=> Max crit. Outside OpenMP (in s): " +
                       std::to_string(totalOutsideOMPIdeal) + "\n";
      std::cout << "=> Max crit. Outside OpenMP w/o offset (in s): " +
                       std::to_string(totalOutsideOMPIdealNoOffset) + "\n";
      std::cout << "=> Total runtime (in s):         " +
                       std::to_string(totalRuntimeReal) + "\n\n";
    }

    std::string pop_string =
        colorize("\n----------------POP metrics----------------\n");
    pop_string +=
        "Parallel Efficiency:                " + std::to_string(PE) + "\n";
    pop_string +=
        "  Load Balance:                     " + std::to_string(LB) + "\n";
    pop_string +=
        "  Communication Efficiency:         " + std::to_string(CommE) + "\n";
    pop_string +=
        "    Serialisation Efficiency:       " + std::to_string(SerE) + "\n";
    pop_string +=
        "    Transfer Efficiency:            " + std::to_string(TE) + "\n";
    pop_string +=
        "  MPI Parallel Efficiency:          " + std::to_string(mpiPE) + "\n";
    pop_string +=
        "    MPI Load Balance:               " + std::to_string(mpiLB) + "\n";
    pop_string +=
        "    MPI Communication Efficiency:   " + std::to_string(mpiCommE) +
        "\n";
    pop_string +=
        "      MPI Serialisation Efficiency: " + std::to_string(mpiSerE) + "\n";
    /*    pop_string +=
            "  MPI Parallel Efficiency2:          " + std::to_string(mpiPE2) +
       "\n"; pop_string += "    MPI Communication Efficiency2:   " +
       std::to_string(mpiCommE2) +
            "\n";
        pop_string +=
            "      MPI Serialisation Efficiency2: " + std::to_string(mpiSerE2) +
       "\n";*/
    pop_string +=
        "      MPI Transfer Efficiency:      " + std::to_string(mpiTE) + "\n";
    pop_string +=
        "  OMP Parallel Efficiency:          " + std::to_string(ompPE) + "\n";
    pop_string +=
        "    OMP Load Balance:               " + std::to_string(ompLB) + "\n";
    pop_string +=
        "    OMP Communication Efficiency:   " + std::to_string(ompCommE) +
        "\n";
    pop_string +=
        "      OMP Serialisation Efficiency: " + std::to_string(ompSerE) + "\n";
    /*    pop_string +=
            "  OMP Parallel Efficiency2:          " + std::to_string(ompPE2) +
       "\n"; pop_string += "    OMP Communication Efficiency2:   " +
       std::to_string(ompCommE2) +
            "\n";
        pop_string +=
            "      OMP Serialisation Efficiency2: " + std::to_string(ompSerE2) +
       "\n";*/
    pop_string +=
        "      OMP Transfer Efficiency:      " + std::to_string(ompTE) + "\n";
    pop_string += colorize("-------------------------------------------\n");
    std::cout << pop_string;
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
    std::cout << "NULL" << std::endl;
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

    if (analysis_flags->verbose)
      printf("starting tool\n");
    double time = getTime();
    analysis_flags->running = true;
    startMeasurement(time);
    thread_local_clock->Start(-time, cc, __func__);
  }
}

void stopTool() {
  if (analysis_flags->running) {
    if (analysis_flags->verbose)
      printf("ending tool\n");
    double time = getTime();
    thread_local_clock->Stop(time, CLOCK_ALL, __func__);
    analysis_flags->running = false;
    stopMeasurement(time);
  }
}

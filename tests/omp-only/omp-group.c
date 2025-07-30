// ALLOW_RETRIES: 1
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK2,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK4,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt_omp %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK2,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt_omp %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK4,CHECK %metricfile

// UNSUPPORTED: GNU

#include <omp.h>
#include <stdio.h>
#include <unistd.h>

#include "expected-metrics.h"

int main(int argc, char **argv) {
  int sum = 0, nt = omp_get_max_threads();
  // Expected distribution of work based on libomp tasking implementation:
  // (8+7+6+5+4+3+2+1), (8+7,1+2+3+4+5+6), (8+7,1+3+5,2+4+6), (8,1+4+7,2+5,3+6)
  // locally pop as LIFO, remote steal as FIFO
  int lbs[] = {1000, 18000 / 21, 12000 / 15, 9000 / 12};
  metrics m = {lbs[nt - 1], 1000, 1000, 1000, 1000, 1000};
  printMetrics(m);
  omp_control_tool(omp_control_tool_start, 0, NULL);
#pragma omp parallel
  {
#pragma omp master
#pragma omp taskgroup
#pragma omp taskgroup
    for (int i = 0; i < 8; i++) {
#pragma omp task
      {
        printf("Thread %i executing Task %i\n", omp_get_thread_num(), i);
        usleep(WORK * (1 + i));
      }
    }
  }
  omp_control_tool(omp_control_tool_end, 0, NULL);
  printf("sum = %i\n", sum);
}

// CHECK2: Parallel Efficiency:                [[PE:0.85[0-9]+]]
// CHECK4: Parallel Efficiency:                [[PE:0.7[45][0-9]+]]
// CHECK2:   Load Balance:                     [[LB:0.85[0-9]+]]
// CHECK4:   Load Balance:                     [[LB:0.7[45][0-9]+]]
// CHECK:   Communication Efficiency:         [[CE:(1.0|0.99)[0-9]+]]
// CHECK:     Serialisation Efficiency:       [[SE:(1.0|0.99)[0-9]+]]
// CHECK:     Transfer Efficiency:            [[TE:(1.0|0.99)[0-9]+]]
// CHECK:   MPI Parallel Efficiency:          1.000
// CHECK:     MPI Load Balance:               1.000
// CHECK:     MPI Communication Efficiency:   1.000
// CHECK:       MPI Serialisation Efficiency: 1.000
// CHECK:       MPI Transfer Efficiency:      1.000
// CHECK:   OMP Parallel Efficiency:          [[PE]]
// CHECK:     OMP Load Balance:               [[LB]]
// CHECK:     OMP Communication Efficiency:   [[CE]]
// CHECK:       OMP Serialisation Efficiency: [[SE]]
// CHECK:       OMP Transfer Efficiency:      [[TE]]

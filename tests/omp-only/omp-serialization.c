// ALLOW_RETRIES: 1
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK2,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK4,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt_omp %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK2,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt_omp %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK4,CHECK %metricfile

#include <omp.h>
#include <stdio.h>
#include <unistd.h>

#include "expected-metrics.h"

int main(int argc, char **argv) {
  int sum = 0, nt = omp_get_max_threads();
  metrics m = {1000, 1000 / nt, 1000, 1000, 1000, 1000};
  printMetrics(m);
  omp_control_tool(omp_control_tool_start, 0, NULL);
#pragma omp parallel for ordered schedule(static, 1)
  for (int i = 0; i < nt; i++) {
#pragma omp ordered
    {
      sum += i;
      usleep(2 * WORK);
    }
  }
  omp_control_tool(omp_control_tool_end, 0, NULL);
  printf("sum = %i\n", sum);
}

// CHECK4: Parallel Efficiency:                [[PE:0.2[45][0-9]+]]
// CHECK2: Parallel Efficiency:                [[PE:0.(49|50)[0-9]+]]
// CHECK:   Load Balance:                     [[LB:(1.0|0.99)[0-9]+]]
// CHECK4:   Communication Efficiency:         [[CE:0.25[0-9]+]]
// CHECK2:   Communication Efficiency:         [[CE:0.50[0-9]+]]
// CHECK4:     Serialisation Efficiency:       [[SE:0.25[0-9]+]]
// CHECK2:     Serialisation Efficiency:       [[SE:0.50[0-9]+]]
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

// clang-format off
// ALLOW_RETRIES: 1
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK2,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK4,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt_omp %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK2,CHECK %metricfile
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt_omp %otfcpt_options_dump_stopped \
// RUN: %t | %FileCheck --check-prefixes=CHECK4,CHECK %metricfile
// clang-format on

// UNSUPPORTED: GNU

#include <omp.h>
#include <stdio.h>
#include <unistd.h>

#include "expected-metrics.h"

int main(int argc, char **argv) {
  int sum = 0, nt = omp_get_max_threads();
  metrics m = {1000, 1000 / nt, 1000, 1000, 1000, 1000};
  printMetrics(m);
#pragma omp parallel
  {}
  omp_control_tool(omp_control_tool_start, 0, NULL);
#pragma omp parallel for ordered schedule(static, 1)
  for (int i = 0; i < nt; i++) {
#pragma omp ordered
    {
      sum += i;
      usleep(4 * WORK);
    }
  }
  omp_control_tool(omp_control_tool_end, 0, NULL);
  printf("sum = %i\n", sum);
}

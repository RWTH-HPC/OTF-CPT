// ALLOW_RETRIES: 1
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt %t | %FileCheck \
// RUN: --check-prefixes=CHECK %s
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt %t | %FileCheck \
// RUN: --check-prefixes=CHECK %s
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt_omp %t | %FileCheck \
// RUN: --check-prefixes=CHECK %s
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt_omp %t | %FileCheck \
// RUN: --check-prefixes=CHECK %s

#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int sum = 0;
#pragma omp parallel for schedule(static, 1) reduction(+:sum)
  for (int i = 0; i < 20; i++) {
    sum += i;
    usleep(100000);
  }

  printf("sum = %i\n", sum);
}

// CHECK: {{^}}Parallel Efficiency:                [[PE:(1.0|0.99)[0-9]+]]
// CHECK: {{^}}  Load Balance:                     [[LB:(1.0|0.99)[0-9]+]]
// CHECK: {{^}}  Communication Efficiency:         [[CE:(1.0|0.99)[0-9]+]]
// CHECK: {{^}}    Serialisation Efficiency:       [[SE:(1.0|0.99)[0-9]+]]
// CHECK: {{^}}    Transfer Efficiency:            [[TE:(1.0|0.99)[0-9]+]]
// CHECK: {{^}}  MPI Parallel Efficiency:          1.000
// CHECK: {{^}}    MPI Load Balance:               1.000
// CHECK: {{^}}    MPI Communication Efficiency:   1.000
// CHECK: {{^}}      MPI Serialisation Efficiency: 1.000
// CHECK: {{^}}      MPI Transfer Efficiency:      1.000
// CHECK: {{^}}  OMP Parallel Efficiency:          [[PE]]
// CHECK: {{^}}    OMP Load Balance:               [[LB]]
// CHECK: {{^}}    OMP Communication Efficiency:   [[CE]]
// CHECK: {{^}}      OMP Serialisation Efficiency: [[SE]]
// CHECK: {{^}}      OMP Transfer Efficiency:      [[TE]]

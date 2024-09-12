// ALLOW_RETRIES: 1
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt %t | %FileCheck \
// RUN: --check-prefixes=CHECK2,CHECK %s
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt %t | \
// RUN: %FileCheck --check-prefixes=CHECK4,CHECK %s
// RUN: env OMP_NUM_THREADS=2 %load_otfcpt_omp %t | \
// RUN: %FileCheck --check-prefixes=CHECK2,CHECK %s
// RUN: env OMP_NUM_THREADS=4 %load_otfcpt_omp %t | \
// RUN: %FileCheck --check-prefixes=CHECK4,CHECK %s

#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int sum = 0, nt = omp_get_max_threads();
#pragma omp parallel
  {
#pragma omp master
#pragma omp taskgroup
#pragma omp taskgroup
    for (int i = 0; i < 8; i++) {
#pragma omp task
      {
        printf("Thread %i executing Task %i\n", omp_get_thread_num(), i);
        usleep(100000 * (1 + i));
      }
    }
  }
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

//ALLOW_RETRIES: 5
//RUN: env OMP_NUM_THREADS=2 %load_otfcpt %t | %FileCheck --check-prefixes=CHECK2,CHECK %s
//RUN: env OMP_NUM_THREADS=4 %load_otfcpt %t | %FileCheck --check-prefixes=CHECK4,CHECK %s
//RUN: env OMP_NUM_THREADS=2 %load_otfcpt_omp %t | %FileCheck --check-prefixes=CHECK2,CHECK %s
//RUN: env OMP_NUM_THREADS=4 %load_otfcpt_omp %t | %FileCheck --check-prefixes=CHECK4,CHECK %s

#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int sum = 0, nt = omp_get_max_threads();
#pragma omp parallel 
{
    int tid = omp_get_thread_num();
#pragma omp for schedule(static, 1) reduction(+:sum)
  for (int i = 0; i < 100; i++) {
      sum += i;
      usleep(10000 * (1 + 2*tid));
  }
}
  printf("sum = %i\n", sum);
}

//CHECK2: Parallel Efficiency:                [[PE:0.6[67][0-9]+]]
//CHECK4: Parallel Efficiency:                [[PE:0.57[0-9]+]]
//CHECK2:   Load Balance:                     [[LB:0.6[67][0-9]+]]
//CHECK4:   Load Balance:                     [[LB:0.57[0-9]+]]
//CHECK:   Communication Efficiency:         [[CE:0.99[0-9]+]]
//CHECK:     Serialisation Efficiency:       [[SE:0.99[0-9]+]]
//CHECK:     Transfer Efficiency:            [[TE:0.99[0-9]+]]
//CHECK:   MPI Parallel Efficiency:          1.000000
//CHECK:     MPI Load Balance:               1.000000
//CHECK:     MPI Communication Efficiency:   1.000000
//CHECK:       MPI Serialisation Efficiency: 1.000000
//CHECK:       MPI Transfer Efficiency:      1.000000
//CHECK:   OMP Parallel Efficiency:          [[PE]]
//CHECK:     OMP Load Balance:               [[LB]]
//CHECK:     OMP Communication Efficiency:   [[CE]]
//CHECK:       OMP Serialisation Efficiency: [[SE]]
//CHECK:       OMP Transfer Efficiency:      [[TE]]

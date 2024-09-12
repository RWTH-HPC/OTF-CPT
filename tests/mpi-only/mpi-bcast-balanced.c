//ALLOW_RETRIES: 5
//RUN: env %load_otfcpt %mpirunnp 2 %t | %FileCheck --check-prefixes=CHECK %s
//RUN: env %load_otfcpt %mpirunnp 4 %t | %FileCheck --check-prefixes=CHECK %s

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int sum = 0, rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank > 0)
    usleep(100000);

  MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);


  if (rank == 0)
    usleep(100000);

  MPI_Finalize();
}

// BCast implementation might block, resulting in 50% TE!

// CHECK: Parallel Efficiency:                {{0.[49]9[0-9]+}}
// CHECK:   Load Balance:                     [[LB:0.99[0-9]+]]
// CHECK:   Communication Efficiency:         {{0.[49]9[0-9]+}}
// CHECK:     Serialisation Efficiency:       1.000
// CHECK:     Transfer Efficiency:            {{0.[49]9[0-9]+}}
// CHECK:   MPI Parallel Efficiency:          {{0.[49]9[0-9]+}}
// CHECK:     MPI Load Balance:               [[LB]]
// CHECK:     MPI Communication Efficiency:   {{0.[49]9[0-9]+}}
// CHECK:       MPI Serialisation Efficiency: 1.000
// CHECK:       MPI Transfer Efficiency:      {{0.[49]9[0-9]+}}
// CHECK:   OMP Parallel Efficiency:          {{(0.99[0-9]+|1.00[0-9]+)}}
// CHECK:     OMP Load Balance:               1.000
// CHECK:     OMP Communication Efficiency:   {{(0.99[0-9]+|1.00[0-9]+)}}
// CHECK:       OMP Serialisation Efficiency: 1.000
// CHECK:       OMP Transfer Efficiency:      {{(0.99[0-9]+|1.00[0-9]+)}}

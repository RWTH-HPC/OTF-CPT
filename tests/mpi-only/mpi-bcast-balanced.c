// ALLOW_RETRIES: 1
// RUN: %mpirunnp 2 %load_otfcpt %otfcpt_options_stopped %t | %FileCheck \
// RUN: --check-prefixes=CHECK %s
// RUN: %mpirunnp 4 %load_otfcpt %otfcpt_options_stopped %t | %FileCheck \
// RUN: --check-prefixes=CHECK %s

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int sum = 0, rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Pcontrol(1);
  if (rank > 0)
    usleep(100000);

  MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);


  if (rank == 0)
    usleep(100000);
  MPI_Pcontrol(0);

  MPI_Finalize();
}

// BCast implementation might block, resulting in 50% TE!

// CHECK: Parallel Efficiency:                {{(1.00|0.99|0.50|0.49)[0-9]+}}
// CHECK:   Load Balance:                     [[LB:(1.0|0.99)[0-9]+]]
// CHECK:   Communication Efficiency:         {{(1.00|0.99|0.50|0.49)[0-9]+}}
// CHECK:     Serialisation Efficiency:       1.000
// CHECK:     Transfer Efficiency:            {{(1.00|0.99|0.50|0.49)[0-9]+}}
// CHECK:   MPI Parallel Efficiency:          {{(1.00|0.99|0.50|0.49)[0-9]+}}
// CHECK:     MPI Load Balance:               [[LB]]
// CHECK:     MPI Communication Efficiency:   {{(1.00|0.99|0.50|0.49)[0-9]+}}
// CHECK:       MPI Serialisation Efficiency: 1.000
// CHECK:       MPI Transfer Efficiency:      {{(1.00|0.99|0.50|0.49)[0-9]+}}
// CHECK:   OMP Parallel Efficiency:          1.000
// CHECK:     OMP Load Balance:               1.000
// CHECK:     OMP Communication Efficiency:   1.000
// CHECK:       OMP Serialisation Efficiency: 1.000
// CHECK:       OMP Transfer Efficiency:      1.000

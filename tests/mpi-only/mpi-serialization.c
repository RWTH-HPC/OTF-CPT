//ALLOW_RETRIES: 5
//RUN: env %load_otfcpt %mpirunnp 2 %t | %FileCheck --check-prefixes=CHECK2,CHECK %s
//RUN: env %load_otfcpt %mpirunnp 4 %t | %FileCheck --check-prefixes=CHECK4,CHECK %s

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int sum = 0, rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank > 0)
    MPI_Recv(&sum, 1, MPI_INT, rank - 1, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  usleep(100000);

  if (rank < size - 1)
    MPI_Send(&sum, 1, MPI_INT, rank + 1, 42, MPI_COMM_WORLD);

  MPI_Finalize();
}

// CHECK4: Parallel Efficiency:                [[PE:0.24]]{{[0-9]+}}
// CHECK2: Parallel Efficiency:                [[PE:0.49]]{{[0-9]+}}
// CHECK:   Load Balance:                     [[LB:0.99[0-9]+]]
// CHECK4:   Communication Efficiency:         [[CE:0.24]]{{[0-9]+}}
// CHECK2:   Communication Efficiency:         [[CE:0.49]]{{[0-9]+}}
// CHECK4:     Serialisation Efficiency:       [[SE:0.25]]{{[0-9]+}}
// CHECK2:     Serialisation Efficiency:       [[SE:0.50]]{{[0-9]+}}
// CHECK:     Transfer Efficiency:            [[TE:0.99]]{{[0-9]+}}
// CHECK:   MPI Parallel Efficiency:          [[PE]]
// CHECK:     MPI Load Balance:               [[LB]]
// CHECK:     MPI Communication Efficiency:   [[CE]]
// CHECK:       MPI Serialisation Efficiency: [[SE]]
// CHECK:       MPI Transfer Efficiency:      [[TE]]
// CHECK:   OMP Parallel Efficiency:          {{(0.99[0-9]+|1.00[0-9]+)}}
// CHECK:     OMP Load Balance:               1.000
// CHECK:     OMP Communication Efficiency:   {{(0.99[0-9]+|1.00[0-9]+)}}
// CHECK:       OMP Serialisation Efficiency: 1.000
// CHECK:       OMP Transfer Efficiency:      {{(0.99[0-9]+|1.00[0-9]+)}}

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

  if (rank == 0)
    usleep(100000);

  MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);


  if (rank > 0)
    usleep(100000);

  MPI_Finalize();
}


//CHECK: Parallel Efficiency:                [[PE:0.49]]{{[0-9]+}}
//CHECK:   Load Balance:                     [[LB:0.99[0-9]+]]
//CHECK:   Communication Efficiency:         [[CE:0.49]]{{[0-9]+}}
//CHECK:     Serialisation Efficiency:       [[SE:0.50]]{{[0-9]+}}
//CHECK:     Transfer Efficiency:            [[TE:0.99]]{{[0-9]+}}
//CHECK:   MPI Parallel Efficiency:          [[PE]]
//CHECK:     MPI Load Balance:               [[LB]]
//CHECK:     MPI Communication Efficiency:   {{0.(49|50)[0-9]+}}
//CHECK:       MPI Serialisation Efficiency: [[SE]]
//CHECK:       MPI Transfer Efficiency:      [[TE]]
//CHECK:   OMP Parallel Efficiency:          {{(0.99[0-9]+|1.00[0-9]+)}}
//CHECK:     OMP Load Balance:               1.000000
//CHECK:     OMP Communication Efficiency:   {{(0.99[0-9]+|1.00[0-9]+)}}
//CHECK:       OMP Serialisation Efficiency: 1.000000
//CHECK:       OMP Transfer Efficiency:      {{(0.99[0-9]+|1.00[0-9]+)}}


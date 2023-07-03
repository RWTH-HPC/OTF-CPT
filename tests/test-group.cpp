#include <assert.h>
#include <mpi.h>
#include <stdio.h>

#ifndef NOTOOL
#include "tracking.h"
#endif

#define COUNT 400
int main() {
  int rank, size;
  MPI_Group g1[COUNT], g2[COUNT];
  int provided;
  MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);
  assert(provided == MPI_THREAD_MULTIPLE);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  for (int i = 0; i < COUNT; i++) {
    g1[i] = g2[i] = MPI_GROUP_NULL;
  }

  double time = MPI_Wtime();
  for (int it = 0; it < COUNT; it++) {
#pragma omp parallel
    {
#pragma omp for schedule(static) nowait
      for (int i = 0; i < COUNT; i++) {
        MPI_Comm_group(MPI_COMM_WORLD, g1 + i);
      }
#pragma omp for schedule(static)
      for (int i = 0; i < COUNT; i++) {
        MPI_Group_free(g1 + i);
      }
    }
  }
  printf("%i: comm-group %lf\n", rank, MPI_Wtime() - time);
  time = MPI_Wtime();
  for (int it = 0; it < COUNT; it++) {
#pragma omp parallel
    {
#pragma omp for schedule(static) nowait
      for (int i = 0; i < COUNT; i++) {
        PMPI_Comm_group(MPI_COMM_WORLD, g1 + i);
      }
#pragma omp for schedule(static)
      for (int i = 0; i < COUNT; i++) {
        PMPI_Group_free(g1 + i);
      }
    }
  }
  printf("%i: comm-group-base %lf\n", rank, MPI_Wtime() - time);

  MPI_Finalize();
}

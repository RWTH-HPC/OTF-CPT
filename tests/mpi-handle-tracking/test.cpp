#include <assert.h>
#include <mpi.h>
#include <omp.h>
#include <stdio.h>

#ifndef NOTOOL
#include "tracking.h"
#endif

#include "timing.h"

#define COUNT 200
int main() {
  int rank, size, rbuf[COUNT], total = 0;
  MPI_Request allreq[2 * COUNT], *rreq = allreq, *sreq = allreq + COUNT;
  int provided;
  int required = MPI_THREAD_MULTIPLE;
  if (omp_get_max_threads() == 1)
    required = MPI_THREAD_SINGLE;
  MPI_Init_thread(NULL, NULL, required, &provided);
  assert(provided == required);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  for (int i = 0; i < COUNT; i++) {
    rreq[i] = sreq[i] = MPI_REQUEST_NULL;
  }

  /*******************
         wait
  *******************/

  double time = MPI_Wtime();
  {
    RaiiTimer timer{rank, "p2p-wait"};
    for (int it = 0; it < COUNT; it++) {
      auto iTimer = timer.iterTimer();
#pragma omp parallel
      {
#pragma omp for schedule(static) nowait
        for (int i = 0; i < COUNT; i++) {
          MPI_Irecv(rbuf + i, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                    rreq + i);
          MPI_Isend(&rank, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                    sreq + i);
        }
#pragma omp for schedule(static) reduction(+ : total)
        for (int i = 0; i < COUNT; i++) {
          MPI_Wait(rreq + i, MPI_STATUS_IGNORE);
          MPI_Wait(sreq + i, MPI_STATUS_IGNORE);
          total += rbuf[i];
        }
      }
    }
  }
  // printf("%i: p2p-wait %i, %lf\n", rank, total, MPI_Wtime() - time);
  time = MPI_Wtime();
  {
    RaiiTimer timer{rank, "p2p-wait-notool"};
    for (int it = 0; it < COUNT; it++) {
      auto iTimer = timer.iterTimer();
#pragma omp parallel
      {
#pragma omp for schedule(static) nowait
        for (int i = 0; i < COUNT; i++) {
          PMPI_Irecv(rbuf + i, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                     rreq + i);
          PMPI_Isend(&rank, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                     sreq + i);
        }
#pragma omp for schedule(static) reduction(+ : total)
        for (int i = 0; i < COUNT; i++) {
          PMPI_Wait(rreq + i, MPI_STATUS_IGNORE);
          PMPI_Wait(sreq + i, MPI_STATUS_IGNORE);
          total += rbuf[i];
        }
      }
    }
  }
  // printf("%i: p2p-wait-notool %i, %lf\n", rank, total, MPI_Wtime() - time);
  /*******************
        waitall
  *******************/
  time = MPI_Wtime();
  {
    RaiiTimer timer{rank, "p2p-waitall"};
    for (int it = 0; it < COUNT; it++) {
      auto iTimer = timer.iterTimer();
#pragma omp parallel for
      for (int i = 0; i < COUNT; i++) {
        MPI_Irecv(rbuf + i, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                  rreq + i);
        MPI_Isend(&rank, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                  sreq + i);
      }
      MPI_Waitall(2 * COUNT, allreq, MPI_STATUSES_IGNORE);
      for (int i = 0; i < COUNT; i++) {
        total += rbuf[i];
      }
    }
  }
  // printf("%i: p2p-waitall %i, %lf\n", rank, total, MPI_Wtime() - time);
  time = MPI_Wtime();
  {
    RaiiTimer timer{rank, "p2p-waitall-notool"};
    for (int it = 0; it < COUNT; it++) {
      auto iTimer = timer.iterTimer();
#pragma omp parallel for
      for (int i = 0; i < COUNT; i++) {
        PMPI_Irecv(rbuf + i, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                   rreq + i);
        PMPI_Isend(&rank, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                   sreq + i);
      }
      PMPI_Waitall(2 * COUNT, allreq, MPI_STATUSES_IGNORE);
      for (int i = 0; i < COUNT; i++) {
        total += rbuf[i];
      }
    }
  }
  // printf("%i: p2p-waitall-notool %i, %lf\n", rank, total, MPI_Wtime() -
  // time);

  /*******************
        waitsome
  *******************/
  time = MPI_Wtime();
  {
    RaiiTimer timer{rank, "p2p-waitsome"};
    for (int it = 0; it < COUNT; it++) {
      auto iTimer = timer.iterTimer();
#pragma omp parallel for
      for (int i = 0; i < COUNT; i++) {
        MPI_Irecv(rbuf + i, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                  rreq + i);
        MPI_Isend(&rank, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                  sreq + i);
      }
      int ri[COUNT], si[COUNT];
      for (int sc = 0, rc = 0, is = 0;
           is < COUNT && (sc < COUNT || rc < COUNT);) {
        int soutcount = 0, routcount = 0;
        if (rc < COUNT)
          MPI_Waitsome(COUNT, rreq, &routcount, ri, MPI_STATUSES_IGNORE);
        if (sc < COUNT)
          MPI_Waitsome(COUNT, sreq, &soutcount, si, MPI_STATUSES_IGNORE);
        for (int i = 0; i < routcount; i++)
          total += rbuf[ri[i]];
        if (soutcount != MPI_UNDEFINED)
          sc += soutcount;
        if (routcount != MPI_UNDEFINED)
          rc += routcount;
        is++;
      }
    }
  }
  // printf("%i: p2p-waitsome %i, %lf\n", rank, total, MPI_Wtime() - time);
  for (int i = 0; i < COUNT; i++) {
    rreq[i] = sreq[i] = MPI_REQUEST_NULL;
  }
  time = MPI_Wtime();
  {
    RaiiTimer timer{rank, "p2p-waitsome-notool"};
    for (int it = 0; it < COUNT; it++) {
      auto iTimer = timer.iterTimer();
#pragma omp parallel for
      for (int i = 0; i < COUNT; i++) {
        PMPI_Irecv(rbuf + i, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                   rreq + i);
        PMPI_Isend(&rank, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                   sreq + i);
      }
      int ri[COUNT], si[COUNT];
      for (int sc = 0, rc = 0, is = 0;
           is < COUNT && (sc < COUNT || rc < COUNT);) {
        int soutcount = 0, routcount = 0;
        if (rc < COUNT)
          PMPI_Waitsome(COUNT, rreq, &routcount, ri, MPI_STATUSES_IGNORE);
        if (sc < COUNT)
          PMPI_Waitsome(COUNT, sreq, &soutcount, si, MPI_STATUSES_IGNORE);
        for (int i = 0; i < routcount; i++)
          total += rbuf[ri[i]];
        if (soutcount != MPI_UNDEFINED)
          sc += soutcount;
        if (routcount != MPI_UNDEFINED)
          rc += routcount;
        is++;
      }
    }
  }
  // printf("%i: p2p-waitsome-notool %i, %lf\n", rank, total, MPI_Wtime() -
  // time);

  /*******************
        waitany
  *******************/
  time = MPI_Wtime();
  {
    RaiiTimer timer{rank, "p2p-waitany"};
    for (int it = 0; it < COUNT; it++) {
      auto iTimer = timer.iterTimer();
#pragma omp parallel for
      for (int i = 0; i < COUNT; i++) {
        MPI_Irecv(rbuf + i, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                  rreq + i);
        MPI_Isend(&rank, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                  sreq + i);
      }
      for (int i = 0; i < COUNT; i++) {
        int ri, si;
        MPI_Waitany(COUNT, rreq, &ri, MPI_STATUS_IGNORE);
        MPI_Waitany(COUNT, sreq, &si, MPI_STATUS_IGNORE);
        total += rbuf[ri];
      }
    }
  }
  // printf("%i: p2p-waitany %i, %lf\n", rank, total, MPI_Wtime() - time);
  time = MPI_Wtime();
  {
    RaiiTimer timer{rank, "p2p-waitany-notool"};
    for (int it = 0; it < COUNT; it++) {
      auto iTimer = timer.iterTimer();
#pragma omp parallel for
      for (int i = 0; i < COUNT; i++) {
        PMPI_Irecv(rbuf + i, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                   rreq + i);
        PMPI_Isend(&rank, 1, MPI_INT, size - rank - 1, 42, MPI_COMM_WORLD,
                   sreq + i);
      }
      for (int i = 0; i < COUNT; i++) {
        int ri, si;
        PMPI_Waitany(COUNT, rreq, &ri, MPI_STATUS_IGNORE);
        PMPI_Waitany(COUNT, sreq, &si, MPI_STATUS_IGNORE);
        total += rbuf[ri];
      }
    }
  }
  // printf("%i: p2p-waitany-notool %i, %lf\n", rank, total, MPI_Wtime() -
  // time);

  MPI_Finalize();
}

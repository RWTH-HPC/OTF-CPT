! ALLOW_RETRIES: 1
! RUN: %mpirunnp 2 %load_otfcpt %otfcpt_options_dump_stopped %t | %FileCheck \
! RUN: --check-prefixes=CHECK2,CHECK %metricfile
! RUN: %mpirunnp 4 %load_otfcpt %otfcpt_options_dump_stopped %t | %FileCheck \
! RUN: --check-prefixes=CHECK4,CHECK %metricfile
! REQUIRES: Fortran

program hello
   use mpi
   use M_time
   interface
      subroutine printMetrics(ulb, use, ute, mlb, mse, mte)
         integer,intent(in) :: ulb, use, ute, mlb, mse, mte
      end subroutine printMetrics
   end interface

   integer rank, size, ierror
   
   call MPI_INIT(ierror)
   call MPI_COMM_SIZE(MPI_COMM_WORLD, size, ierror)
   call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierror)
   if (rank.eq.0) then
     call printMetrics(1000, 1000, 1000, 1000 * size / (2 * size - 1), 1000, 1000);
   end if
   call MPI_BARRIER(MPI_COMM_WORLD, ierror)
   call MPI_PCONTROL(1)
   call usleep(WORK * (1 + 2 * rank))
   call MPI_PCONTROL(0)
   call MPI_BARRIER(MPI_COMM_WORLD, ierror)
   
   call MPI_FINALIZE(ierror)
   
end program

! CHECK2: Parallel Efficiency:                [[PE:0.6[67]]]{{[0-9]+}}
! CHECK4: Parallel Efficiency:                [[PE:0.57]]{{[0-9]+}}
! CHECK2:   Load Balance:                     [[LB:0.6[67][0-9]+]]
! CHECK4:   Load Balance:                     [[LB:0.57[0-9]+]]
! CHECK:   Communication Efficiency:         [[CE:(1.0|0.99)]]{{[0-9]+}}
! CHECK:     Serialisation Efficiency:       1.000
! CHECK:     Transfer Efficiency:            [[TE:(1.0|0.99)]]{{[0-9]+}}
! CHECK:   MPI Parallel Efficiency:          [[PE]]
! CHECK:     MPI Load Balance:               [[LB]]
! CHECK:     MPI Communication Efficiency:   [[CE]]
! CHECK:       MPI Serialisation Efficiency: 1.000
! CHECK:       MPI Transfer Efficiency:      [[TE]]
! CHECK:   OMP Parallel Efficiency:          {{(0.99[0-9]+|1.00[0-9]+)}}
! CHECK:     OMP Load Balance:               1.000
! CHECK:     OMP Communication Efficiency:   {{(0.99[0-9]+|1.00[0-9]+)}}
! CHECK:       OMP Serialisation Efficiency: 1.000
! CHECK:       OMP Transfer Efficiency:      {{(0.99[0-9]+|1.00[0-9]+)}}


#include <mpi.h>
#include <stdlib.h>

#define _EXTERN_C_

/* =============== Fortran Wrappers for MPI_Start =============== */
static void MPI_Start_fortran_wrapper(MPI_Fint *request, MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Start((MPI_Request *)request);
#else  /* MPI-2 safe call */
  MPI_Request temp_request;
  temp_request = MPI_Request_f2c(*request);
  _wrap_py_return_val = MPI_Start(&temp_request);
  *request = MPI_Request_c2f(temp_request);
#endif /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_START(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Start_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_start(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Start_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_start_(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Start_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_start__(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Start_fortran_wrapper(request, ierr);
}

/* ================= End Wrappers for MPI_Start ================= */

/* =============== Fortran Wrappers for MPI_Startall =============== */
static void MPI_Startall_fortran_wrapper(MPI_Fint *count,
                                         MPI_Fint array_of_requests[],
                                         MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Startall(*count, (MPI_Request *)array_of_requests);
#else  /* MPI-2 safe call */
  int i;
  MPI_Request *temp_array_of_requests;
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  _wrap_py_return_val = MPI_Startall(*count, temp_array_of_requests);
  for (i = 0; i < *count; i++)
    array_of_requests[i] = MPI_Request_c2f(temp_array_of_requests[i]);
  if (temp_array_of_requests)
    free(temp_array_of_requests);
#endif /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_STARTALL(MPI_Fint *count, MPI_Fint array_of_requests[],
                             MPI_Fint *ierr) {
  MPI_Startall_fortran_wrapper(count, array_of_requests, ierr);
}

_EXTERN_C_ void mpi_startall(MPI_Fint *count, MPI_Fint array_of_requests[],
                             MPI_Fint *ierr) {
  MPI_Startall_fortran_wrapper(count, array_of_requests, ierr);
}

_EXTERN_C_ void mpi_startall_(MPI_Fint *count, MPI_Fint array_of_requests[],
                              MPI_Fint *ierr) {
  MPI_Startall_fortran_wrapper(count, array_of_requests, ierr);
}

_EXTERN_C_ void mpi_startall__(MPI_Fint *count, MPI_Fint array_of_requests[],
                               MPI_Fint *ierr) {
  MPI_Startall_fortran_wrapper(count, array_of_requests, ierr);
}

/* ================= End Wrappers for MPI_Startall ================= */

/* =============== Fortran Wrappers for MPI_Request_free =============== */
static void MPI_Request_free_fortran_wrapper(MPI_Fint *request,
                                             MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Request_free((MPI_Request *)request);
#else  /* MPI-2 safe call */
  MPI_Request temp_request;
  temp_request = MPI_Request_f2c(*request);
  _wrap_py_return_val = MPI_Request_free(&temp_request);
  *request = MPI_Request_c2f(temp_request);
#endif /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_REQUEST_FREE(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Request_free_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_request_free(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Request_free_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_request_free_(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Request_free_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_request_free__(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Request_free_fortran_wrapper(request, ierr);
}

/* ================= End Wrappers for MPI_Request_free ================= */

/* =============== Fortran Wrappers for MPI_Cancel =============== */
static void MPI_Cancel_fortran_wrapper(MPI_Fint *request, MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Cancel((MPI_Request *)request);
#else  /* MPI-2 safe call */
  MPI_Request temp_request;
  temp_request = MPI_Request_f2c(*request);
  _wrap_py_return_val = MPI_Cancel(&temp_request);
  *request = MPI_Request_c2f(temp_request);
#endif /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_CANCEL(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Cancel_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_cancel(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Cancel_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_cancel_(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Cancel_fortran_wrapper(request, ierr);
}

_EXTERN_C_ void mpi_cancel__(MPI_Fint *request, MPI_Fint *ierr) {
  MPI_Cancel_fortran_wrapper(request, ierr);
}

/* ================= End Wrappers for MPI_Cancel ================= */

/* =============== Fortran Wrappers for MPI_Wait =============== */
static void MPI_Wait_fortran_wrapper(MPI_Fint *request, MPI_Fint *status,
                                     MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Wait((MPI_Request *)request, (MPI_Status *)status);
#else                                        /* MPI-2 safe call */
  MPI_Request temp_request;
  MPI_Status temp_status;
#if defined(MPICH_NAME) && (MPICH_NAME == 1) /* MPICH test */
  temp_request = MPI_Request_f2c(*request);
  MPI_Status_f2c(status, &temp_status);
  _wrap_py_return_val = MPI_Wait(&temp_request, &temp_status);
  *request = MPI_Request_c2f(temp_request);
  MPI_Status_c2f(&temp_status, status);
#else                                        /* MPI-2 safe call */
  temp_request = MPI_Request_f2c(*request);
  if (status != MPI_F_STATUS_IGNORE)
    MPI_Status_f2c(status, &temp_status);
  _wrap_py_return_val = MPI_Wait(
      &temp_request,
      ((status == MPI_F_STATUS_IGNORE) ? MPI_STATUS_IGNORE : &temp_status));
  *request = MPI_Request_c2f(temp_request);
  if (status != MPI_F_STATUS_IGNORE)
    MPI_Status_c2f(&temp_status, status);
#endif                                       /* MPICH test */
#endif                                       /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_WAIT(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr) {
  MPI_Wait_fortran_wrapper(request, status, ierr);
}

_EXTERN_C_ void mpi_wait(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr) {
  MPI_Wait_fortran_wrapper(request, status, ierr);
}

_EXTERN_C_ void mpi_wait_(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr) {
  MPI_Wait_fortran_wrapper(request, status, ierr);
}

_EXTERN_C_ void mpi_wait__(MPI_Fint *request, MPI_Fint *status,
                           MPI_Fint *ierr) {
  MPI_Wait_fortran_wrapper(request, status, ierr);
}

/* ================= End Wrappers for MPI_Wait ================= */

/* =============== Fortran Wrappers for MPI_Waitall =============== */
static void MPI_Waitall_fortran_wrapper(MPI_Fint *count,
                                        MPI_Fint array_of_requests[],
                                        MPI_Fint *array_of_statuses,
                                        MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Waitall(*count, (MPI_Request *)array_of_requests,
                                    (MPI_Status *)array_of_statuses);
#else                                        /* MPI-2 safe call */
  int i;
  MPI_Request *temp_array_of_requests;
  MPI_Status *temp_array_of_statuses;
#if defined(MPICH_NAME) && (MPICH_NAME == 1) /* MPICH test */
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  temp_array_of_statuses = (MPI_Status *)malloc(sizeof(MPI_Status) * *count);
  if (!temp_array_of_statuses)
    abort();
  _wrap_py_return_val =
      MPI_Waitall(*count, temp_array_of_requests, temp_array_of_statuses);
  for (i = 0; i < *count; i++)
    MPI_Status_c2f(&temp_array_of_statuses[i],
                   &array_of_statuses[i * MPI_F_STATUS_SIZE]);
#else                                        /* MPI-2 safe call */
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  temp_array_of_statuses = (MPI_Status *)malloc(sizeof(MPI_Status) * *count);
  if (!temp_array_of_statuses)
    abort();
  _wrap_py_return_val = MPI_Waitall(
      *count, temp_array_of_requests,
      ((array_of_statuses == MPI_F_STATUSES_IGNORE) ? MPI_STATUSES_IGNORE
                                                    : temp_array_of_statuses));
  if (array_of_statuses != MPI_F_STATUSES_IGNORE)
    for (i = 0; i < *count; i++)
      MPI_Status_c2f(&temp_array_of_statuses[i],
                     &array_of_statuses[i * MPI_F_STATUS_SIZE]);
#endif                                       /* MPICH test */
  if (temp_array_of_statuses)
    free(temp_array_of_statuses);
  if (temp_array_of_requests)
    free(temp_array_of_requests);
#endif                                       /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_WAITALL(MPI_Fint *count, MPI_Fint array_of_requests[],
                            MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Waitall_fortran_wrapper(count, array_of_requests, array_of_statuses,
                              ierr);
}

_EXTERN_C_ void mpi_waitall(MPI_Fint *count, MPI_Fint array_of_requests[],
                            MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Waitall_fortran_wrapper(count, array_of_requests, array_of_statuses,
                              ierr);
}

_EXTERN_C_ void mpi_waitall_(MPI_Fint *count, MPI_Fint array_of_requests[],
                             MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Waitall_fortran_wrapper(count, array_of_requests, array_of_statuses,
                              ierr);
}

_EXTERN_C_ void mpi_waitall__(MPI_Fint *count, MPI_Fint array_of_requests[],
                              MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Waitall_fortran_wrapper(count, array_of_requests, array_of_statuses,
                              ierr);
}

/* ================= End Wrappers for MPI_Waitall ================= */

/* =============== Fortran Wrappers for MPI_Waitany =============== */
static void MPI_Waitany_fortran_wrapper(MPI_Fint *count,
                                        MPI_Fint array_of_requests[],
                                        MPI_Fint *indx, MPI_Fint *status,
                                        MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Waitany(*count, (MPI_Request *)array_of_requests,
                                    indx, (MPI_Status *)status);
#else                                        /* MPI-2 safe call */
  int i;
  MPI_Status temp_status;
  MPI_Request *temp_array_of_requests;
#if defined(MPICH_NAME) && (MPICH_NAME == 1) /* MPICH test */
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  MPI_Status_f2c(status, &temp_status);
  _wrap_py_return_val =
      MPI_Waitany(*count, temp_array_of_requests, indx, &temp_status);
  if (*indx != MPI_UNDEFINED) {
    ++(*indx);
    MPI_Status_c2f(&temp_status, status);
  }
#else                                        /* MPI-2 safe call */
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  if (status != MPI_F_STATUS_IGNORE)
    MPI_Status_f2c(status, &temp_status);
  _wrap_py_return_val = MPI_Waitany(
      *count, temp_array_of_requests, indx,
      ((status == MPI_F_STATUS_IGNORE) ? MPI_STATUS_IGNORE : &temp_status));
  if (*indx != MPI_UNDEFINED) {
    ++(*indx);
    if (status != MPI_F_STATUS_IGNORE)
      MPI_Status_c2f(&temp_status, status);
  }
#endif                                       /* MPICH test */
  if (temp_array_of_requests)
    free(temp_array_of_requests);
#endif                                       /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_WAITANY(MPI_Fint *count, MPI_Fint array_of_requests[],
                            MPI_Fint *indx, MPI_Fint *status, MPI_Fint *ierr) {
  MPI_Waitany_fortran_wrapper(count, array_of_requests, indx, status, ierr);
}

_EXTERN_C_ void mpi_waitany(MPI_Fint *count, MPI_Fint array_of_requests[],
                            MPI_Fint *indx, MPI_Fint *status, MPI_Fint *ierr) {
  MPI_Waitany_fortran_wrapper(count, array_of_requests, indx, status, ierr);
}

_EXTERN_C_ void mpi_waitany_(MPI_Fint *count, MPI_Fint array_of_requests[],
                             MPI_Fint *indx, MPI_Fint *status, MPI_Fint *ierr) {
  MPI_Waitany_fortran_wrapper(count, array_of_requests, indx, status, ierr);
}

_EXTERN_C_ void mpi_waitany__(MPI_Fint *count, MPI_Fint array_of_requests[],
                              MPI_Fint *indx, MPI_Fint *status,
                              MPI_Fint *ierr) {
  MPI_Waitany_fortran_wrapper(count, array_of_requests, indx, status, ierr);
}

/* ================= End Wrappers for MPI_Waitany ================= */

/* =============== Fortran Wrappers for MPI_Waitsome =============== */
static void
MPI_Waitsome_fortran_wrapper(MPI_Fint *incount, MPI_Fint array_of_requests[],
                             MPI_Fint *outcount, MPI_Fint array_of_indices[],
                             MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Waitsome(*incount, (MPI_Request *)array_of_requests,
                                     ((int *)outcount), array_of_indices,
                                     (MPI_Status *)array_of_statuses);
#else                                        /* MPI-2 safe call */
  int i;
  MPI_Request *temp_array_of_requests;
  MPI_Status *temp_array_of_statuses;
#if defined(MPICH_NAME) && (MPICH_NAME == 1) /* MPICH test */
  temp_array_of_requests =
      (MPI_Request *)malloc(sizeof(MPI_Request) * *incount);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *incount; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  temp_array_of_statuses = (MPI_Status *)malloc(sizeof(MPI_Status) * *incount);
  if (!temp_array_of_statuses)
    abort();
  _wrap_py_return_val =
      MPI_Waitsome(*incount, temp_array_of_requests, ((int *)outcount),
                   array_of_indices, temp_array_of_statuses);
  if (*outcount != MPI_UNDEFINED) {
    for (i = 0; i < *outcount; ++i)
      ++array_of_indices[i];
    for (i = 0; i < *incount; i++)
      MPI_Status_c2f(&temp_array_of_statuses[i],
                     &array_of_statuses[i * MPI_F_STATUS_SIZE]);
  }
#else                                        /* MPI-2 safe call */
  temp_array_of_requests =
      (MPI_Request *)malloc(sizeof(MPI_Request) * *incount);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *incount; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  temp_array_of_statuses = (MPI_Status *)malloc(sizeof(MPI_Status) * *incount);
  if (!temp_array_of_statuses)
    abort();
  _wrap_py_return_val = MPI_Waitsome(
      *incount, temp_array_of_requests, ((int *)outcount), array_of_indices,
      ((array_of_statuses == MPI_F_STATUSES_IGNORE) ? MPI_STATUSES_IGNORE
                                                    : temp_array_of_statuses));
  if (*outcount != MPI_UNDEFINED) {
    for (i = 0; i < *outcount; ++i)
      ++array_of_indices[i];
    if (array_of_statuses != MPI_F_STATUSES_IGNORE)
      for (i = 0; i < *incount; i++)
        MPI_Status_c2f(&temp_array_of_statuses[i],
                       &array_of_statuses[i * MPI_F_STATUS_SIZE]);
  }
#endif                                       /* MPICH test */
  if (temp_array_of_statuses)
    free(temp_array_of_statuses);
  if (temp_array_of_requests)
    free(temp_array_of_requests);
#endif                                       /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_WAITSOME(MPI_Fint *incount, MPI_Fint array_of_requests[],
                             MPI_Fint *outcount, MPI_Fint array_of_indices[],
                             MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Waitsome_fortran_wrapper(incount, array_of_requests, outcount,
                               array_of_indices, array_of_statuses, ierr);
}

_EXTERN_C_ void mpi_waitsome(MPI_Fint *incount, MPI_Fint array_of_requests[],
                             MPI_Fint *outcount, MPI_Fint array_of_indices[],
                             MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Waitsome_fortran_wrapper(incount, array_of_requests, outcount,
                               array_of_indices, array_of_statuses, ierr);
}

_EXTERN_C_ void mpi_waitsome_(MPI_Fint *incount, MPI_Fint array_of_requests[],
                              MPI_Fint *outcount, MPI_Fint array_of_indices[],
                              MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Waitsome_fortran_wrapper(incount, array_of_requests, outcount,
                               array_of_indices, array_of_statuses, ierr);
}

_EXTERN_C_ void mpi_waitsome__(MPI_Fint *incount, MPI_Fint array_of_requests[],
                               MPI_Fint *outcount, MPI_Fint array_of_indices[],
                               MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Waitsome_fortran_wrapper(incount, array_of_requests, outcount,
                               array_of_indices, array_of_statuses, ierr);
}

/* ================= End Wrappers for MPI_Waitsome ================= */

/* =============== Fortran Wrappers for MPI_Test =============== */
static void MPI_Test_fortran_wrapper(MPI_Fint *request, MPI_Fint *flag,
                                     MPI_Fint *status, MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val =
      MPI_Test((MPI_Request *)request, ((int *)flag), (MPI_Status *)status);
#else                                        /* MPI-2 safe call */
  MPI_Request temp_request;
  MPI_Status temp_status;
#if defined(MPICH_NAME) && (MPICH_NAME == 1) /* MPICH test */
  temp_request = MPI_Request_f2c(*request);
  MPI_Status_f2c(status, &temp_status);
  _wrap_py_return_val = MPI_Test(&temp_request, ((int *)flag), &temp_status);
  *request = MPI_Request_c2f(temp_request);
  MPI_Status_c2f(&temp_status, status);
#else                                        /* MPI-2 safe call */
  temp_request = MPI_Request_f2c(*request);
  if (status != MPI_F_STATUS_IGNORE)
    MPI_Status_f2c(status, &temp_status);
  _wrap_py_return_val = MPI_Test(
      &temp_request, ((int *)flag),
      ((status == MPI_F_STATUS_IGNORE) ? MPI_STATUS_IGNORE : &temp_status));
  *request = MPI_Request_c2f(temp_request);
  if (status != MPI_F_STATUS_IGNORE)
    MPI_Status_c2f(&temp_status, status);
#endif                                       /* MPICH test */
#endif                                       /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_TEST(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status,
                         MPI_Fint *ierr) {
  MPI_Test_fortran_wrapper(request, flag, status, ierr);
}

_EXTERN_C_ void mpi_test(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status,
                         MPI_Fint *ierr) {
  MPI_Test_fortran_wrapper(request, flag, status, ierr);
}

_EXTERN_C_ void mpi_test_(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status,
                          MPI_Fint *ierr) {
  MPI_Test_fortran_wrapper(request, flag, status, ierr);
}

_EXTERN_C_ void mpi_test__(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status,
                           MPI_Fint *ierr) {
  MPI_Test_fortran_wrapper(request, flag, status, ierr);
}

/* ================= End Wrappers for MPI_Test ================= */

/* =============== Fortran Wrappers for MPI_Testall =============== */
static void MPI_Testall_fortran_wrapper(MPI_Fint *count,
                                        MPI_Fint array_of_requests[],
                                        MPI_Fint *flag,
                                        MPI_Fint *array_of_statuses,
                                        MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val =
      MPI_Testall(*count, (MPI_Request *)array_of_requests, ((int *)flag),
                  (MPI_Status *)array_of_statuses);
#else                                        /* MPI-2 safe call */
  int i;
  MPI_Request *temp_array_of_requests;
  MPI_Status *temp_array_of_statuses;
#if defined(MPICH_NAME) && (MPICH_NAME == 1) /* MPICH test */
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  temp_array_of_statuses = (MPI_Status *)malloc(sizeof(MPI_Status) * *count);
  if (!temp_array_of_statuses)
    abort();
  _wrap_py_return_val = MPI_Testall(*count, temp_array_of_requests,
                                    ((int *)flag), temp_array_of_statuses);
  for (i = 0; i < *count; i++)
    MPI_Status_c2f(&temp_array_of_statuses[i],
                   &array_of_statuses[i * MPI_F_STATUS_SIZE]);
#else                                        /* MPI-2 safe call */
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  temp_array_of_statuses = (MPI_Status *)malloc(sizeof(MPI_Status) * *count);
  if (!temp_array_of_statuses)
    abort();
  _wrap_py_return_val = MPI_Testall(
      *count, temp_array_of_requests, ((int *)flag),
      ((array_of_statuses == MPI_F_STATUSES_IGNORE) ? MPI_STATUSES_IGNORE
                                                    : temp_array_of_statuses));
  if (array_of_statuses != MPI_F_STATUSES_IGNORE)
    for (i = 0; i < *count; i++)
      MPI_Status_c2f(&temp_array_of_statuses[i],
                     &array_of_statuses[i * MPI_F_STATUS_SIZE]);
#endif                                       /* MPICH test */
  if (temp_array_of_statuses)
    free(temp_array_of_statuses);
  if (temp_array_of_requests)
    free(temp_array_of_requests);
#endif                                       /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_TESTALL(MPI_Fint *count, MPI_Fint array_of_requests[],
                            MPI_Fint *flag, MPI_Fint *array_of_statuses,
                            MPI_Fint *ierr) {
  MPI_Testall_fortran_wrapper(count, array_of_requests, flag, array_of_statuses,
                              ierr);
}

_EXTERN_C_ void mpi_testall(MPI_Fint *count, MPI_Fint array_of_requests[],
                            MPI_Fint *flag, MPI_Fint *array_of_statuses,
                            MPI_Fint *ierr) {
  MPI_Testall_fortran_wrapper(count, array_of_requests, flag, array_of_statuses,
                              ierr);
}

_EXTERN_C_ void mpi_testall_(MPI_Fint *count, MPI_Fint array_of_requests[],
                             MPI_Fint *flag, MPI_Fint *array_of_statuses,
                             MPI_Fint *ierr) {
  MPI_Testall_fortran_wrapper(count, array_of_requests, flag, array_of_statuses,
                              ierr);
}

_EXTERN_C_ void mpi_testall__(MPI_Fint *count, MPI_Fint array_of_requests[],
                              MPI_Fint *flag, MPI_Fint *array_of_statuses,
                              MPI_Fint *ierr) {
  MPI_Testall_fortran_wrapper(count, array_of_requests, flag, array_of_statuses,
                              ierr);
}

/* ================= End Wrappers for MPI_Testall ================= */

/* =============== Fortran Wrappers for MPI_Testany =============== */
static void MPI_Testany_fortran_wrapper(MPI_Fint *count,
                                        MPI_Fint array_of_requests[],
                                        MPI_Fint *indx, MPI_Fint *flag,
                                        MPI_Fint *status, MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Testany(*count, (MPI_Request *)array_of_requests,
                                    indx, ((int *)flag), (MPI_Status *)status);
#else                                        /* MPI-2 safe call */
  int i;
  MPI_Status temp_status;
  MPI_Request *temp_array_of_requests;
#if defined(MPICH_NAME) && (MPICH_NAME == 1) /* MPICH test */
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  MPI_Status_f2c(status, &temp_status);
  _wrap_py_return_val = MPI_Testany(*count, temp_array_of_requests, indx,
                                    ((int *)flag), &temp_status);
  if (*indx != MPI_UNDEFINED) {
    ++(*indx);
    MPI_Status_c2f(&temp_status, status);
  }
#else                                        /* MPI-2 safe call */
  temp_array_of_requests = (MPI_Request *)malloc(sizeof(MPI_Request) * *count);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *count; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  if (status != MPI_F_STATUS_IGNORE)
    MPI_Status_f2c(status, &temp_status);
  _wrap_py_return_val = MPI_Testany(
      *count, temp_array_of_requests, indx, ((int *)flag),
      ((status == MPI_F_STATUS_IGNORE) ? MPI_STATUS_IGNORE : &temp_status));
  if (*indx != MPI_UNDEFINED) {
    ++(*indx);
    if (status != MPI_F_STATUS_IGNORE)
      MPI_Status_c2f(&temp_status, status);
  }
#endif                                       /* MPICH test */
  if (temp_array_of_requests)
    free(temp_array_of_requests);
#endif                                       /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_TESTANY(MPI_Fint *count, MPI_Fint array_of_requests[],
                            MPI_Fint *indx, MPI_Fint *flag, MPI_Fint *status,
                            MPI_Fint *ierr) {
  MPI_Testany_fortran_wrapper(count, array_of_requests, indx, flag, status,
                              ierr);
}

_EXTERN_C_ void mpi_testany(MPI_Fint *count, MPI_Fint array_of_requests[],
                            MPI_Fint *indx, MPI_Fint *flag, MPI_Fint *status,
                            MPI_Fint *ierr) {
  MPI_Testany_fortran_wrapper(count, array_of_requests, indx, flag, status,
                              ierr);
}

_EXTERN_C_ void mpi_testany_(MPI_Fint *count, MPI_Fint array_of_requests[],
                             MPI_Fint *indx, MPI_Fint *flag, MPI_Fint *status,
                             MPI_Fint *ierr) {
  MPI_Testany_fortran_wrapper(count, array_of_requests, indx, flag, status,
                              ierr);
}

_EXTERN_C_ void mpi_testany__(MPI_Fint *count, MPI_Fint array_of_requests[],
                              MPI_Fint *indx, MPI_Fint *flag, MPI_Fint *status,
                              MPI_Fint *ierr) {
  MPI_Testany_fortran_wrapper(count, array_of_requests, indx, flag, status,
                              ierr);
}

/* ================= End Wrappers for MPI_Testany ================= */

/* =============== Fortran Wrappers for MPI_Testsome =============== */
static void
MPI_Testsome_fortran_wrapper(MPI_Fint *incount, MPI_Fint array_of_requests[],
                             MPI_Fint *outcount, MPI_Fint array_of_indices[],
                             MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Testsome(*incount, (MPI_Request *)array_of_requests,
                                     ((int *)outcount), array_of_indices,
                                     (MPI_Status *)array_of_statuses);
#else                                        /* MPI-2 safe call */
  int i;
  MPI_Request *temp_array_of_requests;
  MPI_Status *temp_array_of_statuses;
#if defined(MPICH_NAME) && (MPICH_NAME == 1) /* MPICH test */
  temp_array_of_requests =
      (MPI_Request *)malloc(sizeof(MPI_Request) * *incount);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *incount; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  temp_array_of_statuses = (MPI_Status *)malloc(sizeof(MPI_Status) * *incount);
  if (!temp_array_of_statuses)
    abort();
  _wrap_py_return_val =
      MPI_Testsome(*incount, temp_array_of_requests, ((int *)outcount),
                   array_of_indices, temp_array_of_statuses);
  if (*outcount != MPI_UNDEFINED) {
    for (i = 0; i < *outcount; ++i)
      ++array_of_indices[i];
    for (i = 0; i < *incount; i++)
      MPI_Status_c2f(&temp_array_of_statuses[i],
                     &array_of_statuses[i * MPI_F_STATUS_SIZE]);
  }
#else                                        /* MPI-2 safe call */
  temp_array_of_requests =
      (MPI_Request *)malloc(sizeof(MPI_Request) * *incount);
  if (!temp_array_of_requests)
    abort();
  for (i = 0; i < *incount; i++)
    temp_array_of_requests[i] = MPI_Request_f2c(array_of_requests[i]);
  temp_array_of_statuses = (MPI_Status *)malloc(sizeof(MPI_Status) * *incount);
  if (!temp_array_of_statuses)
    abort();
  _wrap_py_return_val = MPI_Testsome(
      *incount, temp_array_of_requests, ((int *)outcount), array_of_indices,
      ((array_of_statuses == MPI_F_STATUSES_IGNORE) ? MPI_STATUSES_IGNORE
                                                    : temp_array_of_statuses));
  if (*outcount != MPI_UNDEFINED) {
    for (i = 0; i < *outcount; ++i)
      ++array_of_indices[i];
    if (array_of_statuses != MPI_F_STATUSES_IGNORE)
      for (i = 0; i < *incount; i++)
        MPI_Status_c2f(&temp_array_of_statuses[i],
                       &array_of_statuses[i * MPI_F_STATUS_SIZE]);
  }
#endif                                       /* MPICH test */
  if (temp_array_of_statuses)
    free(temp_array_of_statuses);
  if (temp_array_of_requests)
    free(temp_array_of_requests);
#endif                                       /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_TESTSOME(MPI_Fint *incount, MPI_Fint array_of_requests[],
                             MPI_Fint *outcount, MPI_Fint array_of_indices[],
                             MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Testsome_fortran_wrapper(incount, array_of_requests, outcount,
                               array_of_indices, array_of_statuses, ierr);
}

_EXTERN_C_ void mpi_testsome(MPI_Fint *incount, MPI_Fint array_of_requests[],
                             MPI_Fint *outcount, MPI_Fint array_of_indices[],
                             MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Testsome_fortran_wrapper(incount, array_of_requests, outcount,
                               array_of_indices, array_of_statuses, ierr);
}

_EXTERN_C_ void mpi_testsome_(MPI_Fint *incount, MPI_Fint array_of_requests[],
                              MPI_Fint *outcount, MPI_Fint array_of_indices[],
                              MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Testsome_fortran_wrapper(incount, array_of_requests, outcount,
                               array_of_indices, array_of_statuses, ierr);
}

_EXTERN_C_ void mpi_testsome__(MPI_Fint *incount, MPI_Fint array_of_requests[],
                               MPI_Fint *outcount, MPI_Fint array_of_indices[],
                               MPI_Fint *array_of_statuses, MPI_Fint *ierr) {
  MPI_Testsome_fortran_wrapper(incount, array_of_requests, outcount,
                               array_of_indices, array_of_statuses, ierr);
}

/* ================= End Wrappers for MPI_Testsome ================= */

/* =============== Fortran Wrappers for MPI_Win_free =============== */
static void MPI_Win_free_fortran_wrapper(MPI_Fint *win, MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Win_free((MPI_Win *)win);
#else  /* MPI-2 safe call */
  MPI_Win temp_win;
  temp_win = MPI_Win_f2c(*win);
  _wrap_py_return_val = MPI_Win_free(&temp_win);
  *win = MPI_Win_c2f(temp_win);
#endif /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_WIN_FREE(MPI_Fint *win, MPI_Fint *ierr) {
  MPI_Win_free_fortran_wrapper(win, ierr);
}

_EXTERN_C_ void mpi_win_free(MPI_Fint *win, MPI_Fint *ierr) {
  MPI_Win_free_fortran_wrapper(win, ierr);
}

_EXTERN_C_ void mpi_win_free_(MPI_Fint *win, MPI_Fint *ierr) {
  MPI_Win_free_fortran_wrapper(win, ierr);
}

_EXTERN_C_ void mpi_win_free__(MPI_Fint *win, MPI_Fint *ierr) {
  MPI_Win_free_fortran_wrapper(win, ierr);
}

/* ================= End Wrappers for MPI_Win_free ================= */

/* =============== Fortran Wrappers for MPI_Comm_free =============== */
static void MPI_Comm_free_fortran_wrapper(MPI_Fint *comm, MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Comm_free((MPI_Comm *)comm);
#else  /* MPI-2 safe call */
  MPI_Comm temp_comm;
  temp_comm = MPI_Comm_f2c(*comm);
  _wrap_py_return_val = MPI_Comm_free(&temp_comm);
  *comm = MPI_Comm_c2f(temp_comm);
#endif /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_COMM_FREE(MPI_Fint *comm, MPI_Fint *ierr) {
  MPI_Comm_free_fortran_wrapper(comm, ierr);
}

_EXTERN_C_ void mpi_comm_free(MPI_Fint *comm, MPI_Fint *ierr) {
  MPI_Comm_free_fortran_wrapper(comm, ierr);
}

_EXTERN_C_ void mpi_comm_free_(MPI_Fint *comm, MPI_Fint *ierr) {
  MPI_Comm_free_fortran_wrapper(comm, ierr);
}

_EXTERN_C_ void mpi_comm_free__(MPI_Fint *comm, MPI_Fint *ierr) {
  MPI_Comm_free_fortran_wrapper(comm, ierr);
}

/* ================= End Wrappers for MPI_Comm_free ================= */

/* =============== Fortran Wrappers for MPI_Comm_disconnect =============== */
static void MPI_Comm_disconnect_fortran_wrapper(MPI_Fint *comm,
                                                MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Comm_disconnect((MPI_Comm *)comm);
#else  /* MPI-2 safe call */
  MPI_Comm temp_comm;
  _wrap_py_return_val = MPI_Comm_disconnect(&temp_comm);
  *comm = MPI_Comm_c2f(temp_comm);
#endif /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_COMM_DISCONNECT(MPI_Fint *comm, MPI_Fint *ierr) {
  MPI_Comm_disconnect_fortran_wrapper(comm, ierr);
}

_EXTERN_C_ void mpi_comm_disconnect(MPI_Fint *comm, MPI_Fint *ierr) {
  MPI_Comm_disconnect_fortran_wrapper(comm, ierr);
}

_EXTERN_C_ void mpi_comm_disconnect_(MPI_Fint *comm, MPI_Fint *ierr) {
  MPI_Comm_disconnect_fortran_wrapper(comm, ierr);
}

_EXTERN_C_ void mpi_comm_disconnect__(MPI_Fint *comm, MPI_Fint *ierr) {
  MPI_Comm_disconnect_fortran_wrapper(comm, ierr);
}

/* ================= End Wrappers for MPI_Comm_disconnect ================= */

#ifdef HAVE_SESSION
/* =============== Fortran Wrappers for MPI_Session_finalize =============== */
static void MPI_Session_finalize_fortran_wrapper(MPI_Fint *session,
                                                 MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
#if (!defined(MPICH_HAS_C2F) && defined(MPICH_NAME) &&                         \
     (MPICH_NAME == 1)) /* MPICH test */
  _wrap_py_return_val = MPI_Session_finalize((MPI_Session *)session);
#else  /* MPI-2 safe call */
  MPI_Session temp_session;
  temp_session = MPI_Session_f2c(*session);
  _wrap_py_return_val = MPI_Session_finalize(&temp_session);
  *session = MPI_Session_c2f(temp_session);
#endif /* MPICH test */
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_SESSION_FINALIZE(MPI_Fint *session, MPI_Fint *ierr) {
  MPI_Session_finalize_fortran_wrapper(session, ierr);
}

_EXTERN_C_ void mpi_session_finalize(MPI_Fint *session, MPI_Fint *ierr) {
  MPI_Session_finalize_fortran_wrapper(session, ierr);
}

_EXTERN_C_ void mpi_session_finalize_(MPI_Fint *session, MPI_Fint *ierr) {
  MPI_Session_finalize_fortran_wrapper(session, ierr);
}

_EXTERN_C_ void mpi_session_finalize__(MPI_Fint *session, MPI_Fint *ierr) {
  MPI_Session_finalize_fortran_wrapper(session, ierr);
}
#endif
/* ================= End Wrappers for MPI_Session_finalize ================= */

/* =============== Fortran Wrappers for MPI_Finalize =============== */
static void MPI_Finalize_fortran_wrapper(MPI_Fint *ierr) {
  int _wrap_py_return_val = 0;
  _wrap_py_return_val = MPI_Finalize();
  *ierr = _wrap_py_return_val;
}

_EXTERN_C_ void MPI_FINALIZE(MPI_Fint *ierr) {
  MPI_Finalize_fortran_wrapper(ierr);
}

_EXTERN_C_ void mpi_finalize(MPI_Fint *ierr) {
  MPI_Finalize_fortran_wrapper(ierr);
}

_EXTERN_C_ void mpi_finalize_(MPI_Fint *ierr) {
  MPI_Finalize_fortran_wrapper(ierr);
}

_EXTERN_C_ void mpi_finalize__(MPI_Fint *ierr) {
  MPI_Finalize_fortran_wrapper(ierr);
}

/* ================= End Wrappers for MPI_Finalize ================= */

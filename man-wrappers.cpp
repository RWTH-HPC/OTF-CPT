#include <assert.h>
#include <mpi.h>
#include <stdio.h>

#include "mpi-critical.h"
#include "tracking.h"

////////////////////
// FREE functions
////////////////////

/* ================== C Wrappers for MPI_Win_free ================== */
int PMPI_Win_free(MPI_Win *win);
int MPI_Win_free(MPI_Win *win) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

#ifdef HANDLE_WIN
  MPI_Win myWin = wf.getHandle(*win);
  *win = wf.freeHandle(*win);
  MPI_Win *myWinP = &myWin;
#else
  MPI_Win *myWinP = win;
#endif
  _wrap_py_return_val = PMPI_Win_free(myWinP);

  return _wrap_py_return_val;
}

/* ================== C Wrappers for MPI_Comm_free ================== */
int PMPI_Comm_free(MPI_Comm *comm);
int MPI_Comm_free(MPI_Comm *comm) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

#ifdef HANDLE_COMM
  MPI_Comm myComm = cf.getHandle(*comm);
  *comm = cf.freeHandle(*comm);
  MPI_Comm *myCommP = &myComm;
#else
  MPI_Comm *myCommP = comm;
#endif
  _wrap_py_return_val = PMPI_Comm_free(myCommP);

  return _wrap_py_return_val;
}

/* ================== C Wrappers for MPI_Comm_disconnect ================== */
int PMPI_Comm_disconnect(MPI_Comm *comm);
int MPI_Comm_disconnect(MPI_Comm *comm) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

#ifdef HANDLE_COMM
  MPI_Comm myComm = cf.getHandle(*comm);
  *comm = cf.freeHandle(*comm);
  MPI_Comm *myCommP = &myComm;
#else
  MPI_Comm *myCommP = comm;
#endif
  _wrap_py_return_val = PMPI_Comm_disconnect(myCommP);

  return _wrap_py_return_val;
}

#ifdef HAVE_SESSION
/* ================== C Wrappers for MPI_Session_finalize ================== */
int PMPI_Session_finalize(MPI_Session *session);
int MPI_Session_finalize(MPI_Session *session) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

#ifdef HANDLE_SESSION
  MPI_Session mySession = sf.getHandle(*session);
  *session = sf.freeHandle(*session);
  MPI_Session *mySessionP = &mySession;
#else
  MPI_Session *mySessionP = session;
#endif
  _wrap_py_return_val = PMPI_Session_finalize(mySessionP);

  return _wrap_py_return_val;
}
#endif

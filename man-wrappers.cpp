#ifndef NOTOOL

#include <assert.h>
#include <mpi.h>
#include <stdio.h>

#include "mpi-critical.h"
#include "tracking.h"

/* ================== C Wrappers for MPI_Type_create_struct ==================
 */
int PMPI_Type_create_struct(int count, const int array_of_blocklengths[],
                            const MPI_Aint array_of_displacements[],
                            const MPI_Datatype array_of_types[],
                            MPI_Datatype *newtype);
int MPI_Type_create_struct(int count, const int array_of_blocklengths[],
                           const MPI_Aint array_of_displacements[],
                           const MPI_Datatype array_of_types[],
                           MPI_Datatype *newtype) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;
#ifdef HANDLE_TYPE
  MPI_Datatype myTypes[count];
  for (int i = 0; i < count; i++) {
    myTypes[i] = tf.getHandle(array_of_types[i]);
  }
#else
  const MPI_Datatype *myTypes = array_of_types;
#endif

  _wrap_py_return_val = PMPI_Type_create_struct(
      count, array_of_blocklengths, array_of_displacements, myTypes, newtype);
  postType(newtype);

  return _wrap_py_return_val;
}

/* ================== C Wrappers for MPI_Type_get_contents ================== */
int PMPI_Type_get_contents(MPI_Datatype datatype, int max_integers,
                           int max_addresses, int max_datatypes,
                           int array_of_integers[],
                           MPI_Aint array_of_addresses[],
                           MPI_Datatype array_of_datatypes[]);
int MPI_Type_get_contents(MPI_Datatype datatype, int max_integers,
                          int max_addresses, int max_datatypes,
                          int array_of_integers[],
                          MPI_Aint array_of_addresses[],
                          MPI_Datatype array_of_datatypes[]) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

  preType(datatype);
#ifdef HANDLE_TYPE
  MPI_Datatype myTypes[max_datatypes] = {MPI_DATATYPE_NULL};
#else
  MPI_Datatype *myTypes = array_of_datatypes;
#endif

  _wrap_py_return_val = PMPI_Type_get_contents(
      datatype, max_integers, max_addresses, max_datatypes, array_of_integers,
      array_of_addresses, myTypes);
#ifdef HANDLE_TYPE
  for (int i = 0; i < max_datatypes; i++) {
    array_of_datatypes[i] = tf.newHandle(myTypes[i]);
  }
#endif

  return _wrap_py_return_val;
}

#if MPI_VERSION < 3
/* ================== C Wrappers for MPI_Type_struct ================== */
int PMPI_Type_struct(int count, int array_of_blocklengths[],
                     MPI_Aint array_of_displacements[],
                     MPI_Datatype array_of_types[], MPI_Datatype *newtype);
int MPI_Type_struct(int count, int array_of_blocklengths[],
                    MPI_Aint array_of_displacements[],
                    MPI_Datatype array_of_types[], MPI_Datatype *newtype) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;
#ifdef HANDLE_TYPE
  MPI_Datatype myTypes[count];
  for (int i = 0; i < count; i++) {
    myTypes[i] = tf.getHandle(array_of_types[i]);
  }
#else
  MPI_Datatype *myTypes = array_of_types;
#endif

  _wrap_py_return_val = PMPI_Type_struct(
      count, array_of_blocklengths, array_of_displacements, myTypes, newtype);
  postType(newtype);

  return _wrap_py_return_val;
}
#endif


#ifdef HAVE_COUNT
/* ================== C Wrappers for MPI_Type_create_struct_c ==================
 */
int PMPI_Type_create_struct_c(MPI_Count count,
                              const MPI_Count array_of_blocklengths[],
                              const MPI_Count array_of_displacements[],
                              const MPI_Datatype array_of_types[],
                              MPI_Datatype *newtype);
int MPI_Type_create_struct_c(MPI_Count count,
                             const MPI_Count array_of_blocklengths[],
                             const MPI_Count array_of_displacements[],
                             const MPI_Datatype array_of_types[],
                             MPI_Datatype *newtype) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;
#ifdef HANDLE_TYPE
  MPI_Datatype myTypes[count];
  for (int i = 0; i < count; i++) {
    myTypes[i] = tf.getHandle(array_of_types[i]);
  }
#else
  const MPI_Datatype *myTypes = array_of_types;
#endif

  _wrap_py_return_val = PMPI_Type_create_struct_c(
      count, array_of_blocklengths, array_of_displacements, myTypes, newtype);
  postType(newtype);

  return _wrap_py_return_val;
}

/* ================== C Wrappers for MPI_Type_get_contents_c ==================
 */
int PMPI_Type_get_contents_c(MPI_Datatype datatype, MPI_Count max_integers,
                             MPI_Count max_addresses,
                             MPI_Count max_large_counts,
                             MPI_Count max_datatypes, int array_of_integers[],
                             MPI_Aint array_of_addresses[],
                             MPI_Count array_of_large_counts[],
                             MPI_Datatype array_of_datatypes[]);
int MPI_Type_get_contents_c(MPI_Datatype datatype, MPI_Count max_integers,
                            MPI_Count max_addresses, MPI_Count max_large_counts,
                            MPI_Count max_datatypes, int array_of_integers[],
                            MPI_Aint array_of_addresses[],
                            MPI_Count array_of_large_counts[],
                            MPI_Datatype array_of_datatypes[]) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

  preType(datatype);
#ifdef HANDLE_TYPE
  MPI_Datatype myTypes[max_datatypes] = {MPI_DATATYPE_NULL};
#else
  MPI_Datatype *myTypes = array_of_datatypes;
#endif

  _wrap_py_return_val = PMPI_Type_get_contents_c(
      datatype, max_integers, max_addresses, max_large_counts, max_datatypes,
      array_of_integers, array_of_addresses, array_of_large_counts, myTypes);
#ifdef HANDLE_TYPE
  for (int i = 0; i < max_datatypes; i++) {
    array_of_datatypes[i] = tf.newHandle(myTypes[i]);
  }
#endif

  return _wrap_py_return_val;
}
#endif

////////////////////
// FREE functions
////////////////////

/* ================== C Wrappers for MPI_Group_free ================== */
int PMPI_Group_free(MPI_Group *group);
int MPI_Group_free(MPI_Group *group) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

#ifdef HANDLE_GROUP
  MPI_Group myGrp = gf.getHandle(*group);
  *group = gf.freeHandle(*group);
  MPI_Group *myGrpP = &myGrp;
#else
  MPI_Group *myGrpP = group;
#endif
  _wrap_py_return_val = PMPI_Group_free(myGrpP);

  return _wrap_py_return_val;
}

/* ================== C Wrappers for MPI_Op_free ================== */
int PMPI_Op_free(MPI_Op *op);
int MPI_Op_free(MPI_Op *op) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

#ifdef HANDLE_OP
  MPI_Op myOp = of.getHandle(*op);
  *op = of.freeHandle(*op);
  MPI_Op *myOpP = &myOp;
#else
  MPI_Op *myOpP = op;
#endif
  _wrap_py_return_val = PMPI_Op_free(myOpP);

  return _wrap_py_return_val;
}

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

/* ================== C Wrappers for MPI_Type_free ================== */
int PMPI_Type_free(MPI_Datatype *datatype);
int MPI_Type_free(MPI_Datatype *datatype) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

#ifdef HANDLE_TYPE
  MPI_Datatype myType = tf.getHandle(*datatype);
  *datatype = tf.freeHandle(*datatype);
  MPI_Datatype *myTypeP = &myType;
#else
  MPI_Datatype *myTypeP = datatype;
#endif
  _wrap_py_return_val = PMPI_Type_free(myTypeP);

  return _wrap_py_return_val;
}

/* ================== C Wrappers for MPI_Type_commit ================== */
int PMPI_Type_commit(MPI_Datatype *datatype);
int MPI_Type_commit(MPI_Datatype *datatype) {
  mpiTimer mt{false, __func__};
  int _wrap_py_return_val = 0;

  preType(*datatype);
  _wrap_py_return_val = PMPI_Type_commit(datatype);
  postType(datatype);

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

#endif

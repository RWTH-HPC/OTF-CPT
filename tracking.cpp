#include "tracking.h"
#ifdef USE_MPI
#include "mpi.h"
#endif

#ifdef USE_ERRHANDLER
#include <execinfo.h>
#include <sys/types.h>
#include <unistd.h>

#if !__cpp_exceptions
namespace std {
void __throw_system_error(int) { abort(); }
void __throw_bad_function_call() { abort(); }
} // namespace std
#endif

#ifdef USE_MPI
static MPI_Errhandler CommErrHandler{MPI_ERRHANDLER_NULL};

static void myMpiErrHandler(MPI_Comm *comm, int *errCode, ...) {
  printf("Handling MPI Error %i at pid %i\n", *errCode, getpid());
  sleep(10);
  disable_signalhandlers();
  abort();
}
void createErrHandler() {
  if (!analysis_flags->stacktrace)
    return;
  PMPI_Comm_create_errhandler(myMpiErrHandler, &CommErrHandler);
}
void registerErrHandler(MPI_Comm comm) {
  if (!analysis_flags->stacktrace)
    return;
  PMPI_Comm_set_errhandler(comm, CommErrHandler);
}
#endif

#ifdef HANDLE_WIN
template <> MPI_Win WinData::nullHandle{MPI_WIN_NULL};
WinFactory wf;
#endif
#ifdef HANDLE_FILE
template <> MPI_File FileData::nullHandle{MPI_FILE_NULL};
FileFactory ff;
#endif
#ifdef HANDLE_COMM
template <>
bool AbstractHandleFactory<MPI_Comm, CommData, toolCommData>::isPredefined(
    MPI_Comm handle) {
  return handle == MPI_COMM_NULL || handle == MPI_COMM_WORLD ||
         handle == MPI_COMM_SELF;
}
template <>
void AbstractHandleFactory<MPI_Comm, CommData, toolCommData>::initPredefined() {
  predefHandles[MPI_COMM_NULL].init(MPI_COMM_NULL);
  predefHandles[MPI_COMM_WORLD].init(MPI_COMM_WORLD);
  predefHandles[MPI_COMM_SELF].init(MPI_COMM_SELF);
}
MPI_Comm CommData::nullHandle{MPI_COMM_NULL};
CommFactory cf;
#endif
#ifdef HANDLE_MESSAGE
template <>
bool AbstractHandleFactory<MPI_Message, MessageData,
                           toolMessageData>::isPredefined(MPI_Message handle) {
  return handle == MPI_MESSAGE_NULL || handle == MPI_MESSAGE_NO_PROC;
}
template <>
void AbstractHandleFactory<MPI_Message, MessageData,
                           toolMessageData>::initPredefined() {}
MessageFactory mf;
#endif
#ifdef HANDLE_REQUEST
MPI_Request RequestData::nullHandle{MPI_REQUEST_NULL};
RequestFactory rf;
#endif
#if defined(HAVE_SESSION) && defined(HANDLE_SESSION)
template <> MPI_Session SessionData::nullHandle{MPI_SESSION_NULL};
SessionFactory sf;
#endif

int ipcData::num_uc_double{NUM_UC_DOUBLE};
int ipcData::num_uc_int64{NUM_UC_INT64};
MPI_Datatype ipcData::ipcMpiType{MPI_DATATYPE_NULL};

void ipcData::initIpcData() {
  if (num_uc_int64 > 0) {
    ipcData tempData{};
    RequestData tempRData{};
    MPI_Aint displs[2], rdispls[2];
    PMPI_Get_address(tempData.uc_double, displs);
    PMPI_Get_address(tempData.uc_int64, displs + 1);
    PMPI_Get_address(tempRData.uc_double, rdispls);
    PMPI_Get_address(tempRData.uc_int64, rdispls + 1);
    displs[1] -= displs[0];
    displs[0] = 0;
    rdispls[1] -= rdispls[0];
    rdispls[0] = 0;
    assert(displs[1] == rdispls[1]);
    MPI_Datatype types[] = {MPI_DOUBLE, MPI_INT64_T};
    int blengths[] = {num_uc_double, num_uc_int64};
    PMPI_Type_create_struct(2, blengths, displs, types, &ipcMpiType);
  } else {
    PMPI_Type_contiguous(num_uc_double, MPI_DOUBLE, &ipcMpiType);
  }
  PMPI_Type_commit(&ipcMpiType);
}
void ipcData::finiIpcData() {
  if (ipcMpiType != MPI_DATATYPE_NULL)
    PMPI_Type_free(&ipcMpiType);
}
#endif // USE_MPI

#include "tracking.h"
#include "mpi.h"

#ifdef USE_ERRHANDLER
#include <execinfo.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#define CALLSTACK_SIZE 20
static void print_stack(void) {
  int nptrs;
  void *buf[CALLSTACK_SIZE + 1];
  nptrs = backtrace(buf, CALLSTACK_SIZE);
  backtrace_symbols_fd(buf, nptrs, STDOUT_FILENO);
}

static void set_signalhandlers(sighandler_t handler) {
  signal(SIGSEGV, handler);
  signal(SIGINT, handler);
  signal(SIGHUP, handler);
  signal(SIGABRT, handler);
  signal(SIGTERM, handler);
  signal(SIGUSR2, handler);
  signal(SIGQUIT, handler);
  signal(SIGALRM, handler);
}

static void disable_signalhandlers() { set_signalhandlers(SIG_DFL); }

void mySignalHandler(int signum) {
  disable_signalhandlers();
  printf("pid %i caught signal nr %i\n", getpid(), signum);
  if (signum == SIGINT || signum == SIGKILL) {
    print_stack();
    _exit(signum + 128);
  }
  if (signum == SIGTERM || signum == SIGUSR2) {
    print_stack();
    fflush(stdout);
    sleep(1);
    _exit(signum + 128);
  }
  print_stack();

  printf("Waiting up to %i seconds to attach with a debugger.\n", 20);
  sleep(20);
  _exit(1);
}

void init_signalhandlers() {
  if (!analysis_flags->stacktrace)
    return;
  if (analysis_flags->verbose)
    fprintf(analysis_flags->output, "Setting signal handlers\n");
  set_signalhandlers(mySignalHandler);
}

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

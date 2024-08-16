#define USE_ERRHANDLER 1

#ifdef USE_ERRHANDLER

#ifdef USE_MPI
#include <mpi.h>
#define errHandlerComm(c) registerErrHandler(*c)
void createErrHandler();
void registerErrHandler(MPI_Comm comm);
#endif

void init_signalhandlers();
#else
#define errHandlerComm(c) (void)c
#endif

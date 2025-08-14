// mpicc -E mpi_implementation_version.c | grep -v '^#' | clang-format
#include <mpi.h>

int main()
{

#ifdef MPI_VERSION
    int mpi_version = MPI_VERSION;
#endif
#ifdef MPI_SUBVERSION
    int mpi_subversion = MPI_SUBVERSION;
#endif
#ifdef MPICH_NUMVERSION
    int mpich_numversion = MPICH_NUMVERSION;
#endif
#ifdef MPICH_VERSION
    char mpich_version[] = MPICH_VERSION;
#endif
#ifdef I_MPI_NUMVERSION
    int i_mpi_numversion = I_MPI_NUMVERSION;
#endif
#ifdef I_MPI_VERSION
    char i_mpi_version[] = I_MPI_VERSION;
#endif
#ifdef OMPI_MAJOR_VERSION
    int ompi_major_version = OMPI_MAJOR_VERSION;
#endif
#ifdef OMPI_MINOR_VERSION
    int ompi_minor_version = OMPI_MINOR_VERSION;
#endif
#ifdef OMPI_RELEASE_VERSION
    int ompi_release_version = OMPI_RELEASE_VERSION;
#endif
}

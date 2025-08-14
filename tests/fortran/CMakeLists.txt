set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Output/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Output/)

if(NOT OTFCPT_HAVE_Fortran)
  return()
endif()

if(NOT OTFCPT_ENABLE_FORTRAN_SUPPORT)
  enable_language(Fortran)
  find_package(MPI REQUIRED)
endif()

set(fortranTests
    mpi-balanced 
    mpi-imbalance
    #mpi-message
    #mpi-serialization
    #mpi-bcast-balanced
    #mpi-bcast-serialization
)

add_library(m_time OBJECT m_time.f90)

FOREACH(tests ${fortranTests})
        add_executable(${tests}.f90.tmp ${tests}.f90 ../expected-metrics.c)
        target_link_libraries(${tests}.f90.tmp PRIVATE OTFCPT)
        target_link_libraries(${tests}.f90.tmp PUBLIC MPI::MPI_Fortran)
        target_link_libraries(${tests}.f90.tmp PRIVATE m_time)
        add_dependencies(build-fortran-tests ${tests}.f90.tmp)
ENDFOREACH()


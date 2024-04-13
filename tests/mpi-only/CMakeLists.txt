set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Output/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Output/)
set(CMAKE_EXECUTABLE_SUFFIX_C .c.tmp)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .cpp.tmp)

set(mpiOnlyTests
    mpi-balanced 
    mpi-imbalance
    mpi-message
    mpi-serialization
    mpi-bcast-balanced
    mpi-bcast-serialization
)


FOREACH(tests ${mpiOnlyTests})
    add_executable(${tests} ${tests}.c)
    target_link_libraries(${tests} ${MPI_C_LIBRARIES})
    add_dependencies(build-mpi-only-tests ${tests})
ENDFOREACH()

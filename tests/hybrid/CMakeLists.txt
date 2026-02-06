set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Output/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Output/)

set(HybridTests
    omp-mpi-hello
    mpi-omp-hello
)

FOREACH(tests ${HybridTests})
    add_executable(${tests}.c.tmp ${tests}.c)
    target_link_libraries(${tests}.c.tmp OTFCPT)
    target_link_libraries(${tests}.c.tmp OpenMP::OpenMP_C)
    target_link_libraries(${tests}.c.tmp MPI::MPI_C)
    add_dependencies(build-mpi-only-tests ${tests}.c.tmp)
ENDFOREACH()

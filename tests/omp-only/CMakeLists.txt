set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Output/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Output/)
set(CMAKE_EXECUTABLE_SUFFIX_C .c.tmp)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .cpp.tmp)

set(ompOnlyTests
    omp-serialization
    omp-balanced
    omp-imbalance
    omp-group
)

FOREACH(tests ${ompOnlyTests})
    add_executable(${tests} ${tests}.c)
    target_link_libraries(${tests} PUBLIC OpenMP::OpenMP_C)
    add_dependencies(build-omp-only-tests ${tests})
ENDFOREACH()
add_executable(test test.cpp)
target_link_libraries(test ${MPI_C_LIBRARIES})
add_executable(test-wc-source test-wc-source.cpp)
target_link_libraries(test-wc-source ${MPI_C_LIBRARIES})
add_executable(test-wc-tag test-wc-tag.cpp)
target_link_libraries(test-wc-tag ${MPI_C_LIBRARIES})
add_executable(test-group test-group.cpp)
target_link_libraries(test-group ${MPI_C_LIBRARIES})

add_executable(test-omp test-omp.c)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(test PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(test-wc-source PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(test-wc-tag PUBLIC OpenMP::OpenMP_CXX)
endif()

if(OpenMP_C_FOUND)
    target_link_libraries(test-omp PUBLIC OpenMP::OpenMP_C)
endif()


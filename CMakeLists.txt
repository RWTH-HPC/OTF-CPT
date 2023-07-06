cmake_minimum_required(VERSION 3.0)

project(OTFCPT C CXX)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

set(WRAP ${OTFCPT_SOURCE_DIR}/external/wrap/wrap.py)
include(${OTFCPT_SOURCE_DIR}/external/wrap/WrapConfig.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

SET(CMAKE_MODULE_PATH
    "${OTFCPT_SOURCE_DIR}/external/CMake-MPIhelper/cmake"
    ${CMAKE_MODULE_PATH}
    )

include(CheckMPIFunctionExists)
include(CheckMPISymbolExists)
include(CheckMPISymbolIsRvalue)
include(CheckMPITypeExists)

macro (check_mpi_function_exists_pp function)
    STRING(TOUPPER "HAVE_${function}" havevariable)
    check_mpi_symbol_is_rvalue(${function} ${havevariable})
endmacro (check_mpi_function_exists_pp)
macro (check_mpi_type_exists_pp type)
    STRING(TOUPPER "HAVE_${type}" havevariable)
    check_mpi_type_exists(${type} ${havevariable})
endmacro (check_mpi_type_exists_pp)

check_mpi_type_exists_pp(MPI_Session)
check_mpi_type_exists_pp(MPI_Count)
check_mpi_function_exists_pp(MPI_Barrier_init)

if(HAVE_MPI_SESSION)
    add_definitions(-DHAVE_SESSION)
endif()
if(HAVE_MPI_COUNT)
    add_definitions(-DHAVE_COUNT)
endif()
if(HAVE_MPI_BARRIER_INIT)
    add_definitions(-DHAVE_PCOLL)
endif()

include_directories(${OTFCPT_SOURCE_DIR} ${MPI_C_INCLUDE_PATH} ${MPI_CXX_INCLUDE_PATH})

set(OTFCPT_SOURCES
    completion-wrappers.cpp
    critical-core.cpp
    man-wrappers.cpp
    mpi-critical.cpp
    ompt-critical.cpp
    tracking.cpp
    gen-nb-wrappers.cpp
    gen-wrappers.cpp
)

set(WRAP_FLAGS -s -n)
add_wrapped_file(gen-nb-wrappers.cpp gen-nb-wrappers.w)
add_wrapped_file(gen-wrappers.cpp gen-wrappers.w)

add_library(OTFCPT SHARED ${OTFCPT_SOURCES})
target_link_libraries(OTFCPT ${MPI_C_LIBRARIES})

add_library(OTFCPT_static STATIC ${OTFCPT_SOURCES})
target_link_libraries(OTFCPT_static ${MPI_C_LIBRARIES})

add_subdirectory(tests)

install(TARGETS OTFCPT OTFCPT_static)

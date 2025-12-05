cmake_minimum_required(VERSION 3.13.4...3.27.4)

project(OTFCPT C CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Installation directory" FORCE)
  message(WARNING "No build type selected. OTF-CPT will be built as Release.
Available options are:
  * -DCMAKE_BUILD_TYPE=Release - For an optimized build with no assertions or debug info.
  * -DCMAKE_BUILD_TYPE=Debug - For an unoptimized build with assertions and debug info.
  * -DCMAKE_BUILD_TYPE=RelWithDebInfo - For an optimized build with no assertions but with debug info.
  * -DCMAKE_BUILD_TYPE=MinSizeRel - For a build optimized for size instead of speed.
Learn more about these options in LLVM's documentation at https://llvm.org/docs/CMake.html#cmake-build-type
")
endif()

find_package(MPI REQUIRED)

SET(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmakemodules"
    ${CMAKE_MODULE_PATH}
    )

include(TestMpiFeatures)
set_mpi_test_features()

include(CheckLanguage)

set(MPI_CXX_SKIP_MPICXX TRUE)
add_definitions(-DOMPI_SKIP_MPICXX)

set(OTFCPT_HAVE_Fortran False)
set(OTFCPT_USE_Fortran_WRAPPERS False)

check_language(Fortran)
if(CMAKE_Fortran_COMPILER)
  set(OTFCPT_HAVE_Fortran True)
  if("${TEST_MPI_FEATURES}" MATCHES "ompi")
    set(OTFCPT_USE_Fortran_WRAPPERS True)
    # We don't test for OpenMPI 4 with <4.1.6, because MPI_F_STATUS_IGNORE 
    # is broken with some of these versions
    if("${TEST_MPI_FEATURES}" MATCHES "ompi-4.0" OR 
       "${TEST_MPI_FEATURES}" MATCHES "ompi-4.1.[0-5]")
      set(OTFCPT_HAVE_Fortran False)
      set(OTFCPT_USE_Fortran_WRAPPERS False)
    endif()
  endif()
  if("${TEST_MPI_FEATURES}" MATCHES "\\[\\]")
    message(STATUS "Detection of MPI implementation failed, disabling Fortran support")
    set(OTFCPT_HAVE_Fortran False)
    set(OTFCPT_USE_Fortran_WRAPPERS False)
  endif()
endif()

option(OTFCPT_ENABLE_FORTRAN_WRAPPERS "Build OTF-CPT with pmpi Fortran wrappers" ${OTFCPT_USE_Fortran_WRAPPERS})

if(OTFCPT_ENABLE_FORTRAN_WRAPPERS)
  if(NOT OTFCPT_HAVE_Fortran)
      message(ERROR "No Fortran support available, but requested with OTFCPT_ENABLE_FORTRAN_WRAPPERS")
  endif()
  enable_language(Fortran)
  find_package(MPI REQUIRED)
elseif(OTFCPT_HAVE_Fortran AND NOT OTFCPT_USE_Fortran_WRAPPERS)
  message(STATUS "Assuming native Fortran support for MPICH-3 and IntelMPI")
else()
  message(STATUS "No Fortran support")
endif()

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
check_mpi_function_exists_pp(PMPI_Isendrecv_replace)

if(HAVE_MPI_SESSION)
    add_definitions(-DHAVE_SESSION)
endif()
if(HAVE_MPI_COUNT)
    add_definitions(-DHAVE_COUNT)
endif()
if(HAVE_MPI_BARRIER_INIT)
    add_definitions(-DHAVE_PCOLL)
endif()
if(HAVE_PMPI_ISENDRECV_REPLACE)
    add_definitions(-DHAVE_ISRR)
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
    parse_flags.cpp
    debug.cpp
)

set(OTFCPT_OMP_SOURCES
    critical-core.cpp
    ompt-critical.cpp
    parse_flags.cpp
    debug.cpp
)

if(OTFCPT_ENABLE_FORTRAN_WRAPPERS)
    add_definitions(-DFORTRAN_SUPPORT)
    list(APPEND OTFCPT_SOURCES fort-wrappers.c)
endif()

list(APPEND GnuCompatible "Clang" "IntelLLVM" "GNU")
list(APPEND ClangCompatible "Clang" "IntelLLVM")
option(OTFCPT_USE_BACKWARD "Use Backward-CPP for stacktraces" OFF)
if ("${CMAKE_CXX_COMPILER_ID}" IN_LIST GnuCompatible)
  set(NOSTDLIB_FLAG "-nostdlib++")
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13)
    set(NOSTDLIB_FLAG -nodefaultlibs -lc)
  endif ()
  set(STDCPLUS_COMPILE_FLAGS -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections)
  set(STDCPLUS_LINK_FLAGS -fno-rtti -fno-exceptions ${NOSTDLIB_FLAG} -Wl,--gc-sections)

  if ("${CMAKE_CXX_COMPILER_ID}" IN_LIST ClangCompatible)
    option(OTFCPT_USE_LLVM_LIBCPP "Use LLVM libc++, which might reduce portability." OFF)
    if (OTFCPT_USE_LLVM_LIBCPP)
      message(STATUS "Building with LLVM libc++ was requested, which might limit portability and compatibility with application/library code compiled with gcc.")
      set(STDCPLUS_COMPILE_FLAGS -stdlib=libc++)
      set(STDCPLUS_LINK_FLAGS -stdlib=libc++)
      if(OTFCPT_USE_BACKWARD)
        set(LINK_EXTERNAL_LIBRARIES Backward::Interface)
        add_compile_definitions(-DUSE_BACKWARD)
      endif()
      add_compile_definitions(-DUSE_STL)
    else ()
      message(STATUS "For portability, OTF-CPT is built without dependency to a C++ runtime library")
      set(STDCPLUS_COMPILE_FLAGS ${STDCPLUS_COMPILE_FLAGS} -stdlib=libstdc++)
      set(STDCPLUS_LINK_FLAGS ${STDCPLUS_LINK_FLAGS} -stdlib=libstdc++)
    endif ()
  endif ()
endif ()
if (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set(STDCPLUS_COMPILE_FLAGS -fno-exceptions)
  set(STDCPLUS_LINK_FLAGS -cxxlib-nostd)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  include_directories(external/ompt)
endif()

option(OTFCPT_USE_GNU_LIBCPP "Use GNU libstdc++, which might reduce portability." OFF)
if (OTFCPT_USE_GNU_LIBCPP)
  if (OTFCPT_USE_LLVM_LIBCPP)
    message(WARNING "OTFCPT_USE_GNU_LIBCPP and OTFCPT_USE_LLVM_LIBCPP are on at the same time, will build with GNU libstdc++")
  endif()
  if ("${CMAKE_CXX_COMPILER_ID}" IN_LIST ClangCompatible)
    set(STDCPLUS_COMPILE_FLAGS -stdlib=libstdc++)
    set(STDCPLUS_LINK_FLAGS -stdlib=libstdc++)
  else()
    set(STDCPLUS_COMPILE_FLAGS "")
    set(STDCPLUS_LINK_FLAGS "")
  endif()
  add_compile_definitions(-DUSE_STL)
  if (OTFCPT_USE_BACKWARD)
    add_compile_definitions(-DUSE_BACKWARD)
    set(LINK_EXTERNAL_LIBRARIES Backward::Interface)
  endif()
else()
  if(OTFCPT_USE_BACKWARD AND NOT OTFCPT_USE_LLVM_LIBCPP)
    message(WARNING "OTFCPT_USE_GNU_LIBCPP or OTFCPT_USE_LLVM_LIBCPP should be enabled in order to use BackwardCPP, ignoring.")
  endif()
endif()

set(WRAP_FLAGS -s -n)

if(OTFCPT_ENABLE_FORTRAN_WRAPPERS)
  set(WRAP_FLAGS -n -f)
endif()

if (OTFCPT_USE_BACKWARD)
  add_subdirectory(${CMAKE_SOURCE_DIR}/external/backward-cpp)
endif()

add_wrapped_file(gen-nb-wrappers.cpp gen-nb-wrappers.w)
add_wrapped_file(gen-wrappers.cpp gen-wrappers.w)

add_library(OTFCPT SHARED ${OTFCPT_SOURCES})
target_link_libraries(OTFCPT PUBLIC MPI::MPI_CXX ${LINK_EXTERNAL_LIBRARIES})
target_compile_definitions(OTFCPT PUBLIC -DUSE_MPI=1)
target_link_options(OTFCPT PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${STDCPLUS_LINK_FLAGS}>" "LINKER:-as-needed")
target_compile_options(OTFCPT PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${STDCPLUS_COMPILE_FLAGS}>")

add_library(OTFCPT_omp SHARED ${OTFCPT_OMP_SOURCES})
target_link_libraries(OTFCPT_omp PUBLIC ${LINK_EXTERNAL_LIBRARIES})
target_link_options(OTFCPT_omp PUBLIC ${STDCPLUS_LINK_FLAGS} "LINKER:-as-needed")
target_compile_options(OTFCPT_omp PUBLIC ${STDCPLUS_COMPILE_FLAGS})

add_library(OTFCPT_static STATIC ${OTFCPT_SOURCES})
target_link_libraries(OTFCPT_static PUBLIC MPI::MPI_CXX ${LINK_EXTERNAL_LIBRARIES})
target_compile_definitions(OTFCPT_static PUBLIC -DUSE_MPI=1)
target_link_options(OTFCPT_static PUBLIC ${STDCPLUS_LINK_FLAGS})
target_compile_options(OTFCPT_static PUBLIC ${STDCPLUS_COMPILE_FLAGS})

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/external/FileCheck/CMakeLists.txt)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/FileCheck EXCLUDE_FROM_ALL)
endif()

add_subdirectory(tests)


install(TARGETS 
        OTFCPT 
        OTFCPT_omp
        OTFCPT_static
        )

install (PROGRAMS script/CPT-plot.py DESTINATION bin)

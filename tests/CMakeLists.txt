include(TestHelperMacros)

OPTION(LIT_FORWARD_FULL_ENV "Lit should forward all environmental variables to test execution" FALSE)

set(shipped_lit_executable "${CMAKE_SOURCE_DIR}/external/llvm-lit/lit.py")
if (EXISTS "${shipped_lit_executable}")
    # Use shipped lit if available but still respect user-defined paths to the executable.
    set(LLVM_LIT_PATH "${shipped_lit_executable}" CACHE FILEPATH "llvm-lit executable.")
endif()
unset(shipped_lit_executable)

set(OTFCPT_Test_avail TRUE)

find_package(LIT 12.0.0)

if(NOT LIT_FOUND)
    message(STATUS "Could not find llvm's lit executable needed for testing. \
Please make sure that either the LLVM package is in your CMake module search \
path or that the executable is in your PATH environment variable. \
Alternatively you might want to set the CMake variable LLVM_TOOLS_BINARY_DIR \
to the path containing the lit executable.")
set(OTFCPT_Test_avail FALSE)
endif()

set(shipped_filecheck_executable "$<TARGET_FILE:FileCheck_Standalone>")
set(BUILD_FILECHECK FALSE)
if (TARGET FileCheck_Standalone)
    # Use shipped FileCheck if available but still respect user-defined paths to the binary.
    set(LLVM_FILECHECK_PATH "${shipped_filecheck_executable}" CACHE FILEPATH "Filecheck executable.")

    if ("${LLVM_FILECHECK_PATH}" STREQUAL "${shipped_filecheck_executable}")
        set(BUILD_FILECHECK TRUE)
    endif()
endif()

find_package(FileCheck)

if(NOT FileCheck_FOUND)
    message(STATUS "Could not find llvm's FileCheck executable needed for \
    testing. Please make sure that either the LLVM package is in your CMake \
    module search path or that the executable is in your PATH environment \
    variable. Alternatively you might want to set the CMake variable \
    LLVM_TOOLS_BINARY_DIR to the path containing the FileCheck executable.")
set(OTFCPT_Test_avail FALSE)
endif()

include_directories(./)

set(OTFCPT_TEST_UB 1.01 CACHE STRING "Sets an upper bound for the relative error.")
set(OTFCPT_TEST_LB 0.95 CACHE STRING "Sets a lower bound for the relative error.")
set(OTFCPT_TEST_WORK 100000 CACHE STRING "Sets the time in micro seconds used to emulate work.")

add_compile_definitions(UB_FAK=${OTFCPT_TEST_UB} LB_FAK=${OTFCPT_TEST_LB} WORK=${OTFCPT_TEST_WORK})

add_custom_target(build-tests)
add_custom_target(build-mpi-only-tests)
add_custom_target(build-omp-only-tests)
add_custom_target(build-hybrid-tests)
add_dependencies(build-tests build-mpi-only-tests build-omp-only-tests build-hybrid-tests build-fortran-tests OTFCPT OTFCPT_omp)
if (TARGET FileCheck_Standalone AND BUILD_FILECHECK)
    add_dependencies(build-tests FileCheck_Standalone)
endif()
IF(CMAKE_Fortran_COMPILER)
   add_dependencies(build-tests build-fortran-tests)
   add_custom_target(build-fortran-tests)
   add_subdirectory(fortran)
ENDIF()


add_subdirectory(datastructures)
add_subdirectory(mpi-only)

if(OpenMP_C_FOUND)
    add_subdirectory(hybrid)
    add_subdirectory(omp-only)
endif()

if(NOT OTFCPT_Test_avail)
set(OTFCPT_Test_avail FALSE)
    message(STATUS "No tests available with missing lit/FileCheck")
    return()
endif()


set(FILECHECK_EXECUTABLE "${LLVM_FILECHECK_PATH}")
set(LIT_EXECUTABLE "${LLVM_LIT_PATH}")
set(OTFCPT_LIB $<TARGET_FILE:OTFCPT>)
set(OTFCPT_OMP_LIB $<TARGET_FILE:OTFCPT_omp>)

set(LIT_TEST_WORKERS 1 CACHE STRING "Sets the amount of workers when running tests. (Note that this only sets the amount of lit workers of which each might spawn more processes.)")

pythonize_bool(OpenMP_FOUND)
pythonize_bool(LIT_FORWARD_FULL_ENV)
pythonize_bool(OTFCPT_HAVE_Fortran)

configure_file(lit.site.cfg.in lit.site.cfg.configured @ONLY)
file(GENERATE
     OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg"
     INPUT "${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg.configured"
)

if (TARGET FileCheck_Standalone AND MUST_BUILD_FILECHECK)
    add_dependencies(build-test FileCheck_Standalone)
endif()


add_check_target(NAME check SUITES . COMMENT "Run all OTF-CPT tests" DEPENDS build-tests)


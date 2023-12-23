# Part of the MUST Project, under BSD-3-Clause License
# See https://hpc.rwth-aachen.de/must/LICENSE for license information.
# SPDX-License-Identifier: BSD-3-Clause

#[=======================================================================[.rst:
FindLIT
---------

The module defines the following cache variables:

``LLVM_LIT_PATH``
  Path to lit command-line executable.
``LIT_FOUND``
  True if the lit command-line executable was found.

The following :prop_tgt:`IMPORTED` targets are also defined:

``LIT::LIT``
  The command-line executable.

Example usage:

.. code-block:: cmake

   find_package(LIT)
   if(LIT_FOUND)
     message("llvm lit found: ${LLVM_LIT_PATH}")
   endif()
#]=======================================================================]

find_package(LLVM QUIET CONFIG)

set(LLVM_LIT_PATH "LLVM_LIT_PATH-NOTFOUND" CACHE FILEPATH "llvm-lit executable.")
find_program(LLVM_LIT_PATH
        NAMES llvm-lit lit lit.py llvm-lit.py
        NAMES_PER_DIR
        HINTS "${LLVM_TOOLS_BINARY_DIR}"
        DOC "llvm-lit executable."
        )

if(LLVM_LIT_PATH AND NOT TARGET LIT::LIT)
    add_executable(LIT::LIT IMPORTED)
    set_property(TARGET LIT::LIT PROPERTY IMPORTED_LOCATION ${LLVM_LIT_PATH})

    execute_process(
            COMMAND "${LLVM_LIT_PATH}" --version
            RESULT_VARIABLE _EXEC_RESULT
            OUTPUT_VARIABLE stdout
    )

    if ("${_EXEC_RESULT}" STREQUAL 0)
        string(REGEX MATCH [[^lit ([0-9]+)\.([0-9]+)\.([0-9])+]] MATCH "${stdout}")

        set(LLVM_LIT_VERSION_MAJOR ${CMAKE_MATCH_1})
        set(LLVM_LIT_VERSION_MINOR ${CMAKE_MATCH_2})
        set(LLVM_LIT_VERSION_PATCH ${CMAKE_MATCH_3})
        set(LLVM_LIT_VERSION "${LLVM_LIT_VERSION_MAJOR}.${LLVM_LIT_VERSION_MINOR}.${LLVM_LIT_VERSION_PATCH}")
    endif ()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIT
        REQUIRED_VARS LLVM_LIT_PATH
        VERSION_VAR LLVM_LIT_VERSION)

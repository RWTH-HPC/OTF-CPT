# Part of the MUST Project, under BSD-3-Clause License
# See https://hpc.rwth-aachen.de/must/LICENSE for license information.
# SPDX-License-Identifier: BSD-3-Clause

#[=======================================================================[.rst:
FindFileCheck
---------

The module defines the following cache variables:

``LLVM_FILECHECK_PATH``
  Path to FileCheck command-line executable.
``FileCheck_FOUND``
  True if the FileCheck command-line executable was found.

The following :prop_tgt:`IMPORTED` targets are also defined:

``FileCheck::FileCheck``
  The command-line executable.

Example usage:

.. code-block:: cmake

   find_package(FileCheck)
   if(FileCheck_FOUND)
     message("llvm FileCheck found: ${LLVM_FILECHECK_PATH}")
   endif()
#]=======================================================================]

find_package(LLVM QUIET CONFIG)

set(LLVM_FILECHECK_PATH "LLVM_FILECHECK_PATH-NOTFOUND" CACHE FILEPATH "Filecheck executable.")
find_program(LLVM_FILECHECK_PATH
        NAMES Filecheck FileCheck filecheck
        NAMES_PER_DIR
        HINTS ${LLVM_TOOLS_BINARY_DIR}
        DOC "Filecheck executable."
        )

if(LLVM_FILECHECK_PATH AND NOT TARGET FileCheck::FileCheck)
    add_executable(FileCheck::FileCheck IMPORTED)
    set_property(TARGET FileCheck::FileCheck PROPERTY IMPORTED_LOCATION ${LLVM_FILECHECK_PATH})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FileCheck
        REQUIRED_VARS LLVM_FILECHECK_PATH)

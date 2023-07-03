# This file is part of CMake-MPIhelper.
#
# Copyright (C)
#   2015 RWTH Aachen University, Federal Republic of Germany
#
# See the file LICENSE in the package base directory for details.

# Set the minimum required CMake version.
cmake_minimum_required(VERSION 2.8.12)
IF (POLICY CMP0075)
    CMAKE_POLICY(PUSH)
    CMAKE_POLICY(SET CMP0075 NEW)
ENDIF ()

include(CheckTypeSize)

IF (POLICY CMP0075)
    CMAKE_POLICY(POP)
ENDIF ()


## \brief Wrapper for check_type_size to test whether an MPI \p type exists
## that is not found through check_symbol_exists (e.g. typedefs or enums).
#
# \details Sets MPI environment and calls check_type_size for \p type.
#
#
# \param type Type to test.
# \param variable Variable to be set, if type exists.
#
# \return If \p type was found, \p variable will be set to the size of the type.
#  In any other case \p variable will become not be set.
#
function(CHECK_MPI_TYPE_EXISTS type variable)
	# Note: a check against CMake cache is not needed in this function, thus all
	# called functions implement such a check.

	# search for MPI environment
	IF (POLICY CMP0074)
		CMAKE_POLICY(PUSH)
		#if MPI_ROOT is set, use it for finding MPI
		CMAKE_POLICY(SET CMP0074 NEW)
	ENDIF ()

	find_package(MPI)

	IF (POLICY CMP0074)
		CMAKE_POLICY(POP)
	ENDIF ()

	if (MPI_C_FOUND)
		# check for symbol
		# set environment
		set(CMAKE_REQUIRED_FLAGS "${MPI_C_COMPILE_FLAGS}")
		set(CMAKE_REQUIRED_DEFINITIONS "")
		set(CMAKE_REQUIRED_INCLUDES "${MPI_C_INCLUDE_PATH}")
		set(CMAKE_EXTRA_INCLUDE_FILES "mpi.h")
		set(CMAKE_REQUIRED_LIBRARIES "${MPI_C_LIBRARIES}")

        check_type_size(${type} ${variable})
        IF (POLICY CMP0054)
            CMAKE_POLICY(PUSH)
            CMAKE_POLICY(SET CMP0054 NEW)
        ENDIF ()
        if ("${variable}" STREQUAL "")
            set (${variable} FALSE)
        else ()
            set (${variable} TRUE)
        endif ()
        IF (POLICY CMP0054)
            CMAKE_POLICY(POP)
        ENDIF ()

		return()
	endif ()

	# set variable to false, if MPI was not found
	set(${variable} false)
endfunction(CHECK_MPI_TYPE_EXISTS)


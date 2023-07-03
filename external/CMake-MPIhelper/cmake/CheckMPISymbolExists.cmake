# This file is part of CMake-MPIhelper.
#
# Copyright (C)
#   2015 RWTH Aachen University, Federal Republic of Germany
#
# See the file LICENSE in the package base directory for details.

# Set the minimum required CMake version.
cmake_minimum_required(VERSION 2.8.12)


include(CheckSymbolExists)


## \brief Wrapper for check_symbol_exists to test \p symbol in MPI environment.
#
# \details Sets MPI environment and calls check_symbol_exists for \p symbol.
#
#
# \param symbol Symbol to test.
# \param variable Variable to be set, if symbol exists.
#
# \return If \p symbol was found, \p variable will be set true. In any other
#   case \p variable will become false.
#
function(CHECK_MPI_SYMBOL_EXISTS symbol variable)
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
		# set environment
		set(CMAKE_REQUIRED_FLAGS "${MPI_C_COMPILE_FLAGS}")
		set(CMAKE_REQUIRED_DEFINITIONS "")
		set(CMAKE_REQUIRED_INCLUDES "${MPI_C_INCLUDE_PATH}")
		set(CMAKE_REQUIRED_LIBRARIES "${MPI_C_LIBRARIES}")

		# check for symbol
		check_symbol_exists(${symbol} "mpi.h" ${variable})
		return()
	endif ()

	# set variable to false, if MPI was not found
	set(${variable} false)
endfunction(CHECK_MPI_SYMBOL_EXISTS)

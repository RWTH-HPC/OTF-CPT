# This file is part of CMake-MPIhelper.
#
# Copyright (C)
#   2021 RWTH Aachen University, Federal Republic of Germany
#
# See the file LICENSE in the package base directory for details.

# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.13.4...3.27.4)


include(CheckCSourceCompiles)


## \brief Check whether a symbol can be used as an rvalue in MPI environment.
#
# \details Sets MPI environment and calls check_c_source_compiles to check, if
#   \p symbol is allowed to be used as an rvalue. The intention of this module 
#   is to check whether a specific symbol is marked as deprecated or not 
#   available anymore in the MPI version used.
#
#
# \param symbol Symbol to test.
# \param variable Variable to be set, if symbol exists.
#
# \return If \p symbol was found and no compilation error occurred , \p variable
#   will be set true. In any other case \p variable will become false.
#
function(CHECK_MPI_SYMBOL_IS_RVALUE symbol variable)
	# NOTE: A check against CMake cache is not needed in this function, as all
	#       functions called implement such a check.

	# search for MPI environment
	find_package(MPI)

	if (MPI_C_FOUND)
		# set environment
		set(CMAKE_REQUIRED_FLAGS "${MPI_C_COMPILE_OPTIONS}")
		set(CMAKE_REQUIRED_DEFINITIONS "${MPI_C_COMPILE_DEFINITIONS}")
		set(CMAKE_REQUIRED_INCLUDES "${MPI_C_INCLUDE_DIRS}")
		set(CMAKE_REQUIRED_LIBRARIES "${MPI_C_LIBRARIES}")

		# check for symbol
		check_c_source_compiles("
			#include <mpi.h>

			int main(int argc, char** argv)
			{
				(void)argv;
				argc = (int)${symbol};
				return argc;
			}"
			${variable})
		return()
	endif ()

	# set variable to false, if MPI was not found
	set(${variable} false)
endfunction(CHECK_MPI_SYMBOL_IS_RVALUE)

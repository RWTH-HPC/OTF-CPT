# This file is part of CMake-MPIhelper.
#
# Copyright (C)
#   2015 RWTH Aachen University, Federal Republic of Germany
#
# See the file LICENSE in the package base directory for details.

# Set the minimum required CMake version.
#
# Note: CheckFortranSourceCompiles was introduced in CMake 3.0, but this CMake
#       module shipps these files, too, so this module may be used with earlier
#       versions of CMake.
#
cmake_minimum_required(VERSION 3.13.4...3.27.4)


include(CheckFortranSourceCompiles)


## \brief Wrapper for check_fortran_source_compiles to test if \p symbol exists
#   in fortran MPI environment.
#
# \details Sets MPI environment, generates a new source file for \p symbol and
#   calls check_fortran_source_compiles.
#
#
# \param symbol Symbol to test.
# \param variable Variable to be set, if symbol exists.
#
# \return If \p symbol was found, \p variable will be set true. In any other
#   case \p variable will become false.
#
function(CHECK_FORTRAN_MPI_SYMBOL_EXISTS symbol variable)
	# Note: a check against CMake cache is not needed in this function, thus all
	# called functions implement such a check.

	# search for MPI environment
	find_package(MPI)

	if (MPI_Fortran_FOUND)
		# set environment
		set(CMAKE_REQUIRED_FLAGS "${MPI_Fortran_COMPILE_OPTIONS}")
		set(CMAKE_REQUIRED_DEFINITIONS "${MPI_Fortran_COMPILE_DEFINITIONS}")
		set(CMAKE_REQUIRED_INCLUDES "${MPI_Fortran_INCLUDE_DIRS}")
		set(CMAKE_REQUIRED_LIBRARIES "${MPI_Fortran_LIBRARIES}")

		# generate test source
		set(TEST_SOURCE "      PROGRAM basic\n")
		set(TEST_SOURCE "${TEST_SOURCE}      IMPLICIT NONE\n")
		set(TEST_SOURCE "${TEST_SOURCE}      INCLUDE \"mpif.h\"\n")
		set(TEST_SOURCE "${TEST_SOURCE}      INTEGER CODE\n")
		set(TEST_SOURCE "${TEST_SOURCE}      CODE = ${symbol}\n")
		set(TEST_SOURCE "${TEST_SOURCE}      END\n")

		# try to compile source
		check_fortran_source_compiles("${TEST_SOURCE}" ${variable})
	endif ()

	# set variable to false, if MPI was not found
	set(${variable} false)
endfunction(CHECK_FORTRAN_MPI_SYMBOL_EXISTS)

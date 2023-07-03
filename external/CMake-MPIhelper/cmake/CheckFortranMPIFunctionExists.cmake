# This file is part of CMake-MPIhelper.
#
# Copyright (C)
#   2015 RWTH Aachen University, Federal Republic of Germany
#
# See the file LICENSE in the package base directory for details.

# Set the minimum required CMake version.
cmake_minimum_required(VERSION 2.8.12)


include(CheckFortranFunctionExists)


## \brief Wrapper for check_fortran_function_exists to test \p function in
#   Fortran MPI environment.
#
# \details Sets MPI Fortran environment and calls check_function_exists for \p
#   function and P${function}.
#
#
# \param function Function to test.
# \param variable Variable to be set, if function exists.
#
# \return If \p function and P${function} were found, \p variable will be set
#   true. In any other case \p variable will become false.
#
function(CHECK_FORTRAN_MPI_FUNCTION_EXISTS function variable)
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

	if (MPI_Fortran_FOUND)
		# set environment
		set(CMAKE_REQUIRED_FLAGS "${MPI_Fortran_COMPILE_FLAGS}")
		set(CMAKE_REQUIRED_DEFINITIONS "")
		set(CMAKE_REQUIRED_INCLUDES "${MPI_Fortran_INCLUDE_PATH}")
		set(CMAKE_REQUIRED_LIBRARIES "${MPI_Fortran_LIBRARIES}")

		# check for function
		check_fortran_function_exists(${function} __HAVE_${function})
		if (__HAVE_${function})
			check_fortran_function_exists(P${function} __HAVE_P${function})
			if (__HAVE_P${function})
				set(${variable} 1 CACHE INTERNAL
					"Have MPI Fortran function ${function}")
				return()
			endif ()
		endif ()
	endif ()

	# set variable to false, if MPI was not found
	set(${variable} 0)
endfunction(CHECK_FORTRAN_MPI_FUNCTION_EXISTS)

# This file is part of CMake-MPIhelper.
#
# Copyright (C)
#   2015 RWTH Aachen University, Federal Republic of Germany
#
# See the file LICENSE in the package base directory for details.

# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.13.4...3.27.4)


include(CheckTypeSize)


## \brief Sets \p VARIABLE to a valid basic C type derived from \p BTYPE
#   dependent of the size of \p TYPE.
#
# \details Calls check_type_size for \p type and sets \p variable to a basic C
#   type derived from \p BTYPE.
#
#   The following variables may be set before calling this macro to modify the
#   way the check is run:
#
#   CMAKE_REQUIRED_FLAGS = string of compile command line flags
#   CMAKE_REQUIRED_DEFINITIONS = list of macros to define (-DFOO=bar)
#   CMAKE_REQUIRED_INCLUDES = list of include directories
#   CMAKE_REQUIRED_LIBRARIES = list of libraries to link
#   CMAKE_EXTRA_INCLUDE_FILES = list of extra headers to include
#
#
# \param TYPE T
# \param BTYPE Basic C type. May be {int, uint}
# \param VARIABLE Variable to be set to the basic C type
#
# \return If \p TYPE does not exist, \p BTYPE is unknown or \p TYPE has an
#   unknown size, \p VARIABLE is set to "unknown_t". In any other case,
#   \p VARIABLE will be set to a valid basic C type.
#
function (CHECK_TYPE_BASIC_TYPE type btype var)
	if(NOT DEFINED "${var}")
		# get size of TYPE
		check_type_size(${type} "${type}_SIZE")

		# return function, if TYPE does not exist
		if ("${${type}_SIZE}" EQUAL 0)
			return()
		endif ()


		# lookup an suitable basic type
		if ((${btype} STREQUAL "uint") OR (${btype} STREQUAL "int"))
			math(EXPR "bsize" "${${type}_SIZE} * 8")
			set(USE_TYPE "${btype}${bsize}_t")
		endif ()

		# clean environment
		unset(CMAKE_EXTRA_INCLUDE_FILES)

		# does selected type exist?
		check_type_size(${USE_TYPE} "${USE_TYPE}_SIZE")
		if(NOT CMAKE_REQUIRED_QUIET)
			message(STATUS "Looking for basic type of ${type}")
		endif()
		if (${${USE_TYPE}_SIZE} GREATER 0)
			# does length of selected type match the original size?
			if (${${USE_TYPE}_SIZE} EQUAL ${${type}_SIZE})
				set(${var} "${USE_TYPE}" CACHE INTERNAL
					"CHECK_TYPE_BASIC_TYPE: basic type of ${type}")

				if(NOT CMAKE_REQUIRED_QUIET)
					message(STATUS "Looking for basic type of ${type} - done")
				endif()

				return()
			endif ()
		endif ()

		# set VARIABLE as empty, if not already done
		if (NOT ${VARIABLE})
			set(${var} "" CACHE INTERNAL "CHECK_TYPE_BASIC_TYPE: basic type of ${type}")

			if(NOT CMAKE_REQUIRED_QUIET)
				message(STATUS "Looking for basic type of ${type} - failed")
			endif()
		endif ()
	endif(NOT DEFINED "${var}")
endfunction ()

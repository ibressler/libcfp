# CMakeLists.txt
#
# Copyright (c) 2009 Technische Universität Berlin, 
# Stranski-Laboratory for Physical und Theoretical Chemistry
#
# This file is part of libcfp.
#
# libcfp is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libcfp is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libcfp.  If not, see <http://www.gnu.org/licenses/>.

# Author(s) of this file:
# Ingo Bressler (ingo@cs.tu-berlin.de)


set(PRJ_NAME libcfp)
project(${PRJ_NAME})

cmake_minimum_required(VERSION 2.6)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${${PRJ_NAME}_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${${PRJ_NAME}_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${${PRJ_NAME}_SOURCE_DIR}/bin)

# .<adjust here start>. #
# v                   v #

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g -save-temps ${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mfpmath=sse -msse -m3dnow -msse2 -msse3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 ${CMAKE_CXX_FLAGS}")
message(STATUS ">> Configuring for >${CMAKE_BUILD_TYPE}< build")

# ^                  ^ #
# `<adjust here stop>´ #

 #################################
## libs required for the library ## (included, no adjustments necessary)
 #################################

# parent path to boost include files, so that 'boost/config.hpp' can be found
set(BOOST_INC ${${PRJ_NAME}_SOURCE_DIR}/src)

# parent path to Adobe Source Libraries (ASL), 
# so that 'adobe/config.hpp' can be found
set(ADOBE_INC ${${PRJ_NAME}_SOURCE_DIR}/src)

 ########################################
## Setup Testing Framework (UnitTest++) ##
 ########################################

enable_testing()

# add additional search path eventually
set(UNITTEST_PATH
	/usr/src/UnitTest++-1.4/
)
find_library(UNITTEST_LIB UnitTest++ PATHS ${UNITTEST_PATH})
find_path(UNITTEST_INC UnitTest++.h PATHS ${UNITTEST_PATH}
	PATH_SUFFIXES src
)

set(UNITTEST_FOUND false)
if(UNITTEST_LIB AND UNITTEST_INC)
	set(UNITTEST_FOUND true)
endif(UNITTEST_LIB AND UNITTEST_INC)

if(UNITTEST_FOUND)
	message(STATUS ">> UnitTest++ found.")
else(UNITTEST_FOUND)
	message(STATUS ">> UnitTest++ not found ! Disabling tests.")
endif(UNITTEST_FOUND)

 ######################
## subdirs to process ##
 ######################

# tell cmake to process CMakeLists.txt in that subdirectory
add_subdirectory(src)
add_subdirectory(tests)


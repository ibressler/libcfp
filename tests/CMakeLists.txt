# tests/CMakeLists.txt
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

include_directories(
	${${PRJ_NAME}_SOURCE_DIR}/include
)

add_executable(test_${PRJ_NAME}_manual test_manual.cpp)
target_link_libraries(test_${PRJ_NAME}_manual cfp)

if(UNITTEST_FOUND)

include_directories(${UNITTEST_INC})

add_executable(test_${PRJ_NAME}_automated
	test_auto.cpp
	test_auto_error.cpp
	test_auto_element.cpp
	test_auto_parser.cpp
)
target_link_libraries(test_${PRJ_NAME}_automated cfp ${UNITTEST_LIB})

add_test(AutomatedTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_${PRJ_NAME}_automated)

endif(UNITTEST_FOUND)

# src/CMakeLists.txt
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
# Ingo Bressler (libcfp at ingobressler.net)

set(lib_src
	parser.cpp
	parserstate.cpp
	parserdetails.cpp
	token.cpp
	element.cpp
	elementgroup.cpp
	error.cpp
)

include_directories(
	${${PRJ_NAME}_SOURCE_DIR}/include
	${BOOST_INC}
	${ADOBE_INC}
)

add_library(cfp        SHARED ${lib_src})
add_library(cfp_static STATIC ${lib_src})


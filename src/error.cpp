/*
 * src/error.cpp
 *
 * Copyright (c) 2009 Technische Universität Berlin, 
 * Stranski-Laboratory for Physical und Theoretical Chemistry
 *
 * This file is part of libcfp.
 *
 * libcfp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcfp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libcfp.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * Author(s) of this file:
 * Ingo Bressler (libcfp at ingobressler.net)
 */

#include <cfp/error.h>

using namespace cfp;

Error::Error(const std::string& msg, size_t start, size_t length)
	: msg_m(msg),
	  start_m(start), 
	  length_m(length)
{
}

Error::Error(size_t start, size_t length)
	: msg_m(),
	  start_m(start), 
	  length_m(length)
{
}

Error::~Error() throw()
{
}

void Error::setMessage(const std::string& msg)
{
	msg_m.assign(msg);
}

const char *
Error::what() const throw()
{
	return whatStr().c_str();
}

const std::string&
Error::whatStr() const throw()
{
	return msg_m;
}

const char*
Error::what(size_t& start, size_t& length) const throw()
{
	return whatStr(start, length).c_str();
}

const std::string&
Error::whatStr(size_t& start, size_t& length) const throw()
{
	start = start_m;
	length = length_m;
	return whatStr();
}


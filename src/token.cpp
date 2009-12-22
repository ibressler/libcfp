/*
 * src/token.cpp
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
 * Ingo Bressler (ingo at cs.tu-berlin.de)
 */

#include <iostream>
#include <sstream>
#include "token.h"

using namespace cfp;

Token::Token()
	: type(TYPE_NONE), mStr()
{
}

void Token::append(char c)
{
	mStr.append(&c, 1);
}

size_t Token::length() const
{
	return mStr.length();
}

void Token::clear()
{
	mStr.clear();
}

const std::string& Token::toString() const
{
	return mStr;
}

int Token::toInt() const
{
	std::stringstream ss(mStr);
	int i;
	ss >> i;
	return i;
}

double Token::toDouble() const
{
	std::stringstream ss(mStr);
	double d;
	ss >> d;
	return d;
}

std::ostream& std::operator<<(std::ostream& o, const cfp::Token::Type& t)
{
	switch(t){
		case Token::TYPE_SYMBOL:
			o << "SYMBOL";
			break;
		case Token::TYPE_GROUP:
			o << "GROUP";
			break;
		case Token::TYPE_INT:
			o << "NUM_INT";
			break;
		case Token::TYPE_FLOAT:
			o << "NUM_FLOAT";
			break;
		default:
			o << "NONE";
			break;
	}
	return o;
}

std::ostream& std::operator<<(std::ostream& o, const cfp::Token& t)
{
	o << t.toString() << "(" << t.type << ")";
	return o;
}



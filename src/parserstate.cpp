/*
 * src/parserstate.cpp
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

#include "parserstate.h"

#define MAX_RECURSION_LVL 30

using namespace cfp;

ParserState::ParserState()
	: maxNestingLevel(MAX_RECURSION_LVL),
	  curNestingLevel(MAX_RECURSION_LVL),
	  curPos(0),
	  formula(),
	  rootGroup(),
	  mCurGroup(&rootGroup)
{}

void 
ParserState::reset(const char * f, const size_t l)
{
	curNestingLevel = maxNestingLevel;
	curPos = 0;
	rootGroup.clear();
	mCurGroup = &rootGroup;
	if (f) formula.assign(f, l);
	else   formula.clear();
}

void 
ParserState::setToken(Token & t)
{
	mToken = &t;
}

Token & 
ParserState::token(void)
{
	return *mToken;
}

void 
ParserState::setCurrentGroup(ElementGroup & eg)
{
	mCurGroup = &eg;
}

ElementGroup & 
ParserState::currentGroup(void)
{
	return *mCurGroup;
}


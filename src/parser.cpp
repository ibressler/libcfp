/*
 * src/parser.cpp
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

#include <iostream>
#include <sstream>
#include <cfp/cfp.h>
#include "parserstate.h"

using namespace cfp;

Parser::Parser()
	: mD(new ParserState())
{
}

Parser::Parser(const char * formula, const size_t len)
	: mD(new ParserState())
{
	mD->reset(formula, len);
}

Parser::Parser(const Parser& p)
	: mD(new ParserState(*(p.mD)))
{
}

Parser::~Parser()
{
	delete mD;
}

void
Parser::setFormula(const std::string& formula)
{
	mD->reset(formula.c_str(), formula.length());
}

void
Parser::setFormula(const char * formula, const size_t len)
{
	mD->reset(formula, len);
}

const std::string& 
Parser::formula() const
{
	return mD->formula;
}

void 
Parser::setMaxNestingLevel(size_t lvl)
{
	mD->maxNestingLevel = lvl;
	mD->curNestingLevel = lvl;
}

size_t 
Parser::maxNestingLevel() const
{
	return mD->maxNestingLevel;
}

const Compound& 
Parser::empirical() const
{
	return mD->rootGroup.flatList();
}

const Compound& 
Parser::process(const char * formula, const size_t len)
{
	setFormula(formula, len);
	process();
	return empirical();
}

void 
Parser::process()
{
	if (!mD || mD->formula.empty()) return;

	mD->parse();
	mD->rootGroup.flatten();
}

std::string 
Parser::toMarkup(void) const
{
	std::stringstream ss;
	ss << mD->rootGroup;
	return ss.str();
}


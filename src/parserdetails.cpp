/*
 * src/parserdetails.cpp
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
#include <cfp/cfp.h>
#include "parserstate.h"

using namespace cfp;

///// ElementGroup /////

void 
ElementGroup::addToken(ParserState& s)
{
	if (s.token().length() == 0) return;
	(this->*mAddTokenFunctions.at(s.token().type))(s);
}

void
ElementGroup::addTokenSymbol(ParserState& s)
{
	// last element has symbol or is list
	if ( empty() || 
	    !back().symbol().empty() ||
	     back().isGroup())
	{
		add(CompoundGroupElement());
	}
	back().setSymbol( s.token().toString() );
	mLastElementProperty = SYMBOL_PROPERTY;
}

void
ElementGroup::addTokenInt(ParserState& s)
{
	if (empty() || mLastElementProperty == COEFFICIENT_PROPERTY)
	{
		add(CompoundGroupElement());
		back().setNucleons( s.token().toInt() );
		mLastElementProperty = NUCLEON_PROPERTY;
	} else {
		back().setCoefficient( s.token().toDouble() );
		mLastElementProperty = COEFFICIENT_PROPERTY;
	}
}

void
ElementGroup::addTokenFloat(ParserState& s)
{
	if (empty() || mLastElementProperty == COEFFICIENT_PROPERTY)
	{
		size_t pos = 0;
		size_t width = size_t(s.token().length());
		// get the start position of this symbol, we're behind it
		if (s.curPos >= width) 
			pos = s.curPos - width;
		throw ErrorStartWithCoef(pos, 1);
	} else {
		back().setCoefficient( s.token().toDouble() );
		mLastElementProperty = COEFFICIENT_PROPERTY;
	}
}

void
ElementGroup::addTokenGroup(ParserState& s)
{
}

void
ElementGroup::addTokenNone(ParserState& s)
{
}

void
ElementGroup::addSubgroup(const ElementGroup& subGrp, size_t strIdx)
{
	if (subGrp.empty() ) return;

	if (!empty() &&
	    !back().isGroup() &&
	     back().symbol().empty() ) // no symbol yet
	{
		// we add a valid group and the last element is not ready yet
		throw ErrorLoneNucleonNum(strIdx, 1);
	}
	if ( subGrp.size() == 1 && // single element
	    !subGrp.back().isGroup() && // not a list
	     subGrp.back().symbol().empty() && // no symbol but nucleon number
	     subGrp.back().isIsotope() )
	{
		if (empty())
		{
			throw ErrorLoneNucleonNum(strIdx, 1);
		} else {
			back().setNucleons(
				subGrp.back().nucleons() );
			mLastElementProperty = NUCLEON_PROPERTY;
		}
	} 
	else if (subGrp.size() >= 1)
	{
		addGroup(subGrp);
		mLastElementProperty = GROUP_PROPERTY;
	}
}

///// ParserState /////

/// Tests for upper case characters.
#define IS_ALPHA_U(c) (( 65 <= (c)) && ((c) <= 90 ))

/// Tests for lower case characters.
#define IS_ALPHA_L(c) (( 97 <= (c)) && ((c) <= 122))

/// Tests for alphabetic characters.
#define IS_ALPHA(c)   (IS_ALPHA_U((c)) || IS_ALPHA_L((c)))

/// Tests for numerical characters.
#define IS_NUM(c)     (( 48 <= (c)) && ((c) <= 57 ))

/// Tests for the space character.
#define IS_SPACE(c)    ( 32 == (c))

/// Tests for decimal separators , .
#define IS_DECIM(c)   (( 44 == (c)) || ((c) == 46 ))

/// Tests for opening brackets ( [ {
#define IS_BRACKET_O(c) (( 40 == (c)) || ( 91 == (c)) || (123 == (c)))

/// Tests for closing brackets ) ] }
#define IS_BRACKET_C(c) (((c) == 41 ) || ((c) == 93 ) || ((c) == 125))

void 
ParserState::parseNumbers(const char& c)
{
	if (token().type == Token::TYPE_SYMBOL)
	{
		// save & transl dep. on previous token
		currentGroup().addToken(*this);
		token().clear();
	}
	token().append(c);
	if (token().type != Token::TYPE_FLOAT)
	{
		token().type = Token::TYPE_INT;
	}
}

void 
ParserState::parseDecimalOp(const char& c)
{
	if (token().type != Token::TYPE_INT)
	{
		throw ErrorDecimBetwInt(curPos, 1);
	} // allow it after a symbol to begin a new float token with ?
	if (c == ',') { // is comma
		token().append('.');
	} else {
		token().append(c);
	}
	token().type = Token::TYPE_FLOAT;
}

void 
ParserState::parseAlpha(const char& c)
{
	// continue symbol token
	if (IS_ALPHA_L(c))
	{
		if (token().type == Token::TYPE_SYMBOL)
		{
			token().append(c);
		} 
		else // prev token.type not symbol
		{
			throw ErrorSymBegLowChar(curPos, 1);
		}
	} else { // (IS_ALPHA_U(c)) -> new token
		// save & transl dep. on previous token
		// create token without coef (==1.0)
		currentGroup().addToken(*this);
		token().clear();
		token().append(c);
	}
	token().type = Token::TYPE_SYMBOL;
}

void 
ParserState::parseBracketO(const char& c)
{
	ElementGroup   subGrp;
	size_t         old_pos = (curPos == 0) ? 0 : curPos-1;
	// saving work data before recursion
	ElementGroup * parentGrp = &currentGroup();
	Token        * parentToken = &token();
	currentGroup().addToken(*this);
	try {
		// call this function (recursion)
		// recLvl keeps also track of balanced brackets
		setCurrentGroup(subGrp);
		curNestingLevel--;
		curPos++;
		parse();
	} 
	catch(Error& e)
	{
		//e.addOffset(offset);
		throw;
	}
	// restoring work data
	setCurrentGroup(*parentGrp);
	setToken(*parentToken);
	// adding the result and updating current state
	currentGroup().addSubgroup(subGrp, old_pos);
	token().type = Token::TYPE_GROUP;
	token().clear();
}

void 
ParserState::parseBracketC(const char& c)
{
	curNestingLevel++;
	if (curNestingLevel > maxNestingLevel) {
		throw ErrorLoneClosingBracket(curPos, 1);
	}
}

void 
ParserState::parseSpace(const char& c)
{
	if (token().type == Token::TYPE_SYMBOL ||
	    token().type == Token::TYPE_FLOAT  ||
	    token().type == Token::TYPE_INT      )
	{
		// save & transl dep. on previous token
		currentGroup().addToken(*this);
		token().type = Token::TYPE_NONE;
		token().clear();
	}
}

void 
ParserState::parse(void)
{
	size_t thisFirstPos = (curPos == 0) ? 0 : curPos-1;
	Token token;

	setToken(token);

	if (curNestingLevel == 0) {
		throw ErrorMaxNesting(thisFirstPos, 1);
	}

	while (curPos < formula.length())
	{
		char c = formula.at(curPos);
		if (IS_NUM(c)) {
			parseNumbers(c);
		} else if (IS_DECIM(c)) {
			parseDecimalOp(c);
		} else if (IS_ALPHA(c)) {
			parseAlpha(c);
		} else if (IS_BRACKET_O(c)) {
			parseBracketO(c);
		} else if (IS_BRACKET_C(c)) {
			parseBracketC(c);
			break; // position stays @closingbracket
		} else if (IS_SPACE(c)) {
			parseSpace(c);
			// ignore
		} else {
			throw ErrorInvalidChar(curPos, 1);
		}
		curPos++;
	}
	if (curPos >= formula.length() && curNestingLevel < maxNestingLevel) {
		// does never match if last was a closing bracket 
		// (position stays at it)
		throw ErrorMissingClosingBracket(thisFirstPos, 1);
	}
	// translate the last token read in
	currentGroup().addToken(*this);
	// throws exception std::out_of_range, cfp::Error
}


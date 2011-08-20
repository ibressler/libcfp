/*
 * tests/test_auto_error.cpp
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
#include <vector>
#include <UnitTest++.h>
#include <cfp/cfp.h>

/**
 * UnitTest++ Macro extension for testing not only a specific exception type
 * but also for correct error position and length within the parsed string.
 */
#define CHECK_THROW_CUSTOM(call, err, exp_s, exp_l) \
do { \
	CHECK_THROW(call, err); \
	try { call; \
	} catch (err e) { \
		size_t start, len; \
		e.what(start, len); \
		CHECK_EQUAL((exp_s), start); \
		CHECK_EQUAL((exp_l), len); \
	} \
} while(0)

TEST(ErrorInvalidChar)
{
	cfp::Parser p;
	std::vector<std::string> strl;
	strl.push_back(" H2O"); strl.push_back("H 2O");
	strl.push_back("H2 O"); strl.push_back("H2O ");
	for(size_t i=1; i < 256; i++) // for all possible char values
	{
		if ((( 65 <= (i)) && ((i) <= 90 )) || // upper case chars
		    (( 97 <= (i)) && ((i) <= 122)) || // lower case chars
		    (( 48 <= (i)) && ((i) <= 57 )) || // numerical chars
		     ( 32 == (i))                  || // space
		     ( 44 == (i)) || ((i) == 46 )  || // decimal op , .
		     ( 40 == (i)) || ( 91 == (i))  || (123 == (i)) || // ([{
		     ((i) == 41 ) || ((i) == 93 )  || ((i) == 125))   // )]}
			continue;

		char c = (char)i;
		// check single char
		CHECK_THROW_CUSTOM(p.process(&c, 1), cfp::ErrorInvalidChar, (size_t)0, (size_t)1);
		// for all test strings
		for(size_t k=0; k < strl.size(); k++)
		{
			strl.at(k).at(k) = c; // $H2O, H$2O, H2$O, H2O$
			CHECK_THROW_CUSTOM(p.process(strl.at(k).c_str(), 
					             strl.at(k).length()),
				           cfp::ErrorInvalidChar,
			                   (size_t)k, (size_t)1);
		} // for all in strl
	} // for all possible char values
}

TEST(ErrorSymBegLowChar)
{
	cfp::Parser p;
	std::vector<std::string> strl;
	strl.push_back("H2O"); strl.push_back("H2O");
	for(size_t i=97; i <= 122; i++) // lower case chars
	{
		char c = (char)i;
		strl.at(0).at(0) = c; // a2O
		strl.at(1).at(2) = c; // H2a
		// check 1st combination
		CHECK_THROW_CUSTOM(p.process(strl.at(0).c_str(), 
		                             strl.at(0).length()), 
		                   cfp::ErrorSymBegLowChar,
		                   (size_t)0, (size_t)1);
		// check 2nd combination
		CHECK_THROW_CUSTOM(p.process(strl.at(1).c_str(), 
		                             strl.at(1).length()), 
		                   cfp::ErrorSymBegLowChar,
		                   (size_t)2, (size_t)1);
	} // for all lower case chars
}

TEST(ErrorDecimBetwInt)
{
	cfp::Parser p;
	std::vector<std::string> strl;
	strl.push_back("H.2O"); strl.push_back("H,2O");
	strl.push_back("H.O "); strl.push_back("H,O ");
//	strl.push_back("H2.O"); strl.push_back("H2,O"); not atm
	strl.push_back("  . "); strl.push_back("  , "); 
	strl.push_back("H2O."); strl.push_back("H2O,");
	for(size_t i=0; i < strl.size(); i++)
	{
		CHECK_THROW(p.process(strl.at(i).c_str(), 
		                      strl.at(i).length()), 
		            cfp::ErrorDecimBetwInt);
		// check error position & length
		try { p.process(strl.at(i).c_str(), 
		                strl.at(i).length());
		} catch (cfp::ErrorDecimBetwInt e) {
			size_t start, len;
			e.what(start, len);
			if      (i < 4) CHECK_EQUAL(start, (size_t)1);
			else if (i < 6) CHECK_EQUAL(start, (size_t)2);
			else            CHECK_EQUAL(start, (size_t)3);
			CHECK_EQUAL(len, (size_t)1);
		}
	}
}

TEST(ErrorMaxNesting)
{
	cfp::Parser p; std::string str;
	str.assign("N");
	for(size_t i=0; i < p.maxNestingLevel(); i++) str.append("(");
	str.append("H2O");
	for(size_t i=0; i < p.maxNestingLevel(); i++) str.append(")");
	str.append("3");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorMaxNesting, p.maxNestingLevel(), (size_t)1);
}

TEST(ErrorStartWithCoef)
{
	cfp::Parser p; std::string str;
	str.assign("3.2H");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorStartWithCoef, (size_t)0, (size_t)1);
	str.assign("H(3.4)O");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorStartWithCoef, (size_t)2, (size_t)1);
}

TEST(ErrorLoneNucleonNum)
{
	cfp::Parser p; std::string str;
//	str.assign("H2(3)"); // allowed ?
//	CHECK_THROW(p.process(str.c_str(), str.length()), cfp::ErrorLoneNucleonNum);
	str.assign("H2(3(D4))");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorLoneNucleonNum, (size_t)3, (size_t)1);
	str.assign("3(H3.4)O");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorLoneNucleonNum, (size_t)0, (size_t)1);
}

TEST(ErrorLoneClosingBracket)
{
	cfp::Parser p; std::string str;
	str.assign(" )");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorLoneClosingBracket, (size_t)1, (size_t)1);
	str.assign("H2)");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorLoneClosingBracket, (size_t)2, (size_t)1);
	str.assign("H2(D4))3");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorLoneClosingBracket, (size_t)6, (size_t)1);
}

TEST(ErrorMissingClosingBracket)
{
	cfp::Parser p; std::string str;
	str.assign("((((H)))");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorMissingClosingBracket, (size_t)0, (size_t)1);
	str.assign("JeH(N(S(D)O)");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorMissingClosingBracket, (size_t)3, (size_t)1);
	str.assign("SdFeH(");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorMissingClosingBracket, (size_t)5, (size_t)1);
	str.assign("Sd(Ju(N(S)H(D(H))O)Fe");
	CHECK_THROW_CUSTOM(p.process(str.c_str(), str.length()), 
	                   cfp::ErrorMissingClosingBracket, (size_t)2, (size_t)1);
}


/*
 * tests/test_auto_parser.cpp
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
 * Ingo Bressler (ingo@cs.tu-berlin.de)
 */

#include <iostream>
#include <sstream>
#include <UnitTest++.h>
#include <cfp/cfp.h>

TEST(ParserConstructor)
{
	std::cerr << "TEST(ParserConstructor) start" << std::endl;
	cfp::Parser p;
	CHECK(p.formula().empty());
	CHECK(p.empirical().empty());
	p.process();
	CHECK(p.empirical().empty());
	CHECK(p.formula().empty());

	cfp::Parser p1("foo", 3);
	CHECK_EQUAL(0, p1.formula().compare("foo"));
	CHECK_EQUAL((size_t)3, p1.formula().length());
	std::cerr << "TEST(ParserConstructor) stop" << std::endl;
}

TEST(ParserGetterSetterCopyConstructor)
{
	std::cerr << "TEST(ParserGetterSetterCopyConstructor) start" << std::endl;
	cfp::Parser p;
	p.setFormula("foobar", 6);
	CHECK_EQUAL(0, p.formula().compare("foobar"));
	CHECK_EQUAL((size_t)6, p.formula().length());
	p.setFormula(std::string("H2O"));
	CHECK_EQUAL(0, p.formula().compare("H2O"));
	CHECK_EQUAL((size_t)3, p.formula().length());
	p.setMaxNestingLevel(46);
	CHECK_EQUAL((size_t)46, p.maxNestingLevel());
	CHECK(p.empirical().empty());

	cfp::Parser p1(p);
	CHECK_EQUAL(0, p1.formula().compare("H2O"));
	CHECK_EQUAL((size_t)3, p1.formula().length());
	CHECK_EQUAL((size_t)46, p1.maxNestingLevel());
	CHECK(p1.empirical().empty());
	std::cerr << "TEST(ParserGetterSetterCopyConstructor) stop" << std::endl;
}

#define TEST_FORMULA_1 "K3 (4J)2.3 (13C)1.2"
TEST(ParserProcessingRegular1)
{
	std::cerr << "TEST(ParserProcessingRegular1) start" << std::endl;
	cfp::Parser p("K3(J(4))2.3(13C)1.2",19);
	p.process();
	std::cerr << "TEST(ParserProcessingRegular1), process done" << std::endl;
	CHECK_EQUAL((size_t)3, p.empirical().size());
	std::cerr << "TEST(ParserProcessingRegular1), empirical1" << std::endl;
	std::stringstream ss;
	ss << p.empirical();
	std::cerr << "TEST(ParserProcessingRegular1), empirical2" << std::endl;
	CHECK_EQUAL(0, ss.str().compare("(13C)1.2 (4J)2.3 K3"));
	std::cerr << "TEST(ParserProcessingRegular1) stop" << std::endl;
}

#define TEST_FORMULA_2 "H2.3O"
TEST(ParserProcessingRegular2)
{
	std::cerr << "TEST(ParserProcessingRegular2) start" << std::endl;
	cfp::Parser p(TEST_FORMULA_2, sizeof(TEST_FORMULA_2)-1);
	p.process();
	CHECK_EQUAL((size_t)2, p.empirical().size());
	std::stringstream ss;
	ss << p.empirical();
	CHECK_EQUAL(0, ss.str().compare("H2.3 O"));
	std::cerr << "TEST(ParserProcessingRegular2) stop" << std::endl;
}

#define TEST_FORMULA_3 "H2,3O"
TEST(ParserProcessingRegular3)
{
	std::cerr << "TEST(ParserProcessingRegular3) start" << std::endl;
	cfp::Parser p(TEST_FORMULA_3, sizeof(TEST_FORMULA_3)-1);
	p.process();
	CHECK_EQUAL((size_t)2, p.empirical().size());
	std::stringstream ss;
	ss << p.empirical();
	CHECK_EQUAL(0, ss.str().compare("H2.3 O"));
	std::cerr << "TEST(ParserProcessingRegular3) stop" << std::endl;
}

#define TEST_FORMULA_4 "1H5 1H7"
TEST(ParserProcessingRegular4)
{
	std::cerr << "TEST(ParserProcessingRegular4) start" << std::endl;
	cfp::Parser p(TEST_FORMULA_4, sizeof(TEST_FORMULA_4)-1);
	p.process();
	CHECK_EQUAL((size_t)1, p.empirical().size());
	std::stringstream ss;
	ss << p.empirical();
	CHECK_EQUAL(0, ss.str().compare("(1H)12"));
	std::cerr << "TEST(ParserProcessingRegular4) stop" << std::endl;
}

#define TEST_FORMULA_5 "1H 5 H7"
TEST(ParserProcessingRegular5)
{
	std::cerr << "TEST(ParserProcessingRegular5) start" << std::endl;
	cfp::Parser p(TEST_FORMULA_5, sizeof(TEST_FORMULA_5)-1);
	p.process();
	CHECK_EQUAL((size_t)2, p.empirical().size());
	std::stringstream ss;
	ss << p.empirical();
	CHECK_EQUAL(0, ss.str().compare("H7 (1H)5"));
	std::cerr << "TEST(ParserProcessingRegular5) stop" << std::endl;
}

/* internal datastructures (std::map) don't preserve order (sort lexically instead)
TEST(ParserProcessingLoop)
{
	cfp::Parser p(TEST_FORMULA_1, sizeof(TEST_FORMULA_1)-1);
	p.process();

	CHECK_EQUAL((size_t)3, p.empirical().size());
	std::stringstream ss3;
	ss3 << p.empirical();
	CHECK_EQUAL(0, ss3.str().compare(TEST_FORMULA_1));
}
*/

TEST(ParserProcessingComplex)
{
	std::cerr << "TEST(ParserProcessingComplex) start" << std::endl;
	cfp::Parser p("K3(J(4)2(J4(K2.2(F3J))3F2.3))2.5",32);
	p.process();
	CHECK_EQUAL((size_t)4, p.empirical().size());
	std::stringstream ss2;
	ss2 << p.empirical();
	CHECK_EQUAL(0, ss2.str().compare("F28.25 J17.5 (4J)5 K19.5"));
	CHECK_EQUAL(0, p.toMarkup().compare("K<sub>3</sub>(<sup>4</sup>J<sub>2</sub>(J<sub>4</sub>(K<sub>2.2</sub>(F<sub>3</sub>J))<sub>3</sub>F<sub>2.3</sub>))<sub>2.5</sub>"));
	std::cerr << "TEST(ParserProcessingComplex) stop" << std::endl;
}


/*
 * tests/test_auto_element.cpp
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

#include <UnitTest++.h>
#include <cfp/cfp.h>

TEST(ElementConstructor)
{
	cfp::CompoundElement e;
	CHECK_EQUAL(1.0, e.coefficient());
	CHECK(!e.isIsotope());
	CHECK(e.symbol().empty());

	cfp::CompoundElement g;
	g.setCoefficient(4.4);
	CHECK_EQUAL(4.4, g.coefficient());
}

#define CHECK_ELEMENT_VALUES(e) \
do { \
	CHECK_EQUAL(5.5, (e).coefficient()); \
	CHECK_EQUAL(0, (e).symbol().compare("foo")); \
	CHECK_EQUAL(3, (e).nucleons()); \
	CHECK((e).isIsotope()); \
} while(0)

TEST(ElementGetterSetterCopyConstructor)
{
	cfp::CompoundElement e;
	e.setCoefficient(5.5);
	e.setSymbol("foo");
	e.setNucleons(3);
	CHECK_ELEMENT_VALUES(e);

	cfp::CompoundElement g(e);
	CHECK_ELEMENT_VALUES(g);
}

TEST(ElementComparison)
{
	cfp::CompoundElement e1;
	e1.setCoefficient(2.0);
	cfp::CompoundElement e2;
	e1.setCoefficient(3.0);
	CHECK(!(e1 < e2));
	e1.setSymbol("Aa");
	e2.setSymbol("Aa");
	CHECK(!(e1 < e2));
	e2.setNucleons(2);
	CHECK(e1 < e2);
	e1.setSymbol("Aa");
	e2.setSymbol("Bb");
	CHECK(e1 < e2);
	e1.setNucleons(2);
	CHECK(e1 < e2);
}


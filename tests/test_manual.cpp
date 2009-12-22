/*
 * tests/test_manual.cpp
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
#include <cfp/cfp.h>

// A simple test and usage example
int main (int argc, char * argv[])
{
	const int   maxlen = 256;
	char        formula[maxlen];
	cfp::Parser p;

	std::cout << "enter chemical formula: ";
	std::cin.getline(formula, maxlen, '\n');

	std::cout << "you entered:           '" << formula << "'" << std::endl;
	int len = strlen(formula);
	if (len > maxlen) len = maxlen;
	p.process(formula, len);

	const cfp::Compound& el = p.empirical();
	std::cout << "empirical formula:     '" << el << "'" << std::endl;
	std::cout << "markup element list:   '" << toMarkup(el) << "'" << std::endl;
	std::cout << "markup parsed formula: '" << p.toMarkup() << "'" << std::endl;

	return 1;
}

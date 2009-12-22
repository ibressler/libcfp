/*
 * src/elementgroup.cpp
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

#include <set>
#include "parserstate.h"

using namespace cfp;

std::vector<ElementGroup::ParseFunc> 
	ElementGroup::mAddTokenFunctions;

ElementGroup::ElementGroup()
	: mLastElementProperty(NO_PROPERTY)
{
	if (mAddTokenFunctions.size() != Token::TYPE_NONE+1)
	{
		mAddTokenFunctions.clear();
		mAddTokenFunctions.push_back(&ElementGroup::addTokenSymbol);
		mAddTokenFunctions.push_back(&ElementGroup::addTokenGroup);
		mAddTokenFunctions.push_back(&ElementGroup::addTokenInt);
		mAddTokenFunctions.push_back(&ElementGroup::addTokenFloat);
		mAddTokenFunctions.push_back(&ElementGroup::addTokenNone);
	}
}

size_t
ElementGroup::size() const 
{
	return mF.size();
}

bool 
ElementGroup::empty() const
{
	return mF.empty();
}

void 
ElementGroup::clear()
{
	mF.clear();
	mList.clear();
}

CompoundGroupElement&
ElementGroup::back()
{
	return mF.back();
}

const CompoundGroupElement&
ElementGroup::back() const
{
	return mF.back();
}

void 
ElementGroup::add(const CompoundGroupElement& e)
{
	mF.insert(mF.end(), e);
}

void 
ElementGroup::addGroup(const ElementGroup& eg)
{
	fiterator i = adobe::trailing_of(
	                mF.insert(mF.end(), CompoundGroupElement(1.0, true)));
	mF.insert(i, adobe::child_begin(eg.mF.end()),
	              adobe::child_end(eg.mF.end()));
}

const Compound&
ElementGroup::flatList(void) const
{
	return mList;
}

// set of elements without group feature (for outside use)
typedef std::set<CompoundElement> Set;

void 
addToSet(Set& s, CompoundElement& e)
{
	Set::iterator it = s.find(e);
	if (it != s.end()) 
	{
		double coef = it->coefficient();
		e.setCoefficient(coef + e.coefficient());
		// we must not modify the element in the set
		s.erase(it);
	} 
	bool success = s.insert(e).second;
	if (!success) {
		// what to do ? at least tell about it.
		std::cerr << "Could not insert into std::map!" << std::endl;
	}
}

void 
ElementGroup::flatten(void)
{
	std::vector<double> coef_stack;
	Set set;

	mList.clear();
	diterator f = diterator(mF.begin());
	diterator l = diterator(mF.end());
	while(f != l) 
	{
		// ignore group nodes add multiply their coef
		if (f->isGroup()) 
		{
			size_t depth = 0;
			if (f.depth() > 0) depth = f.depth();
			if (coef_stack.size() > depth)
			{
				coef_stack.pop_back();
			} else {
				double prev_coef = 1.0;
				if (!coef_stack.empty()) {
					prev_coef = coef_stack.back();
				}
				coef_stack.push_back( prev_coef * f->coefficient() );
			}
		} else { // add elements and multiply their coefficients 
			 // with accumulated group coefficients
			double coef = 1.0;
			if (!coef_stack.empty()) {
				coef = coef_stack.back();
			}
			CompoundElement e(*f);
			e.setCoefficient( e.coefficient() * coef );
			addToSet(set, e);
			f = adobe::trailing_of(f);
		}
		f++;
	}
	copy(set.begin(), set.end(), back_inserter(mList));
}

std::ostream& 
std::operator<<(std::ostream& o, const cfp::ElementGroup& eg)
{
	ElementGroup::forest::const_iterator first = eg.mF.begin();
	ElementGroup::forest::const_iterator last = eg.mF.end();
	while( first != last )
	{
		if (first.edge() == adobe::forest_leading_edge)
		{
			if (first->isGroup()) 
				o << "(";
			else 
				o << first->toMarkup();
		}
		else
		{
			if (first->isGroup()) {
				o << ")";
				if (first->coefficient() != 1.0) 
					o << "<sub>" << first->coefficient() << "</sub>";
			}
		}
		first++;
	}
	return o;
}


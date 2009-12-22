/*
 * src/elementgroup.h
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

#ifndef CFP_ELEMENTGROUP_H
#define CFP_ELEMENTGROUP_H

#include <iostream>
#include <vector>
#include <adobe/forest.hpp>

#include <cfp/cfp.h>
#include "element.h"

namespace cfp {
	class ElementGroup;
}

namespace std {
	/**
	 * Writes the string representation of an ElementGroup to an output stream.
	 * Returns the string representation of this ElementGroup
	 * with markup text formatting (\<sup\>, \<sub\>).
	 * \param[in,out] o  Output stream to write to.
	 * \param[in]     eg ElementGroup to create a string representation from.
	 * \returns          The output stream o.
	 * \relatesalso cfp::Token
	 */
	std::ostream& 
	operator<<(std::ostream& o, const cfp::ElementGroup& eg);
}

namespace cfp
{
	struct Token;

	/// The internal result of the parsing process.
	/// A hierachical data structure based on adobe::forest.
	/// It represents the input formula string exactly.
	/// Because we don't want to introduce external dependencies to
	/// the <em>Adobe Source Library</em>, the result is converted to a
	/// flat empirical element list ( flatten() ).
	/// Alternative result types are possible too, of course, but not yet
	/// implemented.
	class ElementGroup
	{
	private:
		/// Tree structure to represent a formula.
		typedef adobe::forest<CompoundGroupElement> forest;

		/// Regular iterator.
		typedef forest::iterator                    fiterator;

		/// Iterator with (tree) depth counter.
		typedef adobe::depth_fullorder_iterator< 
		                boost::range_const_iterator< 
		                forest >::type >            diterator;

		/// The prototype of the function directly related to parsing.
		/// \see ElementGroup(), parse_details.cpp
		typedef void (ElementGroup::*ParseFunc) (ParserState& s);
	public:
		ElementGroup(); //!< Creates an empty group.

		/// Returns the number of elements including group descriptors.
		size_t 
		size() const;

		/// Clears internal data structures.
		void 
		clear();
		
		/// Adds the current Token data to this element group.
		/// Eventually, creates a new CompoundElement from it or adds its data
		/// to the previous element.
		void 
		addToken(ParserState& s);

		/// Adds another element group.
		/// Some parsing related decisions are made here regarding
		/// nucleon numbers, respectively isotopes which are distinct
		/// element groups formally.
		/// \sa addGroup
		void 
		addSubgroup(const ElementGroup& subGrp, size_t strIdx);

		/// Returns the flat empirical formula representation.
		const Compound& 
		flatList(void) const;

		/// Converts the tree structure into a flat element list.
		void 
		flatten(void);

		friend std::ostream& 
		std::operator<<(std::ostream& o, const ElementGroup& eg);
	private:
		bool 
		empty() const; //!< Returns true, if this group contains no elements.

		CompoundGroupElement& 
		back();        //!< Returns the last element added.

		const CompoundGroupElement& 
		back() const;  //!< Returns the last element added.

		void 
		add(const CompoundGroupElement& e); //!< Adds a new element to the tree.

		/// Adds another element group. It gets hierarchical here.
		/// Final layout:
		/// \code
		/// [ <e1>  <e2>  <e3>  <g> ]         <= this element group
		///                      |
		///             [ <ee1> <ee2> <ee3> ] <= subGrp
		///
		/// <e*[0-9]+> are regular elements
		/// <g> is a CompoundGroupElement with isGroup==true
		/// \endcode
		void addGroup(const ElementGroup& eg);

		void addTokenSymbol(ParserState& s);//!< Handles a symbol token.
		void addTokenInt(ParserState& s);   //!< Handles a natural number token.
		void addTokenFloat(ParserState& s); //!< Handles a real number token.
		void addTokenGroup(ParserState& s); //!< Handles a group token (does nothing).
		void addTokenNone(ParserState& s);  //!< Handles a undefined token (does nothing).
	private:
		/// Tree data structure containing all CompoundGroupElement objects.
		forest   mF;

		/// Flat empirical element list.
		Compound mList;

		/// Array of Functions to handle different Tokens based on
		/// their type.
		static std::vector<ParseFunc> mAddTokenFunctions;

		/// Property of an element which was set last during
		/// formula parsing.
		typedef enum {
			SYMBOL_PROPERTY,
			NUCLEON_PROPERTY,
			COEFFICIENT_PROPERTY,
			GROUP_PROPERTY,
			NO_PROPERTY
		} LastElementProperty;

		/// Remembers the last property which was set during formula
		/// parsing. Used to tell adjacent nucleon and coefficient
		/// numbers apart.
		LastElementProperty mLastElementProperty;
	};
} // namespace cfp

#endif


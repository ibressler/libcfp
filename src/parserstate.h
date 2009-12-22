/*
 * src/parserstate.h
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

#ifndef CFP_PARSERSTATE_H
#define CFP_PARSERSTATE_H

#include <string>
#include "token.h"
#include "elementgroup.h"

namespace cfp
{
	/// Implementation data of a Parser.
	/// Handling and processing of the input string is done here.
	/// It is also a container for all data structures required during
	/// the parse process.
	class ParserState
	{
	public:
		ParserState(); //!< Initializes the data required for parsing.

		/// Prepares for parsing a new formula. Removes all remaining
		/// data related to the previous formula
		void 
		reset(const char * formula, const size_t len);

		/// Continues processing the current formula at this state.
		/// Is called recursively with updated data pointers.
		/// \todo Categorization of characters is done by conditional
		/// tests (if-else). An alternative would be a global, i.e.
		/// static, lookup table (std::vector or array) containing the
		/// appropriate handler function pointers for (125-40=85) 
		/// relevant elements. Performance improvement or overkill ?
		/// Security issues ?
		void 
		parse(void);

		/// Updates the pointer to the current Token to use.
		void 
		setToken(Token & t);

		/// Returns the current Token used for parsing.
		Token & 
		token(void);

	private:
		/// Updates the pointer to the current result group to use.
		void 
		setCurrentGroup(ElementGroup & eg);

		/// Returns the current result group used for parsing.
		ElementGroup &
		currentGroup(void);

		void parseNumbers(const char& c);  //!< Handles numerical characters.
		void parseDecimalOp(const char& c);//!< Handles decimal operators.
		void parseAlpha(const char& c);    //!< Handles Alphabetical characters.
		void parseBracketO(const char& c); //!< Handles opening parentheses.
		void parseBracketC(const char& c); //!< Handles closing parentheses.
		void parseSpace(const char& c);    //!< Handles ignored characters.

	public:
		size_t         maxNestingLevel; //!< Maximum nesting level for element groups.
		size_t         curNestingLevel; //!< Current nesting level during parsing.
		size_t         curPos;          //!< Current position within the formula.
		std::string    formula;         //!< Complete formula.
		ElementGroup   rootGroup;       //!< Hierarchical result structure.
	private:
		ElementGroup * mCurGroup; //!< Result structure for the current nesting level.
		Token *        mToken;    //!< Token for the current nesting level.
	};
} // namespace cfp

#endif // this file

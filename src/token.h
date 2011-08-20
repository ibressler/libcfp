/*
 * src/token.h
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

#ifndef CFP_TOKEN_H
#define CFP_TOKEN_H

#include <string>

namespace cfp
{
	/// A group of adjacent characters with a special meaning within a
	/// formula string.
	class Token
	{
	public:
		/// The type of a Token within a formula.
		/// \sa std::operator<<(std::ostream&, const cfp::Token::Type&)
		typedef enum
		{
			TYPE_SYMBOL, //!< An element symbol.
			TYPE_GROUP,  //!< A group characterized by parentheses.
			TYPE_INT,    //!< A natural number.
			TYPE_FLOAT,  //!< A real number.
			TYPE_NONE    //!< Nothing/Invalid.
		} Type;

		/// Creates a Token with Token::TYPE_NONE.
		Token();

		/// Appends a single character.
		/// \param[in] c Character to append.
		void 
		append(char c);

		/// Returns the length of this Token.
		/// \returns Its length.
		size_t 
		length() const;

		/// Clears this Token so that length() returns 0.
		void 
		clear();

		/// Returns the string representation of this Token.
		/// \returns A string consisting of all characters.
		const std::string& 
		toString() const;

		/// Returns the integer representation of this Token.
		/// \returns This Token converted to an integer number.
		int 
		toInt() const;

		/// Return the floating point representation of this Token.
		/// \returns This Token converted to a floating point number.
		double toDouble() const;

		Type        type; //!< Its type. \sa Token::Type
	private:
		std::string mStr; //!< The characters this token consists of.
	};

} // namespace cfp

namespace std 
{
	/**
	 * Writes the string representation of a Token::Type to an output stream.
	 * \param[in,out] o Output stream to write to.
	 * \param[in]     t Token::Type to create a string representation from.
	 * \returns         The output stream o.
	 */
	std::ostream& operator<<(std::ostream& o, const cfp::Token::Type& t);

	/**
	 * Writes the string representation of a Token to an output stream.
	 * \param[in,out] o Output stream to write to.
	 * \param[in]     t Token to create a string representation from.
	 * \returns         The output stream o.
	 * \relatesalso cfp::Token
	 */
	std::ostream& operator<<(std::ostream& o, const cfp::Token& t);
}


#endif // this file

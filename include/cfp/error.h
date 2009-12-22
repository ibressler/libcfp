/*
 * cfp/error.h
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

#ifndef CFP_ERROR_H
#define CFP_ERROR_H

#include <exception>
#include <string>

namespace cfp
{

	/// Parse error base class.
	class Error: public std::exception
	{
	public:
		/**
		 * Creates a common parse error without a message.
		 * \param[in] start Start position of the erroneous section in the parsed
		 *              string.
		 * \param[in] length Length of the erroneous section in the parsed string.
		 */
		explicit
		Error(size_t start, size_t length);

		/// Destructor. 
		/// Does nothing because there was no data created on the heap.
		virtual 
		~Error(void) throw();

		/**
		 * Provides the general cause of the current error.
		 * \returns A C-style character string describing the cause.
		 * \sa whatStr()
		 */
		virtual const char*
		what(void) const throw();

		/**
		 * Provides the general cause of the current error.
		 * \returns A string describing the cause.
		 * \sa what()
		 */
		virtual const std::string&
		whatStr(void) const throw();

		/**
		 * Provides the general cause of the current error.
		 * \param[out] start First position of the erroneous section in the 
		 *                   parsed string.
		 * \param[out] length Length of the erroneous section in the parsed
		 *                    string.
		 * \returns A C-style character string describing the cause
		 * \sa whatStr(size_t&, size_t&)
		 */
		virtual const char*
		what(size_t& start, size_t& length) const throw();

		/**
		 * Provides the general cause of the current error.
		 * \param[out] start First position of the erroneous section in the 
		 *                   parsed string.
		 * \param[out] length Length of the erroneous section in the parsed
		 *                    string.
		 * \returns A string describing the cause.
		 * \sa what(size_t&, size_t&)
		 */
		virtual const std::string&
		whatStr(size_t& start, size_t& length) const throw();

	protected:
		/**
		 * Creates a parse error with a message.
		 * \param[in] msg Description of this error.
		 * \param[in] start Start position of the erroneous section in the parsed
		 *              string.
		 * \param[in] length Length of the erroneous section in the parsed string.
		 */
		explicit
		Error(const std::string& msg, size_t start, size_t length);

		/**
		 * Sets the message of this error.
		 * \param msg The message string.
		 */
		void setMessage(const std::string& msg);

	private:
		std::string msg_m;    //!< Description of this error.
		size_t      start_m;  //!< First position of the erroneous section.
		size_t      length_m; //!< Length of the erroneous section.
	};


	/// Error for invalid characters.
	class ErrorInvalidChar: public Error
	{
	public:
		explicit ErrorInvalidChar(size_t start, size_t length)
			: Error("Invalid Character !", 
			        start, length)
		{}
	};

	/// Error for a symbol beginning with a lower case character.
	class ErrorSymBegLowChar: public Error
	{
	public:
		explicit ErrorSymBegLowChar(size_t start, size_t length)
			: Error("Symbols must not begin with lower case characters !", 
				start, length)
		{}
	};

	/// Error for a decimal operator not being between two integer characters.
	/// \todo Review the handling of decimal operators. Should it be allowed to
	///       omit the numbers before or after the decimal operator ? Are there
	///       possible ambiguities regarding the dot ?
	class ErrorDecimBetwInt: public Error
	{
	public:
		explicit ErrorDecimBetwInt(size_t start, size_t length)
			: Error("Decimal operator only between numerical characters allowed !",
				start, length)
		{}
	};


	/// Error for the maximum nesting level of groups.
	class ErrorMaxNesting: public Error
	{
	public:
		explicit ErrorMaxNesting(size_t start, size_t length)
			: Error("Maximum nesting level reached !",
				start, length)
		{}
	};


	/// Error for a group starting with a coefficient (floating point value).
	class ErrorStartWithCoef: public Error
	{
	public:
		explicit ErrorStartWithCoef(size_t start, size_t length)
			: Error("Expression must not begin with floating point value !",
				start, length)
		{}
	};


	/// Error for a nucleon number without associated element (could not be found).
	/// \todo Should the handling of \e H2(3) be allowed ? 
	/// (instead of \e H(3)2 for isotopes)
	class ErrorLoneNucleonNum: public Error
	{
	public:
		explicit ErrorLoneNucleonNum(size_t start, size_t length)
			: Error("Nucleon number specified without preceding Element symbol !",
				start, length)
		{}
	};


	/// Error for a closing bracket without matching opening bracket.
	class ErrorLoneClosingBracket: public Error
	{
	public:
		explicit ErrorLoneClosingBracket(size_t start, size_t length)
			: Error("Encountered closing bracket without preceding opening bracket !", 
				start, length)
		{}
	};


	/// Error for an opening bracket without matching closing bracket.
	class ErrorMissingClosingBracket: public Error
	{
	public:
		explicit ErrorMissingClosingBracket(size_t start, size_t length)
			: Error("Missing closing bracket !", 
				start, length)
		{}
	};

} // namespace cfp

#endif


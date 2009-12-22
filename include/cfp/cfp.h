/*
 * cfp/cfp.h
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

#ifndef CHEM_FORMULA_PARSER_H
#define CHEM_FORMULA_PARSER_H

#include <list>
#include <cfp/error.h>

/**
 * Contains all functions and utilities which are provided by this library for
 * parsing of chemical formulas in ASCII notation.
 */
namespace cfp
{
	/**
	 * Representation of a generic chemical element.
	 * It consists of:
	 * - A symbol which identifies the chemical element it represents.
	 *   \sa setSymbol, symbol
	 * - A nucleon number (also called <em>atomic mass number</em>) which
	 *   identifies this element as a specific isotope.
	 *   The default nucleon number is the same (invalid) number for all 
	 *   elements ( naturalNucleonNr() ).
	 *   \sa setNucleon, nucleon, isIsotope
	 */
	class ChemicalElementInterface
	{
	public:
		/// Returns the default nucleon number.
		/// It is assigned to all elements which are considered to 
		/// be \b not an isotope but a natural element which consists
		/// of a mixture of stable isotopes. It is an invalid number 
		/// (negative).
		/// \sa setNucleon, nucleon, isIsotope
		static const int 
		naturalNucleonNr();

		virtual 
		~ChemicalElementInterface(); //!< Destructor.

		/// Returns the symbol.
		/// \sa setSymbol
		std::string 
		symbol(void) const;

		/// Sets the symbol.
		/// \param[in] s The new symbol of the element.
		/// \sa symbol
		void 
		setSymbol(const std::string& s);

		/// Returns the nucleon number.
		/// \sa setNucleon, isIsotope, naturalNucleonNr
		int 
		nucleons(void) const;

		/// Sets the nucleon number.
		/// \param[in] i The new nucleon number.
		/// \sa nucleon, isIsotope, naturalNucleonNr
		void 
		setNucleons(int i);

		/// Identifies an CompoundElement as isotope or natural element.
		/// \returns True, if this element is an isotope. False,
		///          if it is a regular element.
		/// \sa setNucleon, nucleon, naturalNucleonNr
		bool 
		isIsotope(void) const;

		/// Unique string representation of this element.
		/// Same as toString() but will not be overridden by
		/// subclasses by default.
		/// \sa toString()
		std::string 
		uniqueName(void) const;

		/// String representation of this element.
		/// Builds a string with the symbol preceded by the
		/// nucleon number (if it is an isotope).
		std::string 
		toString(void) const;

		/// String representation with markup text
		/// formatting. This includes \<sup\> for the nucleon number
		/// and \<sub\> for the coefficient value.
		/// \returns The formatted string.
		std::string 
		toMarkup(void) const;

		/// Compares two elements lexically.
		/// First, it compares the names of both elements. If the
		/// names are identical, the element with a lower nucleon 
		/// number is \e less than the other element.
		/// \param[in] e The element to compare this element with.
		/// \returns     True, if this element is \e less than the
		///              other provided element. False, otherwise.
		bool 
		operator<(const ChemicalElementInterface& e) const;

		/// Copies element data.
		/// \param[in] e The element to copy data from.
		/// \returns A reference to \e this element.
		ChemicalElementInterface& 
		operator=(const ChemicalElementInterface& e);

	protected:
		/// Unique name implementation for a generic element.
		virtual std::string 
		doUniqueName(void) const;

		/// String (ASCII) conversion for a generic element.
		virtual std::string 
		doToString(void) const;

		/// Formatted string (HTML) conversion for a generic element.
		virtual std::string 
		doToMarkup(void) const;
	private:
		/// Data read access for the symbol.
		virtual std::string 
		doSymbol(void) const = 0;

		/// Data write access for the symbol.
		virtual void 
		doSetSymbol(const std::string& symbol) = 0;

		/// Data read access for the nucleon number.
		virtual int 
		doNucleons(void) const = 0;

		/// Data write access for the nucleon number.
		virtual void 
		doSetNucleons(int i) = 0;
	};

	/**
	 * Representation of a chemical element within a compound.
	 * Besides of the properties a ChemicalElementInterface element
	 * consists of, this one has an additional property:
	 * - A coefficient value to specify the amount of this element in the
	 *   overall compound, i.e. formula (as in \e H<sub>2</sub>).
	 *   \sa setCoef, coef
	 */
	class CompoundElementInterface: public ChemicalElementInterface
	{
	public:
		virtual 
		~CompoundElementInterface(); //!< Destructor.

		/// Sets the coefficient value.
		/// \param[in] coefficient The new coefficient value.
		/// \sa coef
		void 
		setCoefficient(double coefficient);

		/// Returns the coefficient value.
		/// \sa setCoef
		double 
		coefficient(void) const;

		/// \see ChemicalElementInterface::operator=
		CompoundElementInterface& 
		operator=(const CompoundElementInterface& e);

	protected:
		/// String representation of this element.
		/// Builds a string with the symbol preceded by the
		/// nucleon number (if it is an isotope) and followed by the
		/// compound coefficient.
		virtual std::string 
		doToString(void) const;

		/// String representation of this CompoundElement with markup text
		/// formatting. This includes \<sup\> for the nucleon number
		/// and \<sub\> for the coefficient value.
		/// \returns The formatted string.
		virtual std::string 
		doToMarkup(void) const;
	private:
		/// Data read access for coefficient value.
		virtual double 
		doCoefficient(void) const = 0;

		/// Data write access for coefficient value.
		virtual void 
		doSetCoefficient(double coefficient) = 0;
	};

	struct ChemicalElementData; //!< Implementation data structure.
	struct CompoundElementData; //!< Implementation data structure.

	/**
	 * A concrete chemical element descriptor implementation.
	 * \see ChemicalElementInterface
	 */
	class ChemicalElement: public ChemicalElementInterface
	{
	public:
		ChemicalElement(); //!< Default constructor.
		ChemicalElement(const ChemicalElement& eb); //!< Copy constructor.
		/// Copy constructor for related elements.
		explicit 
		ChemicalElement(const ChemicalElementInterface& eb);

		virtual 
		~ChemicalElement(); //!< Destructor.
		
		/// Uses generic implementation from base class.
		using ChemicalElementInterface::operator=;
	private:
		/// Specific implementation of ChemicalElementInterface::doSymbol.
		virtual std::string 
		doSymbol(void) const;

		/// Specific implementation of ChemicalElementInterface::doSetSymbol.
		virtual void 
		doSetSymbol(const std::string& symbol);

		/// Specific implementation of ChemicalElementInterface::doNucleons.
		virtual int 
		doNucleons(void) const;

		/// Specific implementation of ChemicalElementInterface::doSetNucleons.
		virtual void 
		doSetNucleons(int i);
	private:
		ChemicalElementData * mD; //!< Implementation data.
	};

	/**
	 * A concrete compound element descriptor implementation.
	 * \see CompoundElementInterface
	 */
	class CompoundElement: public CompoundElementInterface
	{
	public:
		CompoundElement(); //!< Default constructor.
		CompoundElement(const CompoundElement& e); //!< Copy constructor.

		/// Copy constructor for related elements.
		explicit 
		CompoundElement(const CompoundElementInterface& e);

		virtual 
		~CompoundElement(); //!< Destructor.

		/// Uses generic implementation from base class.
		using CompoundElementInterface::operator=;

	private:
		/// Specific implementation of ChemicalElementInterface::doSymbol.
		virtual std::string 
		doSymbol(void) const;

		/// Specific implementation of ChemicalElementInterface::doSetSymbol.
		virtual void 
		doSetSymbol(const std::string& symbol);

		/// Specific implementation of ChemicalElementInterface::doNucleons.
		virtual int 
		doNucleons(void) const;

		/// Specific implementation of ChemicalElementInterface::doSetNucleons.
		virtual void 
		doSetNucleons(int i);

		/// Specific implementation of CompoundElementInterface::doCoefficient.
		virtual double 
		doCoefficient(void) const;

		/// Specific implementation of CompoundElementInterface::doSetCoefficient.
		virtual void 
		doSetCoefficient(double coefficient);
	private:
		CompoundElementData * mD; //!< Implementation data.
	};

	/// The result type of parsing a formula. 
	/// \sa std::operator<<(std::ostream&, const cfp::Compound&)
	typedef std::list<CompoundElement> Compound;

	class ParserState; //!< Parser implementation data structure.

	/**
	 * Parsing takes place here.
	 * - For input it takes the chemical formula in ASCII notation. 
	 *   - Parser(const char *, const size_t), 
	 *   - setFormula(const char *, const size_t), 
	 *   - setFormula(const std::string&), 
	 *   - process(const char *, const size_t)
	 *
	 * - For output it provides a list of CompoundElement objects which represents
	 *   the empirical chemical formula (each element or isotope of an
	 *   element occurs only once)
	 *   - process(const char *, const size_t),
	 *   - empirical()
	 */
	class Parser
	{
	public:
		/// Default constructor.
		Parser(void);

		/// Constructor.
		/// \param[in] formula Chemical formula in ASCII notation.
		/// \param[in] len     Number of characters in \e formula.
		Parser(const char * formula, const size_t len);

		/// Copy constructor.
		/// \param[in] p Parser to copy all data from.
		Parser(const Parser& p);

		~Parser(); //!< Destructor.

		/// Sets the input formula of this Parser.
		/// \param[in] formula The new input formula C-style string.
		/// \param[in] len     Number of characters in \e formula.
		void setFormula(const char * formula, const size_t len);

		/// Sets the input formula of this Parser.
		/// <em>Provided for convenience.</em>
		/// \param[in] formula The new input formula string.
		void setFormula(const std::string& formula);

		/// Returns the current formula of this Parser.
		const std::string& formula() const;

		/// Parses the current formula of this Parser.
		/// The result can be obtained by calling empirical().
		/// \note May throw a cfp::Error or any of its sub-classes.
		/// \sa process(const char *, const size_t)
		void process(void);

		/// Parses the given formula and returns the result.
		/// <em>Provided for convenience.</em>
		/// \note May throw a cfp::Error or any of its sub-classes.
		/// \param[in] formula The new input formula C-style string.
		/// \param[in] len     Number of characters in \e formula.
		/// \returns The empirical representation of the supplied formula.
		/// \sa setFormula, process, empirical
		const Compound& process(const char * formula, const size_t len);

		/// Returns the result of the most recent parsing operation.
		/// \returns The empirical representation of the supplied formula.
		const Compound& empirical(void) const;

		/// String representation of the parsed Formula with markup text
		/// formatting. This includes \<sup\> for the nucleon number
		/// and \<sub\> for the coefficient value.
		/// \returns The formatted string.
		std::string toMarkup(void) const;

		/// Sets the maximum nesting level within the supplied formulas.
		/// If a formula is processed which contains more nested
		/// parentheses (groups) than specified, an ErrorMaxNesting is 
		/// thrown during Parser::process. The default is 30.
		/// \param[in] lvl The maximum nesting level.
		/// \sa maxNestingLevel 
		void setMaxNestingLevel(size_t lvl);

		/// Returns the maximum nesting level within the supplied
		/// formulas.
		/// \sa setMaxNestingLevel
		size_t maxNestingLevel(void) const;

	private:
		ParserState * mD; //!< Implementation data.
	};

	/**
	 * String representation of a Compound with markup text
	 * formatting. This includes \<sup\> for the nucleon number
	 * and \<sub\> for the coefficient value of the elements.
	 * \returns The formatted string for this Compound.
	 * \sa toString(const cfp::Compound&)
	 */
	std::string toMarkup(const cfp::Compound& el);

	/**
	 * String representation of a Compound. 
	 * \returns The ASCII string for this Compound.
	 * \sa toMarkup(const cfp::Compound&)
	 */
	std::string toString(const cfp::Compound& el);

} // namespace cfp

namespace std
{
	/**
	 * Writes the string representation of an ChemicalElement to an output stream.
	 * \param[in,out] o Output stream to write to.
	 * \param[in]     e ChemicalElement to create a string representation from.
	 * \returns         The output stream o.
	 * \relatesalso cfp::ChemicalElement
	 */
	std::ostream& 
	operator<<(std::ostream& o, const cfp::ChemicalElementInterface& e);

	/**
	 * Writes the string representation of an Compound to an output 
	 * stream.
	 * \param[in,out] o  Output stream to write to.
	 * \param[in]     el Compound to create a string representation from.
	 * \returns          The output stream o.
	 */
	std::ostream& 
	operator<<(std::ostream& o, const cfp::Compound& el);
}

#endif // this file

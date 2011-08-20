/*
 * src/element.h
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

#ifndef CFP_ELEMENT_H
#define CFP_ELEMENT_H

namespace cfp
{
	/// Implementation data of a cfp::ChemicalElement.
	struct ChemicalElementData
	{
		/// Default constructor with initialization.
		ChemicalElementData()
			: symbol(""), 
			  nucleons(ChemicalElementInterface::naturalNucleonNr())
		{}

		std::string symbol;   //!< symbol.
		int         nucleons; //!< Nucleon number (aka atomic mass number).
	};

	/// Implementation data of a cfp::CompoundElement.
	struct CompoundElementData: public ChemicalElementData
	{
		/// Default constructor with initialization.
		CompoundElementData()
			: ChemicalElementData(), 
			  coefficient(1.0)
		{}

		double coefficient; //!< Compound coefficient of this element.
	};

	/// Implementation data of a (internal) cfp::CompoundGroupElement.
	struct CompoundGroupElementData: public CompoundElementData
	{
		/// Default constructor with initialization.
		CompoundGroupElementData()
			: CompoundElementData(), 
			  isGroup(false)
		{}

		/// Decides if the coefficient is associated to a group rather
		/// than to a single element.
		bool isGroup;
	};

	/// Additional to the properties of a cfp::CompoundElement, this
	/// owns a group property. It can be a regular cfp::CompoundElement.
	/// But it may also be a dummy element which serves only the purpose to 
	/// store the coefficient value for a group of elements associated with 
	/// it (by a tree structure in cfp::ElementGroup). If this element
	/// represents a group, its name and symbol are empty.
	/// \note For internal use, only.
	struct CompoundGroupElement: public CompoundElementInterface
	{
	public:
		/// Default constructor.
		/// \param[in] coefficient The compound coefficient of this element.
		/// \param[in] isGroup True, if this element represents a group
		///                    of elements. False, otherwise.
		CompoundGroupElement(double coefficient = 1.0, bool isGroup = false);

		/// Copy constructor.
		CompoundGroupElement(const CompoundGroupElement& cge);

		~CompoundGroupElement(); //!< Destructor.

		/// Identifies an CompoundElement as group descriptor.
		/// \returns True, if this element represents a group of
		///          elements. False, if it is a regular element.
		bool 
		isGroup(void) const;

		/// Copy operator all elements of this kind.
		CompoundGroupElement& 
		operator=(const CompoundGroupElement& e);
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

		/// Specific implementation of ChemicalElementInterface::doCoefficient.
		virtual double 
		doCoefficient(void) const;

		/// Specific implementation of ChemicalElementInterface::doSetCoefficient.
		virtual void 
		doSetCoefficient(double coefficient);
	private:
		CompoundGroupElementData * mD; //!< Implementation data.
	};
}

#endif // this file


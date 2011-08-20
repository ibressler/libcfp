/*
 * src/element.cpp
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

#include <iostream>
#include <sstream>
#include <cfp/cfp.h>
#include "element.h"

using namespace cfp;

////// ChemicalElementInterface, abstract base class //////

const int
ChemicalElementInterface::naturalNucleonNr() 
{
	return -1;
}

ChemicalElementInterface::~ChemicalElementInterface() 
{
}

std::string 
ChemicalElementInterface::symbol(void) const
{
	return doSymbol();
}

void
ChemicalElementInterface::setSymbol(const std::string& s) 
{
	doSetSymbol(s);
}

int
ChemicalElementInterface::nucleons(void) const 
{
	return doNucleons();
}

void
ChemicalElementInterface::setNucleons(int i) 
{
	if (i <= 0) i = naturalNucleonNr();
	doSetNucleons(i);
}

bool
ChemicalElementInterface::isIsotope(void) const 
{
	return nucleons() != naturalNucleonNr();
}

std::string
ChemicalElementInterface::uniqueName(void) const 
{
	return doUniqueName();
}

std::string
ChemicalElementInterface::toString(void) const
{
	return doToString();
}

std::string
ChemicalElementInterface::toMarkup(void) const 
{
	return doToMarkup();
}

bool
ChemicalElementInterface::operator<(const ChemicalElementInterface& e) const
{
	int cmp = symbol().compare(e.symbol());
	if (cmp == 0) {
		return nucleons() < e.nucleons();
	}
	return cmp < 0;
}

ChemicalElementInterface&
ChemicalElementInterface::operator=(const ChemicalElementInterface& e)
{
	if (this != &e) {
		setSymbol(e.symbol());
		setNucleons(e.nucleons());
	}
	return *this;
}

std::string
ChemicalElementInterface::doUniqueName(void) const
{
	return ChemicalElementInterface::doToString();
}

std::string
ChemicalElementInterface::doToString(void) const
{
	std::stringstream ss;
	if (isIsotope())
		ss << nucleons();
	ss << symbol();
	return ss.str();
}

std::string
ChemicalElementInterface::doToMarkup(void) const
{
	std::stringstream ss;
	if (isIsotope()) 
		ss << "<sup>" << nucleons() << "</sup>";
	ss << symbol();
	return ss.str();
}

////// CompoundElementInterface, abstract base class //////

CompoundElementInterface::~CompoundElementInterface()
{
}

void
CompoundElementInterface::setCoefficient(double c)
{
	doSetCoefficient(c);
}

double
CompoundElementInterface::coefficient(void) const 
{
	return doCoefficient();
}

CompoundElementInterface&
CompoundElementInterface::operator=(const CompoundElementInterface& e) 
{
	if (this != &e) {
		ChemicalElementInterface::operator=(e);
		setCoefficient(e.coefficient());
	}
	return *this;
}

std::string
CompoundElementInterface::doToString(void) const
{
	std::stringstream ss;
	if (isIsotope()) ss << "(";
	ss << ChemicalElementInterface::doToString();
	if (isIsotope()) ss << ")";
	if (coefficient() != 1.0) ss << coefficient();
	return ss.str();
}

std::string
CompoundElementInterface::doToMarkup(void) const
{
	std::stringstream ss;
	ss << ChemicalElementInterface::doToMarkup();
	if (coefficient() != 1.0) ss << "<sub>" << coefficient() << "</sub>";
	return ss.str();
}

////// ChemicalElement, concrete element //////

ChemicalElement::ChemicalElement()
	: ChemicalElementInterface(),
	  mD(new ChemicalElementData())
{}

ChemicalElement::ChemicalElement(const ChemicalElement& ce)
	: ChemicalElementInterface(),
	  mD(new ChemicalElementData(*(ce.mD)))
{}

ChemicalElement::ChemicalElement(const ChemicalElementInterface& ce)
	: ChemicalElementInterface(),
	  mD(new ChemicalElementData())
{
	setSymbol(ce.symbol());
	setNucleons(ce.nucleons());
}

ChemicalElement::~ChemicalElement()
{
	if (mD) delete mD;
	mD = NULL;
}

std::string 
ChemicalElement::doSymbol() const 
{
	return mD->symbol;
}

void 
ChemicalElement::doSetSymbol(const std::string& s) 
{
	mD->symbol.assign(s);
}

int
ChemicalElement::doNucleons() const 
{
	return mD->nucleons;
}

void 
ChemicalElement::doSetNucleons(int i) 
{
	mD->nucleons = i;
}

////// CompoundElement, concrete element //////

CompoundElement::CompoundElement()
	: CompoundElementInterface(),
	  mD(new CompoundElementData())
{}

CompoundElement::CompoundElement(const CompoundElement& ce)
	: CompoundElementInterface(),
	  mD(new CompoundElementData(*(ce.mD)))
{}

CompoundElement::CompoundElement(const CompoundElementInterface& ce)
	: CompoundElementInterface(),
	  mD(new CompoundElementData())
{
	setSymbol(ce.symbol());
	setNucleons(ce.nucleons());
	setCoefficient(ce.coefficient());
}

CompoundElement::~CompoundElement()
{
	if (mD) delete mD;
	mD = NULL;
}

std::string 
CompoundElement::doSymbol() const
{
	return mD->symbol;
}

void 
CompoundElement::doSetSymbol(const std::string& s)
{
	mD->symbol.assign(s);
}

int
CompoundElement::doNucleons() const
{
	return mD->nucleons;
}

void 
CompoundElement::doSetNucleons(int i)
{
	mD->nucleons = i;
}

double 
CompoundElement::doCoefficient(void) const
{
	return mD->coefficient;
}

void
CompoundElement::doSetCoefficient(double c) 
{
	mD->coefficient = c;
}

////// CompoundGroupElement //////

CompoundGroupElement::CompoundGroupElement(double c, bool g)
	: CompoundElementInterface(),
	  mD(new CompoundGroupElementData())
{
	setCoefficient(c);
	mD->isGroup = g;
}

CompoundGroupElement::CompoundGroupElement(const CompoundGroupElement& cge)
	: CompoundElementInterface(),
	  mD(new CompoundGroupElementData(*(cge.mD)))
{}

CompoundGroupElement::~CompoundGroupElement()
{
	if (mD) delete mD;
	mD = NULL;
}

CompoundGroupElement&
CompoundGroupElement::operator=(const CompoundGroupElement& e)
{
	CompoundElementInterface::operator=(e);
	mD->isGroup = e.isGroup();
	return *this;
}

bool
CompoundGroupElement::isGroup(void) const
{
	return mD->isGroup;
}

std::string 
CompoundGroupElement::doSymbol() const
{
	return mD->symbol;
}

void 
CompoundGroupElement::doSetSymbol(const std::string& s)
{
	mD->symbol.assign(s);
}

int
CompoundGroupElement::doNucleons() const
{
	return mD->nucleons;
}

void 
CompoundGroupElement::doSetNucleons(int i)
{
	mD->nucleons = i;
}

double 
CompoundGroupElement::doCoefficient(void) const
{
	return mD->coefficient;
}

void
CompoundGroupElement::doSetCoefficient(double c)
{
	mD->coefficient = c;
}

////// ::cfp:: global helper //////

std::string 
cfp::toMarkup(const cfp::Compound& el)
{
	std::string str;
	Compound::const_iterator it = el.begin();
	while(it != el.end())
	{
		str.append(it->toMarkup());
		it++;
		if (it != el.end()) str.append(" ");
	}
	return str;
}

std::string 
cfp::toString(const cfp::Compound& el)
{
	std::stringstream ss;
	ss << el;
	return ss.str();
}

std::ostream& 
std::operator<<(std::ostream& o, const ChemicalElementInterface& e)
{
	o << e.toString();
	return o;
}

std::ostream& 
std::operator<<(std::ostream& o, const Compound& el)
{
	Compound::const_iterator it = el.begin();
	while(it != el.end())
	{
		o << it->toString();
		it++;
		if (it != el.end()) o << " ";
	}
	return o;
}


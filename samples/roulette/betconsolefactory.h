/*******************************************************************************
 * wallaroo - A library for configurable creation and wiring of C++ classes
 * Copyright (C) 2012 Daniele Pallastrelli 
 *
 *
 * This file is part of wallaroo.
 * For more information, see http://wallaroo.googlecode.com/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 ******************************************************************************/

#ifndef BETCONSOLEFACTORY_H_
#define BETCONSOLEFACTORY_H_

#include "wallaroo/registered.h"
#include "wallaroo/cxx0x.h"
#include "bet.h"

// This interface describes the factory classes that can
// create a Bet starting from a cmd line string with a
// specific syntax.
class BetConsoleFactory : public wallaroo::Device
{
public:
    virtual ~BetConsoleFactory() {}
    // Provides the syntax of the string to create the specific Bet
    virtual std::string Help() const = 0;
    // If the cmdLine conforms to the syntax described by Help() method,
    // returns a new instance of the bet.
    // Otherwise it returns an empty shared_ptr
    virtual cxx0x::shared_ptr< Bet > Create( const std::string& cmdLine ) const = 0;
};

#endif

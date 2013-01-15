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

#include <sstream>
#include "straightbetconsolefactory.h"
#include "straightbet.h"

WALLAROO_REGISTER( StraightBetConsoleFactory );

StraightBetConsoleFactory::~StraightBetConsoleFactory()
{
}

std::string StraightBetConsoleFactory::Help() const
{
    return "straight <bin> <amount>";
}

cxx0x::shared_ptr< Bet > StraightBetConsoleFactory::Create( const std::string& cmdLine ) const
{
    using namespace std;

    string cmdName;
    string inputBin;
    Currency amount;

    stringstream ss( cmdLine );

    if ( ss.eof() ) return cxx0x::shared_ptr< Bet >();
    ss >> cmdName;
    if ( cmdName != "straight" ) return cxx0x::shared_ptr< Bet >();

    if ( ss.eof() ) return cxx0x::shared_ptr< Bet >();
    ss >> inputBin;
    
    if ( ss.eof() ) return cxx0x::shared_ptr< Bet >();
    ss >> amount;
    if ( ss.fail() ) return cxx0x::shared_ptr< Bet >();
    if ( !ss.eof() ) return cxx0x::shared_ptr< Bet >();

    Bin bin( inputBin );
    return cxx0x::shared_ptr< Bet >( new StraightBet( bin, amount ) );
}


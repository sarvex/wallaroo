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
#include "oddevenbet.h"

OddEvenBet::OddEvenBet( Type _type, Currency _amount ) :
    type( _type ),
    amount( _amount )
{
    if ( type == Odd )
    {
        winningBins.insert( _1 );
        winningBins.insert( _3 );
        winningBins.insert( _5 );
        winningBins.insert( _7 );
        winningBins.insert( _9 );
        winningBins.insert( _11 );
        winningBins.insert( _13 );
        winningBins.insert( _15 );
        winningBins.insert( _17 );
        winningBins.insert( _19 );
        winningBins.insert( _21 );
        winningBins.insert( _23 );
        winningBins.insert( _25 );
        winningBins.insert( _27 );
        winningBins.insert( _29 );
        winningBins.insert( _31 );
        winningBins.insert( _33 );
        winningBins.insert( _35 );
    }
    else
    {
        winningBins.insert( _2 );
        winningBins.insert( _4 );
        winningBins.insert( _6 );
        winningBins.insert( _8 );
        winningBins.insert( _10 );
        winningBins.insert( _12 );
        winningBins.insert( _14 );
        winningBins.insert( _16 );
        winningBins.insert( _18 );
        winningBins.insert( _20 );
        winningBins.insert( _22 );
        winningBins.insert( _24 );
        winningBins.insert( _26 );
        winningBins.insert( _28 );
        winningBins.insert( _30 );
        winningBins.insert( _32 );
        winningBins.insert( _34 );
        winningBins.insert( _36 );
    }
}

Currency OddEvenBet::Table( const Bin& outcome )
{
    static const unsigned int payout = 1;

    if ( winningBins.find( outcome ) != winningBins.end() ) // win
        return amount * ( payout + 1 );
    else // lose
        return 0;
}

std::string OddEvenBet::Description() const
{
    std::stringstream s;
    s << amount << "$ on " << ( type == Odd ? "ODD" : "EVEN" );
    return s.str();
}

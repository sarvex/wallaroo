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
        winningBins.insert( Bin::_1 );
        winningBins.insert( Bin::_3 );
        winningBins.insert( Bin::_5 );
        winningBins.insert( Bin::_7 );
        winningBins.insert( Bin::_9 );
        winningBins.insert( Bin::_11 );
        winningBins.insert( Bin::_13 );
        winningBins.insert( Bin::_15 );
        winningBins.insert( Bin::_17 );
        winningBins.insert( Bin::_19 );
        winningBins.insert( Bin::_21 );
        winningBins.insert( Bin::_23 );
        winningBins.insert( Bin::_25 );
        winningBins.insert( Bin::_27 );
        winningBins.insert( Bin::_29 );
        winningBins.insert( Bin::_31 );
        winningBins.insert( Bin::_33 );
        winningBins.insert( Bin::_35 );
    }
    else
    {
        winningBins.insert( Bin::_2 );
        winningBins.insert( Bin::_4 );
        winningBins.insert( Bin::_6 );
        winningBins.insert( Bin::_8 );
        winningBins.insert( Bin::_10 );
        winningBins.insert( Bin::_12 );
        winningBins.insert( Bin::_14 );
        winningBins.insert( Bin::_16 );
        winningBins.insert( Bin::_18 );
        winningBins.insert( Bin::_20 );
        winningBins.insert( Bin::_22 );
        winningBins.insert( Bin::_24 );
        winningBins.insert( Bin::_26 );
        winningBins.insert( Bin::_28 );
        winningBins.insert( Bin::_30 );
        winningBins.insert( Bin::_32 );
        winningBins.insert( Bin::_34 );
        winningBins.insert( Bin::_36 );
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
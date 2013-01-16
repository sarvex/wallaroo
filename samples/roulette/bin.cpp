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

#include "bin.h"

Bin Bin::_00( "00" );
Bin Bin::_0( "0" );
Bin Bin::_1( "1" );
Bin Bin::_2( "2" );
Bin Bin::_3( "3" );
Bin Bin::_4( "4" );
Bin Bin::_5( "5" );
Bin Bin::_6( "6" );
Bin Bin::_7( "7" );
Bin Bin::_8( "8" );
Bin Bin::_9( "9" );
Bin Bin::_10( "10" );
Bin Bin::_11( "11" );
Bin Bin::_12( "12" );
Bin Bin::_13( "13" );
Bin Bin::_14( "14" );
Bin Bin::_15( "15" );
Bin Bin::_16( "16" );
Bin Bin::_17( "17" );
Bin Bin::_18( "18" );
Bin Bin::_19( "19" );
Bin Bin::_20( "20" );
Bin Bin::_21( "21" );
Bin Bin::_22( "22" );
Bin Bin::_23( "23" );
Bin Bin::_24( "24" );
Bin Bin::_25( "25" );
Bin Bin::_26( "26" );
Bin Bin::_27( "27" );
Bin Bin::_28( "28" );
Bin Bin::_29( "29" );
Bin Bin::_30( "30" );
Bin Bin::_31( "31" );
Bin Bin::_32( "32" );
Bin Bin::_33( "33" );
Bin Bin::_34( "34" );
Bin Bin::_35( "35" );
Bin Bin::_36( "36" );

Bin::Bin( const Bin& bin ) :
    value( bin.value )
{
}

Bin::Bin( const std::string& _value ) :
    value( _value )
{
}

const std::string& Bin::Value() const
{
    return value;
}

bool Bin::operator==( const Bin& other) const
{
    return value == other.value;
}

bool Bin::operator<( const Bin& other) const
{
    return value < other.value;
}

Bin& Bin::operator=( const Bin& rhs )
{
    if ( this == &rhs )
        return *this;        // Yes, so skip assignment, and just return *this.

    value = rhs.value;

    return *this;
}


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

#include <cstdlib>
#include <ctime>
#include "wheel.h"

WALLAROO_REGISTER( Wheel );

Wheel::Wheel()
{
    bins.push_back( Bin::_0 );
    bins.push_back( Bin::_1 );
    bins.push_back( Bin::_2 );
    bins.push_back( Bin::_3 );
    bins.push_back( Bin::_4 );
    bins.push_back( Bin::_5 );
    bins.push_back( Bin::_6 );
    bins.push_back( Bin::_7 );
    bins.push_back( Bin::_8 );
    bins.push_back( Bin::_9 );
    bins.push_back( Bin::_10 );
    bins.push_back( Bin::_11 );
    bins.push_back( Bin::_12 );
    bins.push_back( Bin::_13 );
    bins.push_back( Bin::_14 );
    bins.push_back( Bin::_15 );
    bins.push_back( Bin::_16 );
    bins.push_back( Bin::_17 );
    bins.push_back( Bin::_18 );
    bins.push_back( Bin::_19 );
    bins.push_back( Bin::_20 );
    bins.push_back( Bin::_21 );
    bins.push_back( Bin::_22 );
    bins.push_back( Bin::_23 );
    bins.push_back( Bin::_24 );
    bins.push_back( Bin::_25 );
    bins.push_back( Bin::_26 );
    bins.push_back( Bin::_27 );
    bins.push_back( Bin::_28 );
    bins.push_back( Bin::_29 );
    bins.push_back( Bin::_30 );
    bins.push_back( Bin::_31 );
    bins.push_back( Bin::_32 );
    bins.push_back( Bin::_33 );
    bins.push_back( Bin::_34 );
    bins.push_back( Bin::_35 );
    bins.push_back( Bin::_36 );
    bins.push_back( Bin::_00 );

    srand( static_cast< unsigned int >( time( NULL ) ) );
}

Wheel::~Wheel()
{
}

Bin Wheel::Spin()
{
    size_t index = rand() % ( bins.size() );
    return bins[ index ];
}
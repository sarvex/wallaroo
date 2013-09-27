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
    bins.push_back( _0 );
    bins.push_back( _1 );
    bins.push_back( _2 );
    bins.push_back( _3 );
    bins.push_back( _4 );
    bins.push_back( _5 );
    bins.push_back( _6 );
    bins.push_back( _7 );
    bins.push_back( _8 );
    bins.push_back( _9 );
    bins.push_back( _10 );
    bins.push_back( _11 );
    bins.push_back( _12 );
    bins.push_back( _13 );
    bins.push_back( _14 );
    bins.push_back( _15 );
    bins.push_back( _16 );
    bins.push_back( _17 );
    bins.push_back( _18 );
    bins.push_back( _19 );
    bins.push_back( _20 );
    bins.push_back( _21 );
    bins.push_back( _22 );
    bins.push_back( _23 );
    bins.push_back( _24 );
    bins.push_back( _25 );
    bins.push_back( _26 );
    bins.push_back( _27 );
    bins.push_back( _28 );
    bins.push_back( _29 );
    bins.push_back( _30 );
    bins.push_back( _31 );
    bins.push_back( _32 );
    bins.push_back( _33 );
    bins.push_back( _34 );
    bins.push_back( _35 );
    bins.push_back( _36 );
    bins.push_back( _00 );

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

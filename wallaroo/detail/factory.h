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

#ifndef WALLAROO_DETAIL_FACTORY_H_
#define WALLAROO_DETAIL_FACTORY_H_

#include "wallaroo/cxx0x.h"
#include "wallaroo/device.h"

namespace wallaroo
{
namespace detail
{

// This helper class exports the method to create the class T.
// Can't use a function because we cannot partial specialize template functions.
template < class T, class P1, class P2 >
class Factory
{
public:
    static cxx0x::shared_ptr< Device > Create( const P1& p1, const P2& p2 )
    {
        return cxx0x::make_shared< T >( p1, p2 );
    }
};

template < class T, class P >
class Factory< T, P, void >
{
public:
    static cxx0x::shared_ptr< Device > Create( const P& p )
    {
        return cxx0x::make_shared< T >( p );
    }
};

template < class T >
class Factory< T, void, void >
{
public:
    static cxx0x::shared_ptr< Device > Create()
    {
        return cxx0x::make_shared< T >();
    }
};

} // detail namespace
} // wallaroo namespace

#endif // WALLAROO_CLASS_H_

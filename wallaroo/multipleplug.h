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

#ifndef WALLAROO_MULTIPLEPLUG_H_
#define WALLAROO_MULTIPLEPLUG_H_

#if 0

#include <string>
#include <cassert>
#include <list>
#include <vector>
#include <deque>
#include <typeinfo>
#include "cxx0x.h"
#include "connector.h"
#include "device.h"

namespace wallaroo
{

/**
 * This represents a connector consisting of many "plugs".
 *
 * It is a (i.e. it *derives* from) std::list< weak_ptr< T >,
 * so you can apply all the STL algorithms to it.
 * Please note that it contains weak_ptr, so when you access
 * the elements, you should check for its validity.
 */
template < 
	typename T,
	template < typename E, typename Allocator = std::allocator< E > > class Container = std::list
>
class MultiplePlug : 
    public Connector,
    public Container< cxx0x::weak_ptr< T > >
{
public:
    /** Create a MultiplePlug and register it to its device for future wiring.
    * @param name the name of this MultiplePlug
    * @param owner the device that contains this MultiplePlug
    */
    MultiplePlug( const std::string& name, Device* owner )
    {
        owner -> Register( name, this );
    }

    /** Connect a device into this multiple plug
    * @param device The device to connect
    * @throw std::bad_cast If @c device is not a subclass of @c T
    */
    void PlugInto( cxx0x::shared_ptr< Device > device )
    {
        cxx0x::shared_ptr< T > obj = cxx0x::dynamic_pointer_cast< T >( device );
        if ( ! obj ) // bad type!
            throw std::bad_cast();
        else
            push_back( obj );
    }
};

// TODO ### we don't have template typedef yet!
#define AttributeContainer MultiplePlug


// TODO ### TEST


template < typename T >
class List : public std::list< cxx0x::weak_ptr< T > > {};

template < typename T >
class Vector : public std::vector< cxx0x::weak_ptr< T > > {};

template < typename T >
class Deque : public std::deque< cxx0x::weak_ptr< T > > {};



template < typename T >
class Plug< List< T > > : public Connector, public List< T >
{
public:
    Plug( const std::string& name, Device* owner )
    {
        owner -> Register( name, this );
    }
    void PlugInto( cxx0x::shared_ptr< Device > device )
    {
        cxx0x::shared_ptr< T > obj = cxx0x::dynamic_pointer_cast< T >( device );
        if ( ! obj ) // bad type!
            throw std::bad_cast();
        else
            push_back( obj );
    }
};

template < typename T >
class Plug< Vector< T > > : public Connector, public Vector< T >
{
public:
    Plug( const std::string& name, Device* owner )
    {
        owner -> Register( name, this );
    }
    void PlugInto( cxx0x::shared_ptr< Device > device )
    {
        cxx0x::shared_ptr< T > obj = cxx0x::dynamic_pointer_cast< T >( device );
        if ( ! obj ) // bad type!
            throw std::bad_cast();
        else
            push_back( obj );
    }
};

template < typename T >
class Plug< Deque< T > > : public Connector, public Deque< T >
{
public:
    Plug( const std::string& name, Device* owner )
    {
        owner -> Register( name, this );
    }
    void PlugInto( cxx0x::shared_ptr< Device > device )
    {
        cxx0x::shared_ptr< T > obj = cxx0x::dynamic_pointer_cast< T >( device );
        if ( ! obj ) // bad type!
            throw std::bad_cast();
        else
            push_back( obj );
    }
};

} // namespace


#endif

#endif
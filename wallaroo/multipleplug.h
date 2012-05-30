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

#include <string>
#include <cassert>
#include <list>
#include <typeinfo>
//#include <boost/shared_ptr.hpp>
#include "connector.h"
#include "device.h"

namespace wallaroo
{

template < class T >
class MultiplePlug : 
    public Connector,
    public std::list< T* >
{
public:
    MultiplePlug( const std::string& id, Device* device )
    {
        device -> Register( id, this );
    }

    /** Connect a device into this multiple plug
    * @param device The device to connect
    * @throw std::bad_cast If @c device is not a subclass of @c T
    */
    void PlugInto( Device* device )
    {
        // obj = boost::dynamic_pointer_cast< T >( device );
        T* obj = dynamic_cast< T* >( device );
        if ( obj == NULL ) // bad type!
            throw std::bad_cast();
        else
            push_back( obj );
    }
};

} // namespace

#endif
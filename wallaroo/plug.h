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

#ifndef WALLAROO_PLUG_H_
#define WALLAROO_PLUG_H_

#include <string>
#include <cassert>
#include <typeinfo>
#include <boost/weak_ptr.hpp>
#include "connector.h"
#include "device.h"

namespace wallaroo
{

template < class T >
class Plug : public Connector
{
public:

    typedef boost::weak_ptr< T > WeakPtr;
    typedef boost::shared_ptr< T > SharedPtr;

    Plug( const std::string& plugName, Device* plugOwner )
    {
        plugOwner -> Register( plugName, this );
    }
    
    /** Plug this plug into a device
    * @param dev The device you want insert this plug into
    * @throw std::bad_cast If @c dev is not a subclass of @c T
    */
    void PlugInto( boost::shared_ptr< Device > dev )
    {
        boost::shared_ptr< T > _dev = boost::dynamic_pointer_cast< T >( dev );
        if ( ! _dev ) // bad type!
            throw std::bad_cast();
        else
            device = _dev;
    }

    SharedPtr operator -> ()
    {
        SharedPtr result = device.lock();
        assert( result );
        return result;
    }

    const T* operator -> () const
    {
        assert( device != NULL );
        return device;
    }

private:
    WeakPtr device;
};

} // namespace

#endif
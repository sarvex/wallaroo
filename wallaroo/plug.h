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
//#include <boost/shared_ptr.hpp>
#include "connector.h"
#include "device.h"

namespace wallaroo
{

template < class T >
class Plug : public Connector
{
public:

    Plug( const std::string& plugName, Device* plugOwner ) :
        device( NULL )
    {
        plugOwner -> Register( plugName, this );
    }
    
    /** Plug this plug into a device
    * @param dev The device you want insert this plug into
    * @throw std::bad_cast If @c dev is not a subclass of @c T
    */
    void PlugInto( Device* dev )
    {
        // device = boost::dynamic_pointer_cast< T >( dev );
        device = dynamic_cast< T* >( dev );
        if ( device == NULL ) // bad type!
            throw std::bad_cast();
    }

    T* operator -> ()
    {
        assert( device != NULL );
        return device;
    }

    const T* operator -> () const
    {
        assert( device != NULL );
        return device;
    }

private:
    T* device;
};

} // namespace

#endif
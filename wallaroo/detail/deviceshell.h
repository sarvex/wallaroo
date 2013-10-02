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

#ifndef WALLAROO_DETAIL_DEVICESHELL_H_
#define WALLAROO_DETAIL_DEVICESHELL_H_

#include <string>
#include <typeinfo>
#include <cassert>
#include "wallaroo/cxx0x.h"
#include "wallaroo/device.h"

namespace wallaroo
{
namespace detail
{


// This class provides the conversion operator to the contained type.
// In particular, the Catalog::operator[] returns a DeviceShell
// that can be converted to the inner type.
class DeviceShell
{
public:

    DeviceShell( const cxx0x::shared_ptr< Device >& dev ) :
        device( dev ) 
    {
        assert( device );
    }

    void Wire( const std::string& plugName, const DeviceShell& destination ) const
    {
        device -> Wire( plugName, destination.device );
    }

    /** Convert the contained device to the type T
    * @return the converted device.
    * @throw WrongType if the contained device is not a subclass of T
    */
    template < class T >
    operator cxx0x::shared_ptr< T >()
    {
        cxx0x::shared_ptr< T > result = cxx0x::dynamic_pointer_cast< T >( device );
        if ( ! result ) throw WrongType();
        return result;
    }

private:
    cxx0x::shared_ptr< Device > device;
};

} // namespace detail
} // namespace wallaroo

#endif

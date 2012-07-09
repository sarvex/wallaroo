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

#ifndef WALLAROO_DEVICE_H_
#define WALLAROO_DEVICE_H_

#include <string>
#include <map>
#include <stdexcept>
#include "connector.h"

namespace wallaroo
{


class Device
{
public:
    /** Plug the plug @c plugName of this device into the device @c device.
     * @throw std::range_error if @c plugName does not exist in this device.
     */
    virtual void Wire( const std::string& plugName, Device* device )
    {
        Plugs::iterator i = plugs.find( plugName );
        if ( i == plugs.end() ) 
            throw std::range_error( plugName + " not found in the class" );
        ( i -> second ) -> PlugInto( device );
    }

    void Register( const std::string& id, Connector* plug )
    {
        plugs[ id ] = plug;
    }

    virtual ~Device() {}

private:
    typedef std::map< std::string, Connector* > Plugs;
    Plugs plugs;
};

} // namespace

#endif
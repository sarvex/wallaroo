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
#include "exceptions.h"
#include "cxx0x.h"
#include "connector.h"

namespace wallaroo
{

/**
 * This represents a "device" that owns connectors.
 * You can plug his connectors to other devices using the method @c Wire.
 */
class Device
{
public:
    /** Plug the connector @c connector of this device into the device @c device.
     * @throw ElementNotFound if @c connector does not exist in this device.
     */
    virtual void Wire( const std::string& connector, cxx0x::shared_ptr< Device > device )
    {
        Connectors::iterator i = connectors.find( connector );
        if ( i == connectors.end() ) 
            throw ElementNotFound( connector );
        ( i -> second ) -> PlugInto( device );
    }

    // this method should be invoked just by the connectors of this device
    // to register itself into the connectors table.
    void Register( const std::string& id, Connector* plug )
    {
        connectors[ id ] = plug;
    }

    virtual ~Device() {}

private:
    typedef std::map< std::string, Connector* > Connectors;
    Connectors connectors;
};

} // namespace

#endif
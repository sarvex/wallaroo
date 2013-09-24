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
#include "exceptions.h"
#include "cxx0x.h"
#include "connector.h"

namespace wallaroo
{

// forward declaration:
class Plugin;

class RegToken
{
public:
    Device* GetDevice() const { return device; }
private:
    friend class Device;
    RegToken( Device* d ) : device( d ) {}
    Device* device;
};

/**
 * This class represents a "device" that owns connectors.
 * You can plug its connectors to other devices using the method Device::Wire.
 */
class Device
{
public:
    // we need to make Device virtual, to use dynamic_cast
    virtual ~Device() {}

    /** Plug the connector @c connector of this device into the Device @c device.
     * @throw ElementNotFound if @c connector does not exist in this device.
     */
    void Wire( const std::string& connector, const cxx0x::shared_ptr< Device >& device )
    {
        Connectors::iterator i = connectors.find( connector );
        if ( i == connectors.end() ) 
            throw ElementNotFound( connector );
        ( i -> second ) -> PlugInto( device );
    }


   /** Check the multiplicity of its plugs.
    * @return true if the check pass
    */
    bool MultiplicitiesOk() const
    {
        for ( 
            Connectors::const_iterator i = connectors.begin();
            i != connectors.end();
            ++i
        )
            if ( ! i -> second -> WiringOk() )
                return false;
        return true;
    }

protected:
    RegToken RegistrationToken()
    { 
        return RegToken( this );
    }
private:
    // this method should only be invoked by Class
    // to add the reference counter for the shared library.
    template < class T1, class T2 > friend class Class;
    void Source( const cxx0x::shared_ptr< Plugin >& p )
    {
        plugin = p;
    }

    // this method should only be invoked by the connectors of this device
    // to register itself into the connectors table.
    template < class T, class P, template < typename E, typename Allocator = std::allocator< E > > class Container > friend class Plug;
    void Register( const std::string& id, Connector* plug )
    {
        connectors[ id ] = plug;
    }

    typedef cxx0x::unordered_map< std::string, Connector* > Connectors;
    Connectors connectors;
    cxx0x::shared_ptr< Plugin > plugin; // optional shared ptr to plugin, to release the shared library when is no more used
};

} // namespace

#endif

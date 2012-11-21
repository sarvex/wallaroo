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
#include <typeinfo>
#include "cxx0x.h"
#include "connector.h"
#include "device.h"
#include "exceptions.h"

namespace wallaroo
{

/// This type should be used as second template parameter in Plug class to specify the Plug is optional
/// (i.e.: you can omit to wire a device to the plug)
struct optional
{
    template < typename T >
    static bool WiringOk( cxx0x::weak_ptr< T > ) { return true; }
};
/// This type should be used as second template parameter in Plug class to specify the Plug is mandatory
/// (i.e.: you cannot omit to wire a device to the plug)
struct mandatory
{
    template < typename T >
    static bool WiringOk( cxx0x::weak_ptr< T > t ) { return t.lock(); }
};
/// This type should be used as second template parameter in Plug class to specify the Plug is a collection
/// (i.e.: you can wire many device to the plug)
template < size_t MIN = 0, size_t MAX = 0 >
struct bounded_collection
{
    template < typename T >
    static bool WiringOk( const T* t )
    {
        const size_t s = t -> size();
        return ( s >= MIN && ( MAX == 0 || s <= MAX ) );
    }
};

typedef bounded_collection<> collection;

/**
 * This represents a "plug" of a "device" that
 * you can "plug" into another "device".
 *
 * If the device1 has the plug1 plugged to device2, device1 will
 * basically get a pointer to device2.
 *
 * @tparam T The type of the Device contained
 * @tparam P This represents the kind of Plug (@ref mandatory if you must wire a device,
 *           @ref optional if you can leave this plug unwired, 
 *           @ref collection if you can wire many devices to this plug)
 * @tparam Container If P = @ref collection, this represents the std container the Plug will derive from.
 */
template < 
    typename T,
    typename P = mandatory,
    template < typename E, typename Allocator = std::allocator< E > > class Container = std::list
>
class Plug  : public Connector
{
public:

    typedef cxx0x::weak_ptr< T > WeakPtr;
    typedef cxx0x::shared_ptr< T > SharedPtr;

    /** Create a Plug and register it to its device for future wiring.
    * @param name the name of this plug
    * @param owner the device that contains this plug
    */
    Plug( const std::string& name, Device* owner )
    {
        owner -> Register( name, this );
    }
    
    /** Plug this plug into a device
    * @param dev The device you want insert this plug into
    * @throw WrongType If @c dev is not a subclass of @c T
    */
    void PlugInto( cxx0x::shared_ptr< Device > dev )
    {
        cxx0x::shared_ptr< T > _dev = cxx0x::dynamic_pointer_cast< T >( dev );
        if ( ! _dev ) // bad type!
            throw WrongType();
        else
            device = _dev;
    }

    /** Give access to the embedded device.
    * @throw DeletedDeviceError If the embedded device has been deleted.
    */
    SharedPtr operator -> ()
    {
        SharedPtr result = device.lock();
        if ( ! result ) 
            throw DeletedDeviceError();
        return result;
    }

    /** Give access to the embedded device as const.
    * @throw DeletedDeviceError If the embedded device has been deleted.
    */
    const SharedPtr operator -> () const
    {
        const SharedPtr result = device.lock();
        if ( ! result )
            throw DeletedDeviceError();
        return result;
    }

   /** Check if this Plug is correctly wired according to the
    * P template parameter policy.
    * @return true If the check pass.
    */
    virtual bool WiringOk() const
    {
        return P::WiringOk( device );
    }

private:
    WeakPtr device;
};


// partial specialization for the collection case
template <
    typename T,
    template < typename E, typename Allocator = std::allocator< E > > class Container,
    size_t MIN,
    size_t MAX
>
class Plug< T, bounded_collection< MIN, MAX >, Container > : public Connector, public Container< cxx0x::weak_ptr< T > >
{
public:
    /** Create a Plug and register it to its device for future wiring.
    * @param name the name of this Plug
    * @param owner the device that contains this Plug
    */
    Plug( const std::string& name, Device* owner )
    {
        owner -> Register( name, this );
    }

    /** Connect a device into this multiple plug
    * @param device The device to connect
    * @throw WrongType If @c device is not a subclass of @c T
    */
    void PlugInto( cxx0x::shared_ptr< Device > device )
    {
        cxx0x::shared_ptr< T > obj = cxx0x::dynamic_pointer_cast< T >( device );
        if ( ! obj ) // bad type!
            throw WrongType();
        else
            push_back( obj );
    }


    /** Check if this Plug is correctly wired (i.e. the size of the collection
    * must be comprise in the interval [MIN, MAX])
    * @return true If the check pass.
    */
    virtual bool WiringOk() const
    {
        return bounded_collection< MIN, MAX >::WiringOk( this );
    }
};


// DEPRECATED: Backward compatibility only

/**
 * @deprecated
 * Backward compatibility only: use Plug< T, collection > instead.
 */
template < class T >
class MultiplePlug : public Plug< T, collection >
{
public:
    MultiplePlug( const std::string& name, Device* owner ) :
        Plug< T, collection >( name, owner )
    {}
};

// end DEPRECATED


}




#endif
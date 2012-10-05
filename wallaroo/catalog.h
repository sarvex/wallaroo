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

#ifndef WALLAROO_CATALOG_H_
#define WALLAROO_CATALOG_H_

#include <string>
#include <map>
#include <typeinfo>
#include <cassert>
#include "cxx0x.h"
#include "device.h"
#include "class.h"

namespace wallaroo
{
namespace detail
{

class DeviceShell; // forward declaration

// This is a service class that provides the "Into" part in the syntax:
//   catalog[ "source" ].Plug( "plug" ).Into( catalog[ "destination" ] );
// In particular, the method DeviceShell::Plug returns a PlugShell
// that contains the references to the "source" device and its "plug".
class PlugShell
{
public:
    PlugShell( cxx0x::shared_ptr< Device > dev, const std::string& plug ) :
        device( dev ),
        plugName( plug )
    {}
    void Into( DeviceShell destination );
private:
    cxx0x::shared_ptr< Device > device;
    const std::string plugName;
};

// This is a service class that provides the "Plug" part in the syntax:
//   catalog[ "source" ].Plug( "plug" ).Into( catalog[ "destination" ] );
// In particular, the Catalog::operator[] returns a DeviceShell
// that contains the reference to the "source" device.
class DeviceShell
{
public:

    DeviceShell( cxx0x::shared_ptr< Device > dev ) : 
        device( dev ) 
    {
        assert( device );
    }

    PlugShell Plug( const std::string& plugName ) const
    {
        return PlugShell( device, plugName );
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
    friend class PlugShell;
    cxx0x::shared_ptr< Device > device;
};


inline void PlugShell::Into( DeviceShell destination )
{
    device -> Wire( plugName, destination.device );
}


// forward declarations:
class Context;
class UseAsExpression;
class UseExpression;

} // namespace detail

/**
 * Catalog of the devices available for the application.
 *
 * It can create the instances from the name of a registered class
 * or add the instances.
 *
 * Each item in the catalog is identified by a @c id, with which
 * you can do a lookup.
 */
class Catalog
{
public:
    /** Look for the element @c itemId in the catalog.
    * @param itemId the name of the element
    * @return the element.
    * @throw ElementNotFound if the element does not exist in the catalog.
    */
    detail::DeviceShell operator [] ( const std::string& itemId )
    {
        Devices::iterator i = devices.find( itemId );
        if ( i == devices.end() ) throw ElementNotFound( itemId );
        return detail::DeviceShell( i -> second );
    }

    /** Add a device to the catalog
    * @param id the name of the device to add
    * @param dev the device to add
    * @throw DuplicatedElement if a device with the name @c id is already in the catalog
    */
    void Add( const std::string& id, cxx0x::shared_ptr< Device > dev )
    {
        cxx0x::shared_ptr< Device > toInsert( dev );
        std::pair< Devices::iterator, bool > result = 
            devices.insert( std::make_pair( id, toInsert ) );
        if ( ! result.second )
            throw DuplicatedElement( id );
    }

    /** Instantiate a class with a 2 parameters constructor and add it to the catalog
    * @param id the name of the element to create and add
    * @param className the name of the class to instantiate
    * @param p1 The first parameter of the class constructor
    * @param p2 The second parameter of the class constructor
    * @throw DuplicatedElement if an element with the name @c id is already in the catalog
    * @throw ElementNotFound if @c className class has not been registered
    */
    template < class P1, class P2 >
    void Create( const std::string& id, const std::string& className, const P1& p1, const P2& p2 )
    {
        typedef Class< Device, P1, P2 > C;
        C c = C::ForName( className );
        cxx0x::shared_ptr< Device > obj = c.NewInstance( p1, p2 );
        if ( obj.get() == NULL ) throw ElementNotFound( className );
        Add( id, obj );
    }

    /** Instantiate a class with a 1 parameters constructor and add it to the catalog
    * @param id the name of the element to create and add
    * @param className the name of the class to instantiate
    * @param p The parameter of the class constructor
    * @throw DuplicatedElement if an element with the name @c id is already in the catalog
    * @throw ElementNotFound if @c className class has not been registered
    */
    template < class P >
    void Create( const std::string& id, const std::string& className, const P& p )
    {
        typedef Class< Device, P, void > C;
        C c = C::ForName( className );
        cxx0x::shared_ptr< Device > obj = c.NewInstance( p );
        if ( obj.get() == NULL ) throw ElementNotFound( className );
        Add( id, obj );
    }

    /** Instantiate a class with 0 parameters constructor and add it to the catalog
    * @param id the name of the element to create and add
    * @param className the name of the class to instantiate
    * @throw DuplicatedElement if an element with the name @c id is already in the catalog
    * @throw ElementNotFound if @c className class has not been registered
    */
    void Create( const std::string& id, const std::string& className )
    {
        typedef Class< Device, void, void > C;
        C c = C::ForName( className );
        cxx0x::shared_ptr< Device > obj = c.NewInstance();
        if ( obj.get() == NULL ) throw ElementNotFound( className );
        Add( id, obj );
    }

private:
    typedef std::map< std::string, cxx0x::shared_ptr< Device > > Devices;
    Devices devices;

    friend class detail::Context;
    friend class detail::UseAsExpression;
    friend detail::UseExpression use( const std::string& destClass );

    static Catalog*& Current()
    {
        static Catalog* current = NULL;
        return current;
    }
};

// begin new DSL

namespace detail
{

// this is a helper class that provides the result of the use().as() function
// useful to concatenate use().as() with of().
class UseAsExpression
{
public:
    UseAsExpression( detail::DeviceShell& _destClass, const std::string& _attribute ) :
      destClass( _destClass ),
      attribute( _attribute )
    {
    }
    void of( const detail::DeviceShell& srcClass )
    {
        // perform the final assignment:
        srcClass.Plug( attribute ).Into( destClass );
    }
    // throw CatalogNotSpecified if the current catalog has not been selected with wallaroo_within
    void of ( const std::string& srcClass )
    {
        // default container case
        Catalog* current = Catalog::Current();
        if ( ! current ) throw CatalogNotSpecified();
        of( ( *current )[ srcClass ] );
    }
private:
    detail::DeviceShell destClass;
    std::string attribute;
};

// this is a helper class that provides the result of the use() function
// useful to concatenate use() with as().
class UseExpression
{
public:
    explicit UseExpression( const detail::DeviceShell& _destClass )
        : destClass( _destClass )
    {
    }
    UseAsExpression as( const std::string& attribute )
    {
        return UseAsExpression( destClass, attribute );
    }
private:
    detail::DeviceShell destClass;
};

} // namespace detail


/**
 * This function provides the "use" part in the syntax 
 * @c use( catalog[ "device1" ] ).as( "plug" ).of( catalog[ "device2" ] )
 */
inline detail::UseExpression use( const detail::DeviceShell& destClass )
{
    return detail::UseExpression( destClass );
}

/**
 * This function provides the "use" part in the syntax @c use( "device1" ).as( "plug" ).of( "device2" )
 * @throw CatalogNotSpecified if the current catalog has not been selected including
 * this function in a wallaroo_within section
 */
inline detail::UseExpression use( const std::string& destClass )
{
    // default container case
    Catalog* current = Catalog::Current();
    if ( ! current ) throw CatalogNotSpecified();
    return use( ( *current )[ destClass ] );
}

namespace detail
{

// Helper class that changes the current catalog on the ctor and
// restores the previous on the dtor
class Context
{
public:
    Context( Catalog& c ) :
      firstTime( true )
    {
        previous = Catalog::Current();
        Catalog::Current() = &c;
    }
    ~Context()
    {
        Catalog::Current() = previous;
    }
    bool FirstTime()
    {
        return firstTime;
    }
    void Terminate()
    {
        firstTime = false;
    }
private:
    bool firstTime;
    Catalog* previous;
};

} // namespace detail

/**
 * This preamble creates a scope in which every statement @c use().as().of() will use
 * the catalog @c C withouth the need to specify it every time.
 * So, this code:
 * @code{.cpp}
 * Catalog myCatalog;
 * ...
 * wallaroo_within( myCatalog )
 * {
 *     use( "f136e" ).as( "engine" ).of( "ferrari_f430" );
 *     use( "m139p" ).as( "engine" ).of( "maserati_granturismo" );
 * }
 * @endcode
 * is equivalent to:
 * @code{.cpp}
 * Catalog myCatalog;
 * ...
 * use( myCatalog[ "f136e" ] ).as( "engine" ).of( myCatalog[ "ferrari_f430" ] );
 * use( myCatalog[ "m139p" ] ).as( "engine" ).of( myCatalog[ "maserati_granturismo" ] );
 * @endcode
 *
 * @hideinitializer
 */
#define wallaroo_within( C ) \
    for ( wallaroo::detail::Context context( C ); context.FirstTime(); context.Terminate() )

////////////////////////////////////////////////////////////////////////

} // namespace

#endif
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
#include <stdexcept>
//#include <boost/shared_ptr.hpp>
#include "wireableclass.h"
#include "class.h"

namespace wallaroo
{

namespace detail
{

class ObjectShell
{
public:

    ObjectShell( WireableClass* obj ) : 
      object( obj ) 
    {
        assert( obj != NULL );
    }

    void Wire( const std::string& role, ObjectShell resource )
    {
        object -> Wire( role, resource.object );
    }

    /** Converts the contained object to the type T
    * @return the converted object.
    * @throw std::bad_cast if the contained object is not a subclass of T
    */
    template < class T >
    operator T*()
    {
        //T* result = boost::dynamic_pointer_cast< T >( object );
        T* result = dynamic_cast< T* >( object );
        if ( result == NULL ) throw std::bad_cast();
        return result;
    }

private:
    WireableClass* object;
};

}



/**
 * Catalog of the objects available for the application.
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
    * @throw std::range_error if the element does not exist in the catalog.
    */
    detail::ObjectShell operator [] ( const std::string& itemId )
    {
        Objects::iterator i = objects.find( itemId );
        if ( i == objects.end() ) throw std::range_error( itemId + " not found in the catalog" );
        return detail::ObjectShell( i -> second );
    }

    /** Add an element to the catalog
    * @param id the name of the element to add
    * @param obj the element to add
    * @throw std::range_error if an element with the name @c id is already in the catalog
    */
    void Add( const std::string& id, WireableClass* obj )
    {
        std::pair< Objects::iterator, bool > result = 
            objects.insert( std::make_pair( id, obj ) );
        if ( ! result.second ) throw std::range_error( id + " already in the catalog" );
    }

    /** Instantiate a class with a 2 parameters constructor and add it to the catalog
    * @param id the name of the element to create and add
    * @param className the name of the class to instantiate
    * @param p1 The first parameter of the class constructor
    * @param p2 The second parameter of the class constructor
    * @throw std::range_error if an element with the name @c id is already in the catalog or the @c className class has not been registered
    */
    template < class P1, class P2 >
    void Create2( const std::string& id, const std::string& className, const P1& p1, const P2& p2 )
    {
        typedef Class< WireableClass, P1, P2 > C;
        C c = C::ForName( className );
        std::auto_ptr< WireableClass > obj = c.NewInstance( p1, p2 );
        if ( obj.get() == NULL ) throw std::range_error( className + " not registered" );
        Add( id, obj.release() );
    }

    /** Instantiate a class with a 1 parameters constructor and add it to the catalog
    * @param id the name of the element to create and add
    * @param className the name of the class to instantiate
    * @param p1 The parameter of the class constructor
    * @throw std::range_error if an element with the name @c id is already in the catalog or the @c className class has not been registered
    */
    template < class P >
    void Create1( const std::string& id, const std::string& className, const P& p )
    {
        typedef Class< WireableClass, P, void > C;
        C c = C::ForName( className );
        std::auto_ptr< WireableClass > obj = c.NewInstance( p );
        if ( obj.get() == NULL ) throw std::range_error( className + " not registered" );
        Add( id, obj.release() );
    }

    /** Instantiate a class with 0 parameters constructor and add it to the catalog
    * @param id the name of the element to create and add
    * @param className the name of the class to instantiate
    * @throw std::range_error if an element with the name @c id is already in the catalog or the @c className class has not been registered
    */
    void Create( const std::string& id, const std::string& className )
    {
        typedef Class< WireableClass, void, void > C;
        C c = C::ForName( className );
        std::auto_ptr< WireableClass > obj = c.NewInstance();
        if ( obj.get() == NULL ) throw std::range_error( className + " not registered" );
        Add( id, obj.release() );
    }

private:
    typedef std::map< std::string, WireableClass* > Objects;
    Objects objects;
};


} // namespace

#endif
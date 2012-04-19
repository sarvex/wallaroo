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

#ifndef CATALOG_H_
#define CATALOG_H_

#include <string>
#include <map>
#include <typeinfo>
#include <cassert>
//#include <boost/shared_ptr.hpp>
#include "wireableclass.h"
#include "class.h"

class ObjectShell
{
public:
    ObjectShell( WireableClass* obj ) : 
      object( obj ) 
    {
        assert( obj != NULL );
    }
    void Wire( const std::string& role, ObjectShell& resource )
    {
        object -> Wire( role, resource.object );
    }
    template < class T >
    operator T*() throw ( std::bad_cast )
    {
        //T* result = boost::dynamic_pointer_cast< T >( object );
        T* result = dynamic_cast< T* >( object );
        if ( result == NULL ) throw std::bad_cast();
        return result;
    }
private:
    WireableClass* object;
};


class Catalog
{
public:
    ObjectShell operator [] ( const std::string& itemId ) throw ( std::range_error )
    {
        Objects::iterator i = objects.find( itemId );
        if ( i == objects.end() ) throw std::range_error( itemId + " not found in the catalog" );
        return ObjectShell( i -> second );
    }
    void Add( const std::string& id, WireableClass* obj ) throw ( std::range_error )
    {
        std::pair< Objects::iterator, bool > result = 
            objects.insert( std::make_pair( id, obj ) );
        if ( ! result.second ) throw std::range_error( id + " already in the catalog" );
    }
    template < class P1, class P2 >
    void Create2( const std::string& id, const std::string& className, const P1& p1, const P2& p2 ) throw ( std::range_error )
    {
        typedef Class< WireableClass, P1, P2 > C;
        C c = C::ForName( className );
        std::auto_ptr< WireableClass > obj = c.NewInstance( p1, p2 );
        if ( obj.get() == NULL ) throw std::range_error( className + " not registered" );
        Add( id, obj.release() );
    }
    template < class P >
    void Create1( const std::string& id, const std::string& className, const P& p ) throw ( std::range_error )
    {
        typedef Class< WireableClass, P, void > C;
        C c = C::ForName( className );
        std::auto_ptr< WireableClass > obj = c.NewInstance( p );
        if ( obj.get() == NULL ) throw std::range_error( className + " not registered" );
        Add( id, obj.release() );
    }
    void Create( const std::string& id, const std::string& className ) throw ( std::range_error )
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


#endif
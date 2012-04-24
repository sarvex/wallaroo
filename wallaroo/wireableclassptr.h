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

#ifndef WIREABLECLASSPTR_H_
#define WIREABLECLASSPTR_H_

#include <string>
#include <cassert>
//#include <boost/shared_ptr.hpp>
#include "configurableassociation.h"
#include "wireableclass.h"

namespace wallaroo
{

template < class T >
class WireableClassPtr : public ConfigurableAssociation
{
public:
    WireableClassPtr( const std::string& id, WireableClass* tree ) :
      object( NULL )
    {
        tree -> Register( id, this );
    }
    void Assign( WireableClass* obj ) throw ( std::bad_cast )
    {
        //object = boost::dynamic_pointer_cast< T >( obj );
        object = dynamic_cast< T* >( obj );
        if ( object == NULL ) // bad type!
            throw std::bad_cast();
    }
    T* operator -> ()
    {
        assert( object != NULL );
        return object;
    }
    const T* operator -> () const
    {
        assert( object != NULL );
        return object;
    }
private:
    T* object;
};

} // namespace

#endif
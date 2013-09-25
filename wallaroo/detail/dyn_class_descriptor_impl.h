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

#ifndef WALLAROO_DETAIL_DYN_CLASS_DESCRIPTOR_IMPL_H_
#define WALLAROO_DETAIL_DYN_CLASS_DESCRIPTOR_IMPL_H_

namespace wallaroo
{
namespace detail
{


namespace
{

template < typename T >
void Deleter( T* obj )
{
    delete obj;
}

template < typename T >
cxx0x::shared_ptr< Device > Builder()
{
    return cxx0x::shared_ptr< Device >( new T, Deleter< T > );
}

}

template < typename T >
void Descriptor::Insert( const std::string& className )
{
    Descriptor d;
    d.name = className;
    d.create = Builder< T >;
    DB().push_back( d );
}

std::vector< Descriptor >& Descriptor::DB()
{
    static std::vector< Descriptor > db;
    return db;
}

template < typename T >
DynRegistration< T >::DynRegistration( const std::string& name )
{
    Descriptor::Insert< T >( name );
}


} // namespace detail
} // namespace wallaroo

#endif // WALLAROO_DETAIL_DYN_CLASS_DESCRIPTOR_IMPL_H_

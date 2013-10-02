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

#ifndef WALLAROO_DETAIL_DYN_CLASS_DESCRIPTOR_H_
#define WALLAROO_DETAIL_DYN_CLASS_DESCRIPTOR_H_

#include <string>
#include <vector>
#include "wallaroo/cxx0x.h"

namespace wallaroo
{

class Device; // forward declaration

namespace detail
{

// Store name and factory for a class
struct Descriptor
{
    // Insert a new Descriptor into DB
    template < typename T >
    static void Insert( const std::string& className );

    // Unique instance of Descriptor's DB
    static std::vector< Descriptor >& DB();

    typedef cxx0x::shared_ptr< Device > Ptr;
    typedef cxx0x::function< Ptr() > FactoryMethod;

    FactoryMethod create; // the factory method
    std::string name; // the class name
};

// This is a utility class that register a new Descriptor on its ctor
template < typename T >
class DynRegistration
{
public:
    explicit DynRegistration( const std::string& name );
};

} // namespace detail
} // namespace wallaroo

#endif // WALLAROO_DYN_CLASS_DESCRIPTOR_H_


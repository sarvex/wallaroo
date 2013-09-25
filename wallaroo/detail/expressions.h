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

#ifndef WALLAROO_DETAIL_EXPRESSIONS_H_
#define WALLAROO_DETAIL_EXPRESSIONS_H_

#include <string>
#include "detail/deviceshell.h"
#include "cxx0x.h"
#include "device.h"

namespace wallaroo
{
namespace detail
{

// This is a helper class that provides the result of the use().as() function
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
        srcClass.Wire( attribute, destClass );
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

// This is a helper class that provides the result of the use() function
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
} // namespace

#endif

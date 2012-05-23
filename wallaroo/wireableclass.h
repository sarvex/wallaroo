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

#ifndef WALLAROO_WIREABLECLASS_H_
#define WALLAROO_WIREABLECLASS_H_

#include <string>
#include <map>
#include <stdexcept>
#include "configurableassociation.h"

namespace wallaroo
{


class WireableClass
{
public:
    /** Link the role @c role of this object to the object @c resource.
     * @throw std::range_error if @c role does not exist in this class.
     */
    virtual void Wire( const std::string& role, WireableClass* resource )
    {
        Roles::iterator i = roles.find( role );
        if ( i == roles.end() ) throw std::range_error( role + " not found in the class" );
        ( i -> second ) -> Assign( resource );
    }

    void Register( const std::string& id, ConfigurableAssociation* node )
    {
        roles[ id ] = node;
    }

    virtual ~WireableClass() {}

private:
    typedef std::map< std::string, ConfigurableAssociation* > Roles;
    Roles roles;
};

} // namespace

#endif
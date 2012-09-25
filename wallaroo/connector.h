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

#ifndef WALLAROO_CONNECTOR_H_
#define WALLAROO_CONNECTOR_H_

#include "cxx0x.h"

namespace wallaroo
{

// forward declaration:
class Device;

/**
 * This represents the base class for everything
 * you can "plug" into another "device".
 */
class Connector
{
public:
    /** Plug this Connector into a device
    * @param dev The device you want insert this Connector into
    * @throw WrongType If this Connector could not be inserted into device @c dev 
    */
    virtual void PlugInto( cxx0x::shared_ptr< Device > dev ) = 0;
    virtual ~Connector() {} // note: maybe this dtor can become not virtual?
};

} // namespace

#endif
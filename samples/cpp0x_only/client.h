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

#ifndef CLIENT_H_
#define CLIENT_H_

#include <list>
#include "wallaroo/registered.h"
#include "interface.h"

using namespace wallaroo;

class Client : public Device // we derive from Device to make this class registrable
{
public:
    Client();
    virtual void G();
    virtual ~Client();
private:
    Plug< Interface > relation;
    Plug< Interface, optional > optionalRelation;
    Plug< Interface, collection > relationVector; // std::vector by default
    Plug< Interface, collection, std::list > relationList; // you can specify a different container
    Plug< Interface, bounded_collection< 1, 10 > > relationBoundedVector; // you can specify the range 
};

#endif

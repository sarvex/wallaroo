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

#include <iostream>
#include "client.h"

WALLAROO_REGISTER( Client )

Client::Client() :
    relation( "relation", RegistrationToken() ),
    optionalRelation( "optionalRelation", RegistrationToken() ),
    relationVector( "relationVector", RegistrationToken() ),
    relationList( "relationList", RegistrationToken() ),
    relationBoundedVector( "relationBoundedVector", RegistrationToken() )
{
    std::cout << this << " Client::Client()" << std::endl;
}

void Client::G()
{
    std::cout << this << " Client::G()" << std::endl;

    relation -> F();

    for ( 
        Plug< Interface, collection, std::list >::iterator i = relationList.begin();
        i != relationList.end();
        ++i )
    {
        cxx0x::shared_ptr< Interface > s = i -> lock();
        if ( s ) 
            s -> F();
        else
            std::cerr << "an element in relationList has been deleted!" << std::endl;
    }

    for ( size_t i = 0; i < relationVector.size(); ++i )
    {
        cxx0x::shared_ptr< Interface > s = relationVector[ i ].lock();
        if ( s ) 
            s -> F();
        else
            std::cerr << "an element in relationVector has been deleted!" << std::endl;
    }
}

Client::~Client()
{
}


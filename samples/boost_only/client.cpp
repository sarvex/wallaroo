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
    x( "x", RegistrationToken() ),
    xList( "xList", RegistrationToken() )
{
    std::cout << this << " Client::Client()" << std::endl;
}

void Client::G()
{
    std::cout << this << " Client::G()" << std::endl;

    x -> F();

    for ( Plug< Interface, collection >::iterator i = xList.begin(); i != xList.end(); ++i )
    {
        boost::shared_ptr< Interface > s = i -> lock();
        if ( s ) 
            s -> F();
        else
            std::cerr << "an element in MultiplePlug has been deleted!" << std::endl;
    }
}

Client::~Client()
{
}


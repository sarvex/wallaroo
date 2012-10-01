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
#include "wallaroo/catalog.h"
#include "a.h"

// Wallaroo library is embedded in the wallaroo namespace
using namespace wallaroo;

int main( int argc, char* argv[] )
{
    // A catalog contains the objects used by our app
    Catalog catalog;

    // You can create the objects you need in this way
    catalog.Create( "c", "C" );
    catalog.Create( "c1", "C" );
    catalog.Create( "c2", "C" );
    catalog.Create( "a", "B" );

    // You can wire the objects.
    use( catalog[ "c" ] ).as( "x" ).of( catalog[ "a" ] ); // this means a.x = c
    // You can also wire lists.
    use( catalog[ "c1" ).as( "xList" ).of( "a" ); // this means a.xList.push_back( c1 )
    use( catalog[ "c2" ).as( "xList" ).of( "a" ); // this means a.xList.push_back( c2 )

    // You can retrieve an object from the catalog using
    // the operator [].
    // The catalog has the ownership of the objects contained
    // so the operator [] returns a shared pointer
    std::shared_ptr< A > a = catalog[ "a" ];

    // Finally, you can use your objects in the standard way
    a -> F();

    // Wait for exit acknowledge
    std::cout << "Press Enter to end the program." << std::endl;
    std::cin.get();

    return 0;
}

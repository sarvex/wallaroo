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
#include "interface.h"
#include "client.h"


// Wallaroo library is embedded in the wallaroo namespace
using namespace wallaroo;

int main( int argc, char* argv[] )
{
    // A catalog contains the objects used by our app
    Catalog catalog;

    // You can create the objects you need in this way
    catalog.Create( "c", "Client" );
    catalog.Create( "c1", "Client" );
    catalog.Create( "c2", "Client" );
    catalog.Create( "base1", "Base" );
    catalog.Create( "base2", "Base" );
    catalog.Create( "derived1", "Derived" );
    catalog.Create( "derived2", "Derived" );

    // You can wire the objects.
    
    // the catalog is specified every time:
    use( catalog[ "base1" ] ).as( "x" ).of( catalog[ "c" ] ); // this means c.x = base1
    use( catalog[ "base2" ] ).as( "x" ).of( catalog[ "c1" ] ) ; // this means c1.x = base2

    // the catalog is specified only once:
    wallaroo_within( catalog )
    {
        use( "derived1" ).as( "x" ).of( "c2" );  // this means c2.x = derived1
        // You can also wire lists.
        use( "base1" ).as( "xList" ).of( "c" ); // this means c.xList.push_back( base1 )
        use( "derived1" ).as( "xList" ).of( "c" ); // this means c.xList.push_back( derived1 )
        use( "derived2" ).as( "xList" ).of( "c" ); // this means c.xList.push_back( derived2 )
    }

    // check if all plugs are wired
    assert( catalog.IsWiringOk() );

    // You can retrieve an object from the catalog using
    // the operator [].
    // The catalog has the ownership of the objects contained
    // so the operator [] returns a shared pointer
    boost::shared_ptr< Client > c = catalog[ "c" ];
    boost::shared_ptr< Interface > i = catalog[ "derived2" ];

    // Finally, you can use your objects in the standard way
    c -> G();
    i -> F();

    // Wait for exit
    std::cout << "Press Enter to end the program." << std::endl;
    std::cin.get();

    return 0;
}

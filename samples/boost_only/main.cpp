/*******************************************************************************
 * wallaroo - A library for configurable creation and wiring of C++ classes.
 * Copyright (C) 2012 Daniele Pallastrelli
 *
 * This file is part of wallaroo.
 * For more information, see http://wallaroo.googlecode.com/
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

#include <iostream>
#include <boost/shared_ptr.hpp> // for this sample, we use boost instead of c++11
#include "wallaroo/catalog.h"
#include "interface.h"
#include "client.h"

// Wallaroo library is embedded in the wallaroo namespace
using namespace wallaroo;

int main( int argc, char* argv[] )
{
    try
    {

        // A catalog contains the objects used by our app
        Catalog catalog;

        // You can create the objects you need in this way
        catalog.Create( "c1", "Client" );
        catalog.Create( "c2", "Client" );
        catalog.Create( "base1", "Base" );
        catalog.Create( "base2", "Base" );
        catalog.Create( "base3", "Base" );
        catalog.Create( "derived1", "Derived" );
        catalog.Create( "derived2", "Derived" );
        catalog.Create( "derived3", "Derived" );

        // You can wire the objects.
        
        // the catalog is specified every time:
        use( catalog[ "base1" ] ).as( "relation" ).of( catalog[ "c1" ] ); // this means c1.relation = base1
        use( catalog[ "base2" ] ).as( "relation" ).of( catalog[ "c2" ] ); // this means c2.relation = base2

        // the catalog is specified only once:
        wallaroo_within( catalog )
        {
            use( "derived1" ).as( "optionalRelation" ).of( "c2" );  // this means c2.optionalRelation = derived1
            // You can also wire lists.
            use( "base1" ).as( "relationList" ).of( "c1" ); // this means c1.relationList.push_back( base1 )
            use( "derived1" ).as( "relationList" ).of( "c1" ); // this means c1.relationList.push_back( derived1 )
            use( "derived2" ).as( "relationList" ).of( "c1" ); // this means c1.relationList.push_back( derived2 )
            // and also vectors, of course:
            use( "base1" ).as( "relationVector" ).of( "c2" ); // this means c2.relationVector.push_back( base1 )
            use( "derived1" ).as( "relationVector" ).of( "c2" ); // this means c2.relationVector.push_back( derived1 )

            use ( "base2" ).as( "relationBoundedVector" ).of( "c1" );
            use ( "base1" ).as( "relationBoundedVector" ).of( "c2" );
            use ( "derived2" ).as( "relationBoundedVector" ).of( "c2" );
        }

        // check if all collaborators are wired according to their specification
        assert( catalog.IsWiringOk() );

        // You can retrieve an object from the catalog using
        // the operator [].
        // The catalog has the ownership of the objects contained
        // so the operator [] returns a shared pointer
        boost::shared_ptr< Client > c = catalog[ "c1" ];
        boost::shared_ptr< Interface > i = catalog[ "derived2" ];

        // Finally, you can use your objects in the standard way
        c -> G();
        i -> F();

    }
    catch ( const WallarooError& error )
    {
        std::cerr << "ERROR: " << error.what() << std::endl;
    }

    // Wait for exit
    std::cout << "Press Enter to end the program." << std::endl;
    std::cin.get();

    return 0;
}

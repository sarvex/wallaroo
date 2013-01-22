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
#include "draft.h"

// Wallaroo library is embedded in the wallaroo namespace
using namespace wallaroo;

// By using this namespace, this source code can be used
// with both boost::shared_ptr and std::shared_ptr.
using namespace cxx0x;

int main( int argc, char* argv[] )
{
    Catalog catalog;

    catalog.Create( "draft", "Draft" );
    catalog.Create( "canvas", "ConsoleCanvas" );
    //catalog.Create( "canvas", "CsvFileCanvas", std::string("result.csv") );
    catalog.Create( "rec1", "Rectangle", (unsigned int)10, (unsigned int)10 );
    catalog.Create( "rec2", "Rectangle", (unsigned int)5, (unsigned int)3 );
    catalog.Create( "circle", "Circle", (unsigned int)10 );
    catalog.Create( "triangle1", "Triangle", (unsigned int)16, (unsigned int)10 );
    catalog.Create( "triangle2", "Triangle", (unsigned int)6, (unsigned int)10 );
    
    wallaroo_within( catalog )
    {
        use( "canvas" ).as( "canvas" ).of( "draft" );
        use( "rec1" ).as( "shapes" ).of( "draft" );
        use( "rec2" ).as( "shapes" ).of( "draft" );
        use( "circle" ).as( "shapes" ).of( "draft" );
        use( "triangle1" ).as( "shapes" ).of( "draft" );
        use( "triangle2" ).as( "shapes" ).of( "draft" );
    }

    // check if all plugs are wired
    assert( catalog.IsWiringOk() );

    shared_ptr< Draft > draft = catalog[ "draft" ];

    draft -> Align();
    draft -> Draw();

    // Wait for exit
    std::cout << "Press Enter to end the program." << std::endl;
    std::cin.get();

    return 0;
}

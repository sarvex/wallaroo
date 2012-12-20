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
#include "wallaroo/xmlwiringfile.h"
#include "a.h"

using namespace wallaroo;
using namespace cxx0x; // use std or boost according to your compiler

int main( int argc, char* argv[] )
{
    try
    {
        Catalog catalog;
        XmlWiringFile file( "wiring.xml" );
        file.Fill( catalog );
        
        // throws a WiringError exception if a plug is missed
        catalog.CheckWiring();

        shared_ptr< A > a = catalog[ "a" ];

        a -> F();
    }
    catch ( const WiringError& we )
    {
        std::cerr << "You forgot to wire a plug in the " 
                  << we.Element() << " object. Check the xml file." << std::endl;
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Press Enter to end the program." << std::endl;
    std::cin.get();

    return 0;
}


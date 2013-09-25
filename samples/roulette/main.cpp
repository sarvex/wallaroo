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
#include "wallaroo/dynamic_loader.h"
#include "game.h"

// Wallaroo library is embedded in the wallaroo namespace
using namespace wallaroo;

// By using this namespace, this source code can be used
// with both boost::shared_ptr and std::shared_ptr.
using namespace cxx0x;

int main( int argc, char* argv[] )
{
    try
    {
        // load classes in shared libraries:
        Plugin::Load( "straightbet" + Plugin::Suffix() );
        Plugin::Load( "oddevenbet" + Plugin::Suffix() );
        Plugin::Load( "redblackbet" + Plugin::Suffix() );

        // separate catalogs for application logic and bets
        Catalog logicCatalog;
        Catalog betCatalog;

        logicCatalog.Create( "player", "ConsolePlayer" );
        logicCatalog.Create( "wheel", "Wheel" );
        logicCatalog.Create( "game", "Game" );

        betCatalog.Create( "straightbetconsolefactory", "StraightBetConsoleFactory" );
        betCatalog.Create( "oddevenbetconsolefactory", "OddEvenBetConsoleFactory" );
        betCatalog.Create( "redblackbetconsolefactory", "RedBlackBetConsoleFactory" );

        wallaroo_within( logicCatalog )
        {
            use( "player" ).as( "player" ).of( "game" );
            use( "wheel" ).as( "wheel" ).of( "game" );
        }

        use( betCatalog[ "straightbetconsolefactory" ] ).as( "availableBets" ).of( logicCatalog[ "player" ] );
        use( betCatalog[ "oddevenbetconsolefactory" ] ).as( "availableBets" ).of( logicCatalog[ "player" ] );
        use( betCatalog[ "redblackbetconsolefactory" ] ).as( "availableBets" ).of( logicCatalog[ "player" ] );

        // check if all plugs are wired
        assert( logicCatalog.IsWiringOk() );
        assert( betCatalog.IsWiringOk() );

        shared_ptr< Game > game = logicCatalog[ "game" ];

        std::cout << "Welcome to the roulette game" << std::endl;
        while ( true )
            game -> Play();

    }
    catch ( const WallarooError& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

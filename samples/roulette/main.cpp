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
        // load classes in shared libraries (in the current dir):
        Plugin::Load( "./straightbet" + Plugin::Suffix() );
        Plugin::Load( "./oddevenbet" + Plugin::Suffix() );
        Plugin::Load( "./redblackbet" + Plugin::Suffix() );

        // separate catalogs for application logic and bets
        Catalog logicCatalog;
        Catalog betCatalog;

        logicCatalog.Create( "player", "ConsolePlayer" );
        logicCatalog.Create( "wheel", "Wheel" );
        shared_ptr< Game > game = logicCatalog.Create( "game", "Game" );

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

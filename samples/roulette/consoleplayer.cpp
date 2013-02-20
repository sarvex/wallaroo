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
#include <string>
#include "consoleplayer.h"

WALLAROO_REGISTER( ConsolePlayer );

ConsolePlayer::ConsolePlayer() :
    availableBets( "availableBets", RegistrationToken() )
{
}

cxx0x::shared_ptr< Bet > ConsolePlayer::NextBet()
{
    // print the usage for this bet:
    std::cout << "Place your bet. Bets availables:" << std::endl;
    for ( BetFactories::iterator i = availableBets.begin(); i != availableBets.end(); ++i )
    {
        cxx0x::shared_ptr< BetConsoleFactory > f = i -> lock();
        if ( f )
            std::cout << "\t" << f -> Help() << std::endl;
    }
    std::cout << "\t(exit to quit.)" << std::endl;
    std::cout << "where bin = {00,0,1,2,3,..,35,36}" << std::endl << std::endl;

    // parse the user string:
    std::string line;
    getline( std::cin, line );

    if ( line == "exit" )
        exit( 0 );

    for ( BetFactories::iterator i = availableBets.begin(); i != availableBets.end(); ++i )
    {
        cxx0x::shared_ptr< BetConsoleFactory > f = i -> lock();
        if ( f )
        {
            cxx0x::shared_ptr< Bet > bet = f -> Create( line );
            if ( bet ) return bet;
        }
    }

    std::cout << std::endl << "command unknown" << std::endl << std::endl;
    return cxx0x::shared_ptr< Bet >();
}



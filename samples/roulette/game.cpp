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
#include "game.h"
#include "player.h"
#include "wheel.h"
#include "currency.h"
#include "bet.h"

WALLAROO_REGISTER( Game );

Game::Game() :
    player( "player", RegistrationToken() ),
    wheel( "wheel", RegistrationToken() )
{
}

void Game::Play()
{
    cxx0x::shared_ptr< Bet > bet = player -> NextBet();
    if ( bet )
    {
        std::cout << "Your bet: " << bet -> Description() << std::endl;
        Bin bin = wheel -> Spin();
        std::cout << "Outcome: " << Value( bin ) << std::endl;
        Currency amount = bet -> Table( bin );
        std::cout << "Your table: " << amount << std::endl;
    }
}


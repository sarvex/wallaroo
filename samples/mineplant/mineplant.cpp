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

#include "wallaroo/xmlconfiguration.h"
#include "mineplant.h"
#include "sumppump.h"
#include "gasalarm.h"

MinePlant::MinePlant( const std::string& xmlFile )
{
    XmlConfiguration cfgFile( xmlFile );
    cfgFile.Fill( catalog );
    catalog.CheckWiring();
}

void MinePlant::Run()
{
    using namespace cxx0x; // use std or boost according to your compiler

    shared_ptr< SumpPump > pump;
    shared_ptr< GasAlarm > gasAlarm;

    try
    {
        pump = catalog[ "pump" ];
        std::cout << "The plant has a pump." << std::endl;
    }
    catch ( const ElementNotFound& e )
    {
        std::cout << "No pump in the plant." << std::endl;
    }

    try
    {
        gasAlarm = catalog[ "gasAlarm" ];
        std::cout << "The plant has a gas alarm." << std::endl;
    }
    catch ( const ElementNotFound& e )
    {
        std::cout << "No gas alarm in the plant." << std::endl;
    }

    while ( true )
	{
        if ( pump ) pump -> Drain();
		if ( gasAlarm ) gasAlarm -> Watch();
	}
}


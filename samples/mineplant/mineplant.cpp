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

#include "mineplant.h"
#include "wallaroo/catalog.h"

// Define to compile a wiring without the check of the gas
// #define NO_GAS_CHECK

MinePlant::MinePlant()
{
    Catalog catalog;

    catalog.Create2( "lowLevelInput", "ConstDigitalInput", std::string( "/dev/lpt1" ), 0u );
    catalog.Create2( "highLevelInput", "ConstDigitalInput", std::string( "/dev/lpt1" ), 1u );
    catalog.Create( "lowSensor", "LevelSensor" );
    catalog.Create( "highSensor", "LevelSensor" );
    catalog.Create( "probe", "LevelSensorBasedSumpProbe" );
    catalog.Create2( "output", "ConsoleDigitalOutput", std::string( "/dev/lpt2" ), 0u );
#ifdef NO_GAS_CHECK
    catalog.Create( "engine", "PumpEngine" );
#else
    catalog.Create( "engine", "SafeEngine" );
    catalog.Create1( "gasSensor", "ThresholdGasSensor", 5u );
    catalog.Create1( "gasInput", "ConstAnalogInput", std::string( "/dev/lpt3") );
#endif
    catalog.Create( "pump", "SumpPump" );
    pump = catalog[ "pump" ];

#if 0 // fa scattare un'eccezione
    SumpPump* p = catalog[ "pippo" ];
    assert( p == pump );
#endif

    catalog[ "lowSensor" ].Wire( "input", catalog[ "lowLevelInput" ] );
    catalog[ "highSensor" ].Wire( "input", catalog[ "highLevelInput" ] );

    catalog[ "probe" ].Wire( "lowSensor", catalog[ "lowSensor" ] );
    catalog[ "probe" ].Wire( "highSensor", catalog[ "highSensor" ] );

    catalog[ "engine" ].Wire( "output", catalog[ "output" ] );
#ifndef NO_GAS_CHECK
    catalog[ "engine" ].Wire( "sensor", catalog[ "gasSensor" ] );
    catalog[ "gasSensor" ].Wire( "input", catalog[ "gasInput" ] );
#endif

    catalog[ "pump" ].Wire( "probe", catalog[ "probe" ] );
#if 1 // correct
    catalog[ "pump" ].Wire( "engine", catalog[ "engine" ] );
#else // bad type
    catalog[ "pump" ].Wire( "engine", catalog[ "probe" ] );
#endif
}

void MinePlant::Run()
{
    while ( true )
        pump -> Drain();
}

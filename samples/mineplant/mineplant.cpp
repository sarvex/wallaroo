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

    catalog.Create( "lowLevelInput", "ConstDigitalInput", std::string( "/dev/lpt1" ), 0u );
    catalog.Create( "highLevelInput", "ConstDigitalInput", std::string( "/dev/lpt1" ), 1u );
    catalog.Create( "lowSensor", "LevelSensor" );
    catalog.Create( "highSensor", "LevelSensor" );
    catalog.Create( "probe", "TwoLevelSumpProbe" );
    catalog.Create( "output", "ConsoleDigitalOutput", std::string( "/dev/lpt2" ), 0u );
#ifdef NO_GAS_CHECK
    catalog.Create( "engine", "PumpEngine" );
#else
    catalog.Create( "engine", "SafeEngine" );
    catalog.Create( "airFlowSensor", "ThresholdGasSensor", 5u );
    catalog.Create( "COSensor", "ThresholdGasSensor", 25u );
    catalog.Create( "methaneSensor", "ThresholdGasSensor", 10u );
    catalog.Create( "airFlowInput", "ConstAnalogInput", std::string( "/dev/lpt3") );
    catalog.Create( "COInput", "ConstAnalogInput", std::string( "/dev/lpt4") );
    catalog.Create( "methaneInput", "ConstAnalogInput", std::string( "/dev/lpt5") );
    catalog.Create( "alarmOutput", "ConsoleDigitalOutput", std::string( "/dev/lpt2" ), 1u );
    catalog.Create( "gasAlarm", "GasAlarm" );
	gasAlarm = catalog[ "gasAlarm" ];
    catalog.Create( "alarm", "Alarm" );
#endif
    catalog.Create( "pump", "SumpPump" );
    pump = catalog[ "pump" ];

#if 0 // fa scattare un'eccezione
    SumpPump* p = catalog[ "pippo" ];
    assert( p == pump );
#endif

    catalog[ "lowSensor" ].Plug( "input" ).Into( catalog[ "lowLevelInput" ] );
    catalog[ "highSensor" ].Plug( "input" ).Into( catalog[ "highLevelInput" ] );

    catalog[ "probe" ].Plug( "lowSensor" ).Into( catalog[ "lowSensor" ] );
    catalog[ "probe" ].Plug( "highSensor" ).Into( catalog[ "highSensor" ] );

    catalog[ "engine" ].Plug( "output" ).Into( catalog[ "output" ] );

#ifndef NO_GAS_CHECK
    catalog[ "airFlowSensor" ].Plug( "input" ).Into( catalog[ "airFlowInput" ] );
    catalog[ "COSensor" ].Plug( "input" ).Into( catalog[ "COInput" ] );
    catalog[ "methaneSensor" ].Plug( "input" ).Into( catalog[ "methaneInput" ] );

    catalog[ "engine" ].Plug( "sensor" ).Into( catalog[ "methaneSensor" ] );

	catalog[ "alarm" ].Plug( "output" ).Into( catalog[ "alarmOutput" ] );

    catalog[ "gasAlarm" ].Plug( "sensors" ).Into( catalog[ "airFlowSensor" ] );
    catalog[ "gasAlarm" ].Plug( "sensors" ).Into( catalog[ "COSensor" ] );
    catalog[ "gasAlarm" ].Plug( "sensors" ).Into( catalog[ "methaneSensor" ] );

    catalog[ "gasAlarm" ].Plug( "alarm" ).Into( catalog[ "alarm" ] );
#endif

    catalog[ "pump" ].Plug( "probe" ).Into( catalog[ "probe" ] );
#if 1 // correct
    catalog[ "pump" ].Plug( "engine" ).Into( catalog[ "engine" ] );
#else // bad type
    catalog[ "pump" ].Plug( "engine" ).Into( catalog[ "probe" ] );
#endif
}

void MinePlant::Run()
{
    while ( true )
	{
        pump -> Drain();
#ifndef NO_GAS_CHECK
		gasAlarm -> Watch();
#endif
	}
}


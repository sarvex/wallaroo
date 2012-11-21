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
#include "sumppump.h"
#include "gasalarm.h"

// Define to compile a wiring with the check of the gas
#define GAS_CHECK

MinePlant::MinePlant()
{
    // object creation:

    catalog.Create( "lowLevelInput", "ConstDigitalInput", std::string( "/dev/lpt1" ), 0u );
    catalog.Create( "highLevelInput", "ConstDigitalInput", std::string( "/dev/lpt1" ), 1u );
    catalog.Create( "lowSensor", "LevelSensor" );
    catalog.Create( "highSensor", "LevelSensor" );
    catalog.Create( "probe", "TwoLevelSumpProbe" );
    catalog.Create( "output", "ConsoleDigitalOutput", std::string( "/dev/lpt2" ), 0u );
#ifdef GAS_CHECK
    catalog.Create( "engine", "SafeEngine" );
    catalog.Create( "airFlowSensor", "ThresholdGasSensor", 5u );
    catalog.Create( "COSensor", "ThresholdGasSensor", 25u );
    catalog.Create( "methaneSensor", "ThresholdGasSensor", 10u );
    catalog.Create( "airFlowInput", "ConstAnalogInput", std::string( "/dev/lpt3") );
    catalog.Create( "COInput", "ConstAnalogInput", std::string( "/dev/lpt4") );
    catalog.Create( "methaneInput", "ConstAnalogInput", std::string( "/dev/lpt5") );
    catalog.Create( "alarmOutput", "ConsoleDigitalOutput", std::string( "/dev/lpt2" ), 1u );
    catalog.Create( "gasAlarm", "GasAlarm" );
    catalog.Create( "alarm", "Alarm" );
#else
    catalog.Create( "engine", "PumpEngine" );
#endif
    catalog.Create( "pump", "SumpPump" );

    // object wiring:

    wallaroo_within( catalog )
    {
        use( "lowLevelInput" ).as( "input" ).of( "lowSensor" );
        use( "highLevelInput" ).as( "input" ).of( "highSensor" );

        use( "lowSensor" ).as( "lowSensor" ).of( "probe" );
        use( "highSensor" ).as( "highSensor" ).of( "probe" );

        use( "output" ).as( "output" ).of( "engine" );

#ifdef GAS_CHECK
        use( "airFlowInput" ).as( "input" ).of( "airFlowSensor" );
        use( "COInput" ).as( "input" ).of( "COSensor" );
        use( "methaneInput" ).as( "input" ).of( "methaneSensor" );

        use( "methaneSensor" ).as( "sensor" ).of( "engine" );

        use( "alarmOutput" ).as( "output" ).of( "alarm" );

        use( "airFlowSensor" ).as( "sensors" ).of( "gasAlarm" );
        use( "COSensor" ).as( "sensors" ).of( "gasAlarm" );
        use( "methaneSensor" ).as( "sensors" ).of( "gasAlarm" );

        use( "alarm" ).as( "alarm" ).of( "gasAlarm" );
#endif

        use( "probe" ).as( "probe" ).of( "pump" );
        use( "engine" ).as( "engine" ).of( "pump" );
    }
    
    // check if all plugs are wired
    assert( catalog.IsWiringOk() );
}

void MinePlant::Run()
{
    boost::shared_ptr< SumpPump > pump = catalog[ "pump" ];
    boost::shared_ptr< GasAlarm > gasAlarm = catalog[ "gasAlarm" ];

    while ( true )
	{
        pump -> Drain();
#ifdef GAS_CHECK
		gasAlarm -> Watch();
#endif
	}
}


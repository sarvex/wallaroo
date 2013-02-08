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
#include "console.h"

using namespace std;

WALLAROO_REGISTER( Console )

void Console::Dump()
{
    // print all values:
    cout << "=============================" << endl;
    cout << "Current values:" << endl << endl;
    for ( Dictionary::const_iterator i = values.begin(); i != values.end(); ++i )
        cout << i -> first << " = " << i -> second << endl;
    cout << "=============================" << endl;
}

void Console::GetInput()
{
    // print the usage
    cout << endl;
    cout << "Please, set a value" << endl;
    cout << "syntax: <key> <value>" << endl;
    cout << "(exit to quit)" << endl << endl;;
    cout << " > ";

    // parse the user string:
    string line;
    getline( cin, line );

    if ( line == "exit" )
        exit( 0 );

    stringstream ss( line );

    string key;
    int value;

    if ( ss.eof() ) return;

    ss >> key;
    if ( ss.eof() ) 
    {
        values[ key ] = 0;
        return;
    }

    ss >> value;
    if ( ss.fail() || !ss.eof() )
    {
        values[ key ] = 0;
        return;
    }

    values[ key ] = value;

    cout << "input " << key << " set to value " << values[ key ] << endl;
    cout << endl;
}

int Console::GetMeasure( const std::string& sensor )
{
    if ( ++counter % 100 == 0 )
    {
        Dump();
        GetInput();
    }

    return values[ sensor ];
}

void Console::SetQuantity( const std::string& actuator, int value )
{
    values[ actuator ] = value;
}


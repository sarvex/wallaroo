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

#ifndef BIN_H_
#define BIN_H_

#include <string>

class Bin
{
public:

    static Bin _00;
    static Bin _0;
    static Bin _1;
    static Bin _2;
    static Bin _3;
    static Bin _4;
    static Bin _5;
    static Bin _6;
    static Bin _7;
    static Bin _8;
    static Bin _9;
    static Bin _10;
    static Bin _11;
    static Bin _12;
    static Bin _13;
    static Bin _14;
    static Bin _15;
    static Bin _16;
    static Bin _17;
    static Bin _18;
    static Bin _19;
    static Bin _20;
    static Bin _21;
    static Bin _22;
    static Bin _23;
    static Bin _24;
    static Bin _25;
    static Bin _26;
    static Bin _27;
    static Bin _28;
    static Bin _29;
    static Bin _30;
    static Bin _31;
    static Bin _32;
    static Bin _33;
    static Bin _34;
    static Bin _35;
    static Bin _36;

    explicit Bin( const std::string& value );
    Bin( const Bin& bin );
    Bin& operator=( const Bin& rhs );
    const std::string& Value() const;
    bool operator==( const Bin& other) const;

private:
    std::string value;

};

#endif

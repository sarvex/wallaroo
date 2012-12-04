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

#include <boost/test/unit_test.hpp>

#include "wallaroo/registered.h"
#include "wallaroo/catalog.h"
#include "wallaroo/xmlwiringfile.h"
#include "wallaroo/cxx0x.h"

using namespace wallaroo;
using namespace cxx0x;

// some classes:

class I4 : public Device
{
public:
    virtual int F() = 0;
    virtual ~I4() {}
};

class A4 : public I4
{
public:
    virtual int F() { return 5; }
    virtual ~A4() {}
};

REGISTER( A4 )

class B4 : public A4
{
public:
    virtual int F() { return 10; }
    virtual ~B4() {}
};

REGISTER( B4 )

class C4 : public Device
{
public:
    C4() : x( "x", RegistrationToken() ) {}
    virtual int F() { return x -> F(); }
    virtual ~C4() {}
private:
    Plug< I4 > x;
};

REGISTER( C4 )

class D4 : public Device
{
public:
    D4( const std::string& _s, int _i ) : 
        container( "container", RegistrationToken() ),
        s( _s ),
        ii( _i )
    {
    }
    std::string String() const { return s; }
    int Int() const { return ii; }
    int F() const
    {
        int sum = 0;
        for ( Plug< I4, collection >::const_iterator i = container.begin(); i != container.end(); ++i )
        {
            sum += i -> lock() -> F();
        }
        return sum;
    }
private:
    Plug< I4, collection > container;
    const std::string s;
    const int ii;
};

REGISTER( D4, std::string, int )

// tests

BOOST_AUTO_TEST_SUITE( XmlLoader )

BOOST_AUTO_TEST_CASE( XmlNotFound )
{
    BOOST_CHECK_THROW( XmlWiringFile( "UnexistentFile.xml" ), WrongFile );
}

BOOST_AUTO_TEST_CASE( XmlOk )
{
    XmlWiringFile file( "test_xml_loader.xml" );
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( file.Fill( catalog ) );

    BOOST_REQUIRE( catalog.IsWiringOk() );

    shared_ptr< C4 > c1 = catalog[ "c1" ];
    BOOST_CHECK( c1 -> F() == 5 );

    shared_ptr< C4 > c2 = catalog[ "c2" ];
    BOOST_CHECK( c2 -> F() == 10 );

    shared_ptr< D4 > d = catalog[ "d" ];
    BOOST_CHECK( d -> String() == "mystring" );
    BOOST_CHECK( d -> Int() == 34 );
    BOOST_CHECK( d -> F() == 15 );
}


BOOST_AUTO_TEST_SUITE_END()
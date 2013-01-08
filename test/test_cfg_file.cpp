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
#include "wallaroo/jsonconfiguration.h"
#include "wallaroo/xmlconfiguration.h"
#include "wallaroo/cxx0x.h"

using namespace wallaroo;
using namespace cxx0x;

// some classes:

class I5 : public Device
{
public:
    virtual int F() = 0;
    virtual ~I5() {}
};

class A5 : public I5
{
public:
    virtual int F() { return 5; }
    virtual ~A5() {}
};

WALLAROO_REGISTER( A5 )

class B5 : public A5
{
public:
    B5( int _x ) : x( _x ) {}
    virtual int F() { return x; }
    virtual ~B5() {}
private:
    const int x;
};

WALLAROO_REGISTER( B5, int )

class C5 : public Device
{
public:
    C5( unsigned int ) : x( "x", RegistrationToken() ) {}
    virtual int F() { return x -> F(); }
    virtual ~C5() {}
private:
    Plug< I5 > x;
};

WALLAROO_REGISTER( C5, unsigned int )

class D5 : public Device
{
public:
    D5( const std::string& _s, int _i ) : 
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
        for ( Plug< I5, collection >::const_iterator i = container.begin(); i != container.end(); ++i )
        {
            sum += i -> lock() -> F();
        }
        return sum;
    }
private:
    Plug< I5, collection > container;
    const std::string s;
    const int ii;
};

WALLAROO_REGISTER( D5, std::string, int )


class E5 : public Device
{
public:
    E5( unsigned int, double ) {}
};

WALLAROO_REGISTER( E5, unsigned int, double )

// tests

BOOST_AUTO_TEST_SUITE( CfgFile )

BOOST_AUTO_TEST_CASE( JsonNotFound )
{
    BOOST_CHECK_THROW( JsonConfiguration( "UnexistentFile.json" ), WrongFile );
}

BOOST_AUTO_TEST_CASE( XmlNotFound )
{
    BOOST_CHECK_THROW( XmlConfiguration( "UnexistentFile.xml" ), WrongFile );
}

static void TestContent( Catalog& catalog )
{
    BOOST_REQUIRE( catalog.IsWiringOk() );

    shared_ptr< C5 > c1 = catalog[ "c1" ];
    BOOST_CHECK( c1 -> F() == 5 );

    shared_ptr< C5 > c2 = catalog[ "c2" ];
    BOOST_CHECK( c2 -> F() == 10 );

    shared_ptr< D5 > d = catalog[ "d" ];
    BOOST_CHECK( d -> String() == "mystring" );
    BOOST_CHECK( d -> Int() == 34 );
    BOOST_CHECK( d -> F() == 15 );

    shared_ptr< E5 > e = catalog[ "e" ];
}

BOOST_AUTO_TEST_CASE( JsonOk )
{
    JsonConfiguration file( "test_json.json" );
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( file.Fill( catalog ) );
    TestContent( catalog );
}

BOOST_AUTO_TEST_CASE( XmlOk )
{
    XmlConfiguration file( "test_xml.xml" );
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( file.Fill( catalog ) );
    TestContent( catalog );
}

BOOST_AUTO_TEST_SUITE_END()
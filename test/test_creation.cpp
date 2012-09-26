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

using namespace wallaroo;
using namespace cxx0x;

// some classes:

REGISTERED_CLASS( A1, void, void ), public Device
{
public:
    int F() { return 5; }
    ~A1() {}
};

REGISTER( A1, void, void )

REGISTERED_CLASS( B1, int, std::string ), public Device
{
public:
    B1( int _x, const std::string& _y ) : x( _x ), y( _y ) {}
    int GetX() const { return x; }
    const std::string& GetY() const { return y; }
    ~B1() {}
private:
    const int x;
    const std::string y;
};

REGISTER( B1, int, std::string )

// tests

BOOST_AUTO_TEST_SUITE( Creation )

BOOST_AUTO_TEST_CASE( creationOk )
{
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A1" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
}

BOOST_AUTO_TEST_CASE( creationUnexistentObj )
{
    Catalog catalog;
    BOOST_CHECK_THROW( catalog.Create( "a", "Unknown" ), ElementNotFound );
}

BOOST_AUTO_TEST_CASE( duplicatedElement )
{
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A1" ) );
    BOOST_CHECK_THROW( catalog.Create( "a", "B1", 10, std::string( "hello" ) ), DuplicatedElement );
}

BOOST_AUTO_TEST_CASE( retrieveOk )
{
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A1" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    shared_ptr< A1 > a = catalog[ "a" ];
    BOOST_CHECK( a -> F() == 5 );
}

BOOST_AUTO_TEST_CASE( retrieveKo )
{
    Catalog catalog;
    BOOST_CHECK_THROW( catalog[ "b" ], ElementNotFound );
}

BOOST_AUTO_TEST_CASE( multipleParametersOk )
{
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B1", 10, std::string( "hello" ) ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );
    boost::shared_ptr< B1 > b = catalog[ "b" ];
    BOOST_CHECK( b -> GetX() == 10 );
    BOOST_CHECK( b -> GetY() == "hello" );
}

#if 0 // fails because we use const char* instead of std::string
BOOST_AUTO_TEST_CASE( stringCtor )
{
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B1", 10, "hello" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );
}
#endif

BOOST_AUTO_TEST_SUITE_END()
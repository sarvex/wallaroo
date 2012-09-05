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

// some classes:

//REGISTERED_CLASS( I, void, void ) /*, public Device*/
class I : public Device
{
public:
    virtual int F() = 0;
    virtual ~I() {}
};

REGISTERED_CLASS( AA, void, void ), public I
{
public:
    virtual int F() { return 5; }
    virtual ~AA() {}
};

REGISTER( AA, void, void )

REGISTERED_CLASS( BB, void, void ), public AA
{
public:
    virtual int F() { return 10; }
    virtual ~BB() {}
};

REGISTER( BB, void, void )

REGISTERED_CLASS( CC, void, void ), public Device
{
public:
    CC() : x( "x", this ) {}
    virtual int F() { return x -> F(); }
    virtual ~CC() {}
private:
    Plug< I > x;
};

REGISTER( CC, void, void )

// tests

BOOST_AUTO_TEST_SUITE( Wiring )

BOOST_AUTO_TEST_CASE( wiringOk )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "AA" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "BB" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c1", "CC" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c1" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c2", "CC" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c2" ] );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( use( "b" ).as( "x" ).of( "c1" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "x" ).of( "c2" ) );
    }

    boost::shared_ptr< CC > c1 = catalog[ "c1" ];
    BOOST_CHECK( c1 -> F() == 10 );

    boost::shared_ptr< CC > c2 = catalog[ "c2" ];
    BOOST_CHECK( c2 -> F() == 5 );
}

BOOST_AUTO_TEST_CASE( multipleCatalogs )
{
    Catalog catalog1;
    Catalog catalog2;

    BOOST_REQUIRE_NO_THROW( catalog1.Create( "a1", "AA" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "a1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "b1", "BB" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "b1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "c1", "CC" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "c1" ] );

    BOOST_REQUIRE_NO_THROW( catalog2.Create( "a2", "AA" ) );
    BOOST_REQUIRE_NO_THROW( catalog2[ "a2" ] );
    BOOST_REQUIRE_NO_THROW( catalog2.Create( "c2", "CC" ) );
    BOOST_REQUIRE_NO_THROW( catalog2[ "c2" ] );
    BOOST_REQUIRE_NO_THROW( catalog2.Create( "c22", "CC" ) );
    BOOST_REQUIRE_NO_THROW( catalog2[ "c22" ] );

    wallaroo_within( catalog1 )
    {
        BOOST_REQUIRE_NO_THROW( use( "b1" ).as( "x" ).of( "c1" ) );
    }

    wallaroo_within( catalog2 )
    {
        BOOST_REQUIRE_NO_THROW( use( "a2" ).as( "x" ).of( "c2" ) );
    }

    BOOST_REQUIRE_NO_THROW( use( catalog1[ "a1" ] ).as( "x" ).of( catalog2[ "c22" ] ) );

    boost::shared_ptr< CC > c1 = catalog1[ "c1" ];
    BOOST_CHECK( c1 -> F() == 10 );

    boost::shared_ptr< CC > c2 = catalog2[ "c2" ];
    BOOST_CHECK( c2 -> F() == 5 );

    boost::shared_ptr< CC > c22 = catalog2[ "c22" ];
    BOOST_CHECK( c22 -> F() == 5 );
}

BOOST_AUTO_TEST_CASE( nestedCatalogs )
{
    Catalog catalog1;
    Catalog catalog2;

    BOOST_REQUIRE_NO_THROW( catalog1.Create( "a1", "AA" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "a1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "b1", "BB" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "b1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "c1", "CC" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "c1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "c11", "CC" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "c11" ] );

    BOOST_REQUIRE_NO_THROW( catalog2.Create( "a2", "AA" ) );
    BOOST_REQUIRE_NO_THROW( catalog2[ "a2" ] );
    BOOST_REQUIRE_NO_THROW( catalog2.Create( "c2", "CC" ) );
    BOOST_REQUIRE_NO_THROW( catalog2[ "c2" ] );

    wallaroo_within( catalog1 )
    {
        BOOST_REQUIRE_NO_THROW( use( "b1" ).as( "x" ).of( "c1" ) );
        wallaroo_within( catalog2 )
        {
            BOOST_REQUIRE_NO_THROW( use( "a2" ).as( "x" ).of( "c2" ) );
        }
        BOOST_REQUIRE_NO_THROW( use( "a1" ).as( "x" ).of( "c11" ) );
    }

    boost::shared_ptr< CC > c1 = catalog1[ "c1" ];
    BOOST_CHECK( c1 -> F() == 10 );

    boost::shared_ptr< CC > c2 = catalog2[ "c2" ];
    BOOST_CHECK( c2 -> F() == 5 );

    boost::shared_ptr< CC > c11 = catalog1[ "c11" ];
    BOOST_CHECK( c11 -> F() == 5 );
}

BOOST_AUTO_TEST_SUITE_END()
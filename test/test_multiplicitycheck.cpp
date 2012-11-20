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

REGISTERED_CLASS( A3, void, void ), public Device
{
public:
    virtual ~A3() {}
};

REGISTER( A3, void, void )

REGISTERED_CLASS( B3, void, void ), public Device
{
public:
    B3() : 
        mandatoryAttribute( "mandatoryAttribute", this ),
        optionalAttribute( "optionalAttribute", this ),
        collectionAttribute( "collectionAttribute", this )
    {}
    virtual ~B3() {}
private:
    Plug< A3 > mandatoryAttribute;
    Plug< A3, optional > optionalAttribute;
    Plug< A3, collection > collectionAttribute;
};

REGISTER( B3, void, void )

REGISTERED_CLASS( C3, void, void ), public Device
{
public:
    C3() :
        collectionAttribute1( "collectionAttribute1", this ),
        collectionAttribute13( "collectionAttribute13", this ),
        collectionAttribute33( "collectionAttribute33", this )
    {}
    virtual ~C3() {}
private:
    Plug< A3, bounded_collection< 1 > > collectionAttribute1;
    Plug< A3, bounded_collection< 1, 3 > > collectionAttribute13;
    Plug< A3, bounded_collection< 3, 3 > > collectionAttribute33;
};

REGISTER( C3, void, void )


// tests

BOOST_AUTO_TEST_SUITE( MultiplicityCheck )

BOOST_AUTO_TEST_CASE( checkOk )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a1", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a1" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "a2", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a2" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "a3", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a3" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( use( "a1" ).as( "mandatoryAttribute" ).of( "b" ) );
        BOOST_REQUIRE_NO_THROW( use( "a2" ).as( "optionalAttribute" ).of( "b" ) );
        BOOST_REQUIRE_NO_THROW( use( "a1" ).as( "collectionAttribute" ).of( "b" ) );
        BOOST_REQUIRE_NO_THROW( use( "a2" ).as( "collectionAttribute" ).of( "b" ) );
        BOOST_REQUIRE_NO_THROW( use( "a3" ).as( "collectionAttribute" ).of( "b" ) );
    }

    BOOST_REQUIRE_NO_THROW( catalog.CheckWiring() );
}

BOOST_AUTO_TEST_CASE( checkOptional )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b1", "B3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b1" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b2", "B3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b2" ] );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "mandatoryAttribute" ).of( "b1" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "optionalAttribute" ).of( "b1" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "mandatoryAttribute" ).of( "b2" ) );
    }

    BOOST_REQUIRE_NO_THROW( catalog.CheckWiring() );
}


BOOST_AUTO_TEST_CASE( checkKo )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "optionalAttribute" ).of( "b" ) );
        // mandatoryAttribute not wired!
    }

    BOOST_CHECK_THROW( catalog.CheckWiring(), WiringError );
}


BOOST_AUTO_TEST_CASE( checkCollectionMultiplicityOk )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c", "C3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c" ] );

    wallaroo_within( catalog )
    {
        // 1 in [1,+inf)
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute1" ).of( "c" ) );

        // 2 in [1,3]
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );

        // 3 in [3,3]
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
    }

    BOOST_REQUIRE_NO_THROW( catalog.CheckWiring() );
}

BOOST_AUTO_TEST_CASE( checkCollectionMultiplicityKo1 )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c", "C3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c" ] );

    wallaroo_within( catalog )
    {
        // 0 not in [1,+inf)
        // BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute1" ).of( "c" ) );

        // 2 in [1,3]
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );

        // 3 in [3,3]
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
    }

    BOOST_CHECK_THROW( catalog.CheckWiring(), WiringError );
}

BOOST_AUTO_TEST_CASE( checkCollectionMultiplicityKo2 )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c", "C3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c" ] );

    wallaroo_within( catalog )
    {
        // 1 in [1,+inf)
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute1" ).of( "c" ) );

        // 4 not in [1,3]
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );

        // 3 in [3,3]
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
    }

    BOOST_CHECK_THROW( catalog.CheckWiring(), WiringError );
}

BOOST_AUTO_TEST_CASE( checkCollectionMultiplicityKo3 )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c", "C3" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c" ] );

    wallaroo_within( catalog )
    {
        // 1 in [1,+inf)
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute1" ).of( "c" ) );

        // 2 in [1,3]
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute13" ).of( "c" ) );

        // 1 not in [3,3]
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collectionAttribute33" ).of( "c" ) );
    }

    BOOST_CHECK_THROW( catalog.CheckWiring(), WiringError );
}

BOOST_AUTO_TEST_SUITE_END()
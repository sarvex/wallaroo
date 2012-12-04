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
#include "wallaroo/cxx0x.h"

#include <deque>
#include <vector>
#include <list>

using namespace wallaroo;
using namespace cxx0x;

// some classes:

class I2 : public Device
{
public:
    virtual int F() = 0;
    virtual ~I2() {}
};

class A2 : public I2
{
public:
    virtual int F() { return 5; }
    virtual ~A2() {}
};

REGISTER( A2 )

class B2 : public A2
{
public:
    virtual int F() { return 10; }
    virtual ~B2() {}
};

REGISTER( B2 )

class C2 : public Device
{
public:
    C2() : x( "x", RegistrationToken() ) {}
    virtual int F() { return x -> F(); }
    virtual ~C2() {}
private:
    Plug< I2 > x;
};

REGISTER( C2 )

class D2 : public Device
{
public:
    D2() : x( "x", RegistrationToken() ) {}
    virtual int F()
    { 
        int sum = 0;
        for ( Container::iterator i = x.begin(); i != x.end(); ++i )
        {
            sum += i -> lock() -> F();
        }
        return sum; 
    }
    virtual ~D2() {}
private:
    typedef Plug< I2, collection > Container;
    Container x;
};

REGISTER( D2 )

class E2 : public Device
{
public:
    E2() : 
      x1( "x1", RegistrationToken() ),
      x2( "x2", RegistrationToken() ),
      x3( "x3", RegistrationToken() )
    {}
    virtual int F()
    { 
        int sum = 0;
        for ( Container1::iterator i = x1.begin(); i != x1.end(); ++i )
        {
            sum += i -> lock() -> F();
        }
        for ( Container2::iterator i = x2.begin(); i != x2.end(); ++i )
        {
            sum += i -> lock() -> F();
        }
        for ( Container3::iterator i = x3.begin(); i != x3.end(); ++i )
        {
            sum += i -> lock() -> F();
        }
        return sum; 
    }
    virtual ~E2() {}
private:
    typedef Plug< I2, collection, std::list > Container1;
    typedef Plug< I2, collection, std::vector > Container2;
    typedef Plug< I2, collection, std::deque > Container3;
    Container1 x1;
    Container2 x2;
    Container3 x3;
};

REGISTER( E2 )

// tests

BOOST_AUTO_TEST_SUITE( Wiring )

BOOST_AUTO_TEST_CASE( wiringOk )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c1", "C2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c1" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c2", "C2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c2" ] );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( use( "b" ).as( "x" ).of( "c1" ) );
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "x" ).of( "c2" ) );
    }

    shared_ptr< C2 > c1 = catalog[ "c1" ];
    BOOST_CHECK( c1 -> F() == 10 );

    shared_ptr< C2 > c2 = catalog[ "c2" ];
    BOOST_CHECK( c2 -> F() == 5 );
}

BOOST_AUTO_TEST_CASE( wiringKo )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c1", "C2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c1" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c2", "C2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "c2" ] );

    // bad type
    BOOST_CHECK_THROW( use( catalog[ "c2" ] ).as( "x" ).of( catalog[ "c1" ] ), WrongType );

    // src does not exist in the catalog
    BOOST_CHECK_THROW( use( catalog[ "b" ] ).as( "x" ).of( catalog[ "does_not_exist" ] ), ElementNotFound );
    // dst does not exist in the catalog
    BOOST_CHECK_THROW( use( catalog[ "does_not_exist" ] ).as( "x" ).of( catalog[ "c2" ] ), ElementNotFound );
    // attribute does not exist in the src
    BOOST_CHECK_THROW( use( catalog[ "a" ] ).as( "does_not_exist" ).of( catalog[ "c2" ] ), ElementNotFound );

    wallaroo_within( catalog )
    {
        // src does not exist in the catalog
        BOOST_CHECK_THROW( use( "b" ).as( "x" ).of( "does_not_exist" ), ElementNotFound );
        // dst does not exist in the catalog
        BOOST_CHECK_THROW( use( "does_not_exist" ).as( "x" ).of( "c2" ), ElementNotFound );
        // attribute does not exist in the src
        BOOST_CHECK_THROW( use( "a" ).as( "does_not_exist" ).of( "c2" ), ElementNotFound );
    }

    // no catalog selected:
    BOOST_CHECK_THROW( use( "b" ).as( "x" ).of( "c1" ), CatalogNotSpecified );

}

BOOST_AUTO_TEST_CASE( multipleCatalogs )
{
    Catalog catalog1;
    Catalog catalog2;

    BOOST_REQUIRE_NO_THROW( catalog1.Create( "a1", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "a1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "b1", "B2" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "b1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "c1", "C2" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "c1" ] );

    BOOST_REQUIRE_NO_THROW( catalog2.Create( "a2", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog2[ "a2" ] );
    BOOST_REQUIRE_NO_THROW( catalog2.Create( "c2", "C2" ) );
    BOOST_REQUIRE_NO_THROW( catalog2[ "c2" ] );
    BOOST_REQUIRE_NO_THROW( catalog2.Create( "c22", "C2" ) );
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

    shared_ptr< C2 > c1 = catalog1[ "c1" ];
    BOOST_CHECK( c1 -> F() == 10 );

    shared_ptr< C2 > c2 = catalog2[ "c2" ];
    BOOST_CHECK( c2 -> F() == 5 );

    shared_ptr< C2 > c22 = catalog2[ "c22" ];
    BOOST_CHECK( c22 -> F() == 5 );
}

BOOST_AUTO_TEST_CASE( nestedCatalogs )
{
    Catalog catalog1;
    Catalog catalog2;

    BOOST_REQUIRE_NO_THROW( catalog1.Create( "a1", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "a1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "b1", "B2" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "b1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "c1", "C2" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "c1" ] );
    BOOST_REQUIRE_NO_THROW( catalog1.Create( "c11", "C2" ) );
    BOOST_REQUIRE_NO_THROW( catalog1[ "c11" ] );

    BOOST_REQUIRE_NO_THROW( catalog2.Create( "a2", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog2[ "a2" ] );
    BOOST_REQUIRE_NO_THROW( catalog2.Create( "c2", "C2" ) );
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

    shared_ptr< C2 > c1 = catalog1[ "c1" ];
    BOOST_CHECK( c1 -> F() == 10 );

    shared_ptr< C2 > c2 = catalog2[ "c2" ];
    BOOST_CHECK( c2 -> F() == 5 );

    shared_ptr< C2 > c11 = catalog1[ "c11" ];
    BOOST_CHECK( c11 -> F() == 5 );
}

BOOST_AUTO_TEST_CASE( listWiring )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a1", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a1" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "a2", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a2" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "d", "D2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "d" ] );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( use( "a1" ).as( "x" ).of( "d" ) );
        BOOST_REQUIRE_NO_THROW( use( "a2" ).as( "x" ).of( "d" ) );
        BOOST_REQUIRE_NO_THROW( use( "b" ).as( "x" ).of( "d" ) );
    }

    shared_ptr< D2 > d = catalog[ "d" ];
    BOOST_CHECK( d -> F() == 20 );
}

BOOST_AUTO_TEST_CASE( containersWiring )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a1", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a1" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "a2", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a2" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "e", "E2" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "e" ] );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( use( "a1" ).as( "x1" ).of( "e" ) );
        BOOST_REQUIRE_NO_THROW( use( "a2" ).as( "x1" ).of( "e" ) );
        BOOST_REQUIRE_NO_THROW( use( "b" ).as( "x1" ).of( "e" ) );

        BOOST_REQUIRE_NO_THROW( use( "a1" ).as( "x2" ).of( "e" ) );
        BOOST_REQUIRE_NO_THROW( use( "a2" ).as( "x2" ).of( "e" ) );
        BOOST_REQUIRE_NO_THROW( use( "b" ).as( "x2" ).of( "e" ) );

        BOOST_REQUIRE_NO_THROW( use( "a1" ).as( "x3" ).of( "e" ) );
        BOOST_REQUIRE_NO_THROW( use( "a2" ).as( "x3" ).of( "e" ) );
        BOOST_REQUIRE_NO_THROW( use( "b" ).as( "x3" ).of( "e" ) );
    }

    shared_ptr< E2 > e = catalog[ "e" ];
    BOOST_CHECK( e -> F() == 60 );

    // check at compile time the containers are the right type

    using cxx0x::is_base_of;
    using cxx0x::weak_ptr;
    using std::deque;
    using std::vector;
    using std::list;
    typedef weak_ptr< I2 > I2Ptr;

    BOOST_STATIC_ASSERT((is_base_of< deque< I2Ptr >, Plug< I2, collection, deque > >::value));
    BOOST_STATIC_ASSERT((is_base_of< list< I2Ptr >, Plug< I2, collection, list > >::value));
    BOOST_STATIC_ASSERT((is_base_of< vector< I2Ptr >, Plug< I2, collection, vector > >::value));
    BOOST_STATIC_ASSERT((is_base_of< list< I2Ptr >, Plug< I2, collection > >::value)); // the default is std::list
}
BOOST_AUTO_TEST_SUITE_END()
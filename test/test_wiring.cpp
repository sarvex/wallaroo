/*******************************************************************************
 * wallaroo - A library for configurable creation and wiring of C++ classes.
 * Copyright (C) 2012 Daniele Pallastrelli
 *
 * This file is part of wallaroo.
 * For more information, see http://wallaroo.googlecode.com/
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
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

class I2 : public Part
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

WALLAROO_REGISTER( A2 )

class B2 : public A2
{
public:
    virtual int F() { return 10; }
    virtual ~B2() {}
};

WALLAROO_REGISTER( B2 )

class C2 : public Part
{
public:
    C2() : x( "x", RegistrationToken() ) {}
    virtual int F() { return x -> F(); }
    virtual ~C2() {}
private:
    Collaborator< I2 > x;
};

WALLAROO_REGISTER( C2 )

class D2 : public Part
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
    typedef Collaborator< I2, collection > Container;
    Container x;
};

WALLAROO_REGISTER( D2 )

class E2 : public Part
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
    typedef Collaborator< I2, collection, std::list > Container1;
    typedef Collaborator< I2, collection, std::vector > Container2;
    typedef Collaborator< I2, collection, std::deque > Container3;
    Container1 x1;
    Container2 x2;
    Container3 x3;
};

WALLAROO_REGISTER( E2 )

class F2 : public Part
{
public:
    F2() : collaborator( "collaborator", RegistrationToken() ) {}
    Collaborator< I2 > collaborator;
};

WALLAROO_REGISTER( F2 )

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

    BOOST_STATIC_ASSERT((is_base_of< deque< I2Ptr >, Collaborator< I2, collection, deque > >::value));
    BOOST_STATIC_ASSERT((is_base_of< list< I2Ptr >, Collaborator< I2, collection, list > >::value));
    BOOST_STATIC_ASSERT((is_base_of< vector< I2Ptr >, Collaborator< I2, collection, vector > >::value));
    BOOST_STATIC_ASSERT((is_base_of< vector< I2Ptr >, Collaborator< I2, collection > >::value)); // the default is std::vector
}

BOOST_AUTO_TEST_CASE( checkCast )
{
    BOOST_STATIC_ASSERT((cxx0x::is_convertible< Collaborator< I2 >, shared_ptr< I2 > >::value));
    BOOST_STATIC_ASSERT((cxx0x::is_convertible< Collaborator< I2 >, const shared_ptr< I2 > >::value));

    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A2" ) );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B2" ) );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "f1", "F2" ) );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "f2", "F2" ) );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( use( "a" ).as( "collaborator" ).of( "f1" ) );
        BOOST_REQUIRE_NO_THROW( use( "b" ).as( "collaborator" ).of( "f2" ) );
    }

    shared_ptr< F2 > f1 = catalog[ "f1" ];
    shared_ptr< F2 > f2 = catalog[ "f2" ];

    BOOST_REQUIRE_NO_THROW( static_cast< shared_ptr< I2 > >( f1 -> collaborator ) );
    BOOST_REQUIRE_NO_THROW( static_cast< const shared_ptr< I2 > >( f1 -> collaborator ) );
    BOOST_REQUIRE_NO_THROW( static_cast< shared_ptr< I2 > >( f2 -> collaborator ) );
    BOOST_REQUIRE_NO_THROW( static_cast< const shared_ptr< I2 > >( f2 -> collaborator ) );
}

BOOST_AUTO_TEST_SUITE_END()
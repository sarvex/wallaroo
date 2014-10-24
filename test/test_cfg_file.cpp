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
#include "plugin_interface.h"

#include "wallaroo/registered.h"
#include "wallaroo/catalog.h"
#include "wallaroo/jsonconfiguration.h"
#include "wallaroo/xmlconfiguration.h"
#include "wallaroo/cxx0x.h"

using namespace wallaroo;
using namespace cxx0x;


// a base class to automate the definition of the test class

namespace
{

template < typename P1, typename P2 >
class Base : public Device
{
public:
    Base( const P1& _p1, const P2& _p2 ) : p1( _p1 ), p2( _p2 ) {}
    const P1 p1;
    const P2 p2;
};

};

#define DEFINE_2PARAM_CLASS( name, p1, p2 ) \
    class name : public Base< p1, p2 > \
    { \
    public: \
        name( const p1& _p1, const p2& _p2 ) : Base< p1, p2 >( _p1, _p2 ) {} \
    }; \
    WALLAROO_REGISTER( name, p1, p2 )

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

namespace Foo
{
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
                sum += i->lock()->F();
            }
            return sum;
        }
    private:
        Plug< I5, collection > container;
        const std::string s;
        const int ii;
    };
}

WALLAROO_REGISTER( Foo::D5, std::string, int )


DEFINE_2PARAM_CLASS( E5, unsigned int, double )
DEFINE_2PARAM_CLASS( F5, double, bool )
DEFINE_2PARAM_CLASS( G5, bool, bool )
DEFINE_2PARAM_CLASS( H5, double, double )
DEFINE_2PARAM_CLASS( L5, int, std::string )
DEFINE_2PARAM_CLASS( M5, long, char )
DEFINE_2PARAM_CLASS( N5, char, unsigned char )

template < typename T >
class O5 : public Device
{
public:
    O5() : x( 10 ) {}
    int F() { return x;  }
    virtual ~O5() {}
private:
    const int x;
};

WALLAROO_REGISTER( O5< double > )
WALLAROO_REGISTER( O5< int > )

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

    shared_ptr< Foo::D5 > d = catalog[ "d" ];
    BOOST_CHECK( d -> String() == "mystring" );
    BOOST_CHECK( d -> Int() == 34 );
    BOOST_CHECK( d -> F() == 15 );

    shared_ptr< E5 > e = catalog[ "e" ];
    BOOST_CHECK( e -> p1 == 5 );
    BOOST_CHECK( e -> p2 == 3.14 );

    shared_ptr< F5 > f = catalog[ "f" ];
    BOOST_CHECK( f -> p1 == 3.14 );
    BOOST_CHECK( f -> p2 == false );

    shared_ptr< G5 > g = catalog[ "g" ];
    BOOST_CHECK( g -> p1 == false );
    BOOST_CHECK( g -> p2 == true );

    shared_ptr< H5 > h = catalog[ "h" ];
    BOOST_CHECK( h -> p1 == 1.0 );
    BOOST_CHECK( h -> p2 == -2.0 );

    shared_ptr< L5 > l = catalog[ "l" ];
    BOOST_CHECK( l -> p1 == -100 );
    BOOST_CHECK( l -> p2 == "foo bar" );

    shared_ptr< M5 > m = catalog[ "m" ];
    BOOST_CHECK( m -> p1 == -2000000000L );
    BOOST_CHECK( m -> p2 == 'a' );

    shared_ptr< N5 > n = catalog[ "n" ];
    BOOST_CHECK( n -> p1 == 'b' );
    BOOST_CHECK( n -> p2 == 200 );

    shared_ptr< I6 > p = catalog[ "p" ];
    BOOST_CHECK( p -> F() == 7 );

    shared_ptr< I6 > q = catalog[ "q" ];
    BOOST_CHECK( q -> F() == 8 );

    shared_ptr< O5< double > > o_double = catalog[ "o_double" ];
    BOOST_CHECK( o_double -> F() == 10 );

    shared_ptr< O5< int > > o_int = catalog[ "o_int" ];
    BOOST_CHECK( o_int->F() == 10 );

}

BOOST_AUTO_TEST_CASE( JsonOk )
{
    JsonConfiguration file( "test_json.json" );
    BOOST_REQUIRE_NO_THROW( file.LoadPlugins() );
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( file.Fill( catalog ) );
    TestContent( catalog );
}

BOOST_AUTO_TEST_CASE( XmlOk )
{
    XmlConfiguration file( "test_xml.xml" );
    BOOST_REQUIRE_NO_THROW( file.LoadPlugins() );
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( file.Fill( catalog ) );
    TestContent( catalog );
}

BOOST_AUTO_TEST_SUITE_END()
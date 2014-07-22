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
#include "wallaroo/jsonconfiguration.h"
#include "wallaroo/xmlconfiguration.h"

using namespace wallaroo;
using namespace cxx0x;

// class with attributes:

struct A7 : public Device
{
    A7() :
        strAtt( "str_attr", RegistrationToken() ),
        strAtt2( "str_attr_2", RegistrationToken() ),
        cintAtt( "c_int_attr", RegistrationToken() ),
        intAtt( "int_attr", RegistrationToken( ) ),
        ulAtt( "ul_attr", RegistrationToken() ),
        boolAtt( "bool_attr", RegistrationToken() )
    {}
    virtual ~A7() {}

    Attribute< std::string > strAtt;
    Attribute< std::string > strAtt2;
    const Attribute< int > cintAtt;
    Attribute< int > intAtt;
    Attribute< unsigned long > ulAtt;
    Attribute< bool > boolAtt;
};

WALLAROO_REGISTER( A7 )


struct B7 : public Device
{
    B7() :
        schAtt( "schar_attr", RegistrationToken() ),
        uchAtt( "uchar_attr", RegistrationToken() ),
        ssiAtt( "ssi_attr", RegistrationToken() ),
        usiAtt( "usi_attr", RegistrationToken() ),
        siAtt( "si_attr", RegistrationToken() ),
        uiAtt( "ui_attr", RegistrationToken() ),
        sliAtt( "sli_attr", RegistrationToken() ),
        uliAtt( "uli_attr", RegistrationToken() ),
        fAtt( "f_attr", RegistrationToken() ),
        dAtt( "d_attr", RegistrationToken() ),
        ldAtt( "ld_attr", RegistrationToken() ),
        strAtt( "str_attr", RegistrationToken() ),
        boolAtt( "bool_attr", RegistrationToken() )
    {}
    virtual ~B7() {}

    Attribute< signed char > schAtt;
    Attribute< unsigned char > uchAtt;
    Attribute< signed short int > ssiAtt;
    Attribute< unsigned short int > usiAtt;
    Attribute< signed int > siAtt;
    Attribute< unsigned int > uiAtt;
    Attribute< signed long int > sliAtt;
    Attribute< unsigned long int > uliAtt;
    Attribute< float > fAtt;
    Attribute< double > dAtt;
    Attribute< long double > ldAtt;
    Attribute< std::string > strAtt;
    Attribute< bool > boolAtt;
};

WALLAROO_REGISTER( B7 )

// class to test the Device::Init method
struct C7 : public Device
{
    C7() : ready( false ) {}
    virtual ~C7() {}
    virtual void Init() { ready = true;  }
    bool ready;
};

WALLAROO_REGISTER( C7 )

// tests

BOOST_AUTO_TEST_SUITE( Attributes )

BOOST_AUTO_TEST_CASE( values )
{
    Catalog catalog;
    shared_ptr< A7 > a;

    BOOST_REQUIRE_NO_THROW( a = catalog.Create( "a", "A7" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );

    BOOST_REQUIRE_NO_THROW( set( "str_attr" ).of( catalog[ "a" ] ).to( std::string( "mystring" ) ) );
    BOOST_REQUIRE_NO_THROW( set( "str_attr_2" ).of( catalog[ "a" ] ).to( "mystring2" ) );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( set( "int_attr" ).of( "a" ).to( -123 ) );
        BOOST_REQUIRE_NO_THROW( set( "ul_attr" ).of( "a" ).to( 123456 ) );
        BOOST_REQUIRE_NO_THROW( set( "bool_attr" ).of( "a" ).to( false ) );
    }

    // check values

    BOOST_CHECK( a -> strAtt == std::string( "mystring" ) );
    BOOST_CHECK( a -> strAtt2 == std::string( "mystring2" ) );

    BOOST_CHECK( std::string( "mystring" ) == a -> strAtt );
    BOOST_CHECK( std::string( "mystring2" ) == a -> strAtt2 );

    const std::string s1 = a -> strAtt;
    BOOST_CHECK( s1 == "mystring" );
    const std::string s2 = a -> strAtt2;
    BOOST_CHECK( s2 == std::string( "mystring2" ) );

    BOOST_CHECK( a -> intAtt == -123 );
    BOOST_CHECK( a -> ulAtt == 123456 );
    BOOST_CHECK( a -> boolAtt == false );
}

BOOST_AUTO_TEST_CASE( attributeRelationalOperators )
{
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B7" ) );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A7" ) );
    BOOST_REQUIRE_NO_THROW( set( "ui_attr" ).of( catalog[ "b" ] ).to( 50 ) );
    BOOST_REQUIRE_NO_THROW( set( "ul_attr" ).of( catalog[ "a" ] ).to( 30 ) );

    shared_ptr< B7 > b = catalog[ "b" ];
    shared_ptr< A7 > a = catalog[ "a" ];

    // Attribute VS size_t

    std::size_t x = 30;
    // x < uiAtt
    BOOST_CHECK( x < b->uiAtt );
    BOOST_CHECK( b->uiAtt > x );
    BOOST_CHECK( x <= b->uiAtt );
    BOOST_CHECK( b->uiAtt >= x );
    BOOST_CHECK( x != b->uiAtt );
    BOOST_CHECK( b->uiAtt != x );

    x = 60;
    // x > uiAtt
    BOOST_CHECK( x > b->uiAtt );
    BOOST_CHECK( b->uiAtt < x );
    BOOST_CHECK( x >= b->uiAtt );
    BOOST_CHECK( b->uiAtt <= x );
    BOOST_CHECK( x != b->uiAtt );
    BOOST_CHECK( b->uiAtt != x );

    x = 50;
    // x == uiAtt
    BOOST_CHECK( x == b->uiAtt );
    BOOST_CHECK( b->uiAtt == x );

    // Attribute VS Attribute

    // a->a->ulAtt < b->uiAtt
    BOOST_CHECK( a->ulAtt < b->uiAtt );
    BOOST_CHECK( b->uiAtt > a->ulAtt );
    BOOST_CHECK( a->ulAtt <= b->uiAtt );
    BOOST_CHECK( b->uiAtt >= a->ulAtt );
    BOOST_CHECK( a->ulAtt != b->uiAtt );
    BOOST_CHECK( b->uiAtt != a->ulAtt );

    // a->ulAtt > b->uiAtt
    a->ulAtt = 60;
    BOOST_CHECK( a->ulAtt > b->uiAtt );
    BOOST_CHECK( b->uiAtt < a->ulAtt );
    BOOST_CHECK( a->ulAtt >= b->uiAtt );
    BOOST_CHECK( b->uiAtt <= a->ulAtt );
    BOOST_CHECK( a->ulAtt != b->uiAtt );
    BOOST_CHECK( b->uiAtt != a->ulAtt );

    a->ulAtt = 50;
    // a->ulAtt == uiAtt
    BOOST_CHECK( a->ulAtt == b->uiAtt );
    BOOST_CHECK( b->uiAtt == a->ulAtt );
}

BOOST_AUTO_TEST_CASE( attributeArithmeticOperators )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A7" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );

    BOOST_REQUIRE_NO_THROW( set( "str_attr" ).of( catalog[ "a" ] ).to( std::string( "mystring" ) ) );
    BOOST_REQUIRE_NO_THROW( set( "str_attr_2" ).of( catalog[ "a" ] ).to( "mystring2" ) );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( set( "c_int_attr" ).of( "a" ).to( -123 ) );
        BOOST_REQUIRE_NO_THROW( set( "ul_attr" ).of( "a" ).to( 123456 ) );
        BOOST_REQUIRE_NO_THROW( set( "bool_attr" ).of( "a" ).to( false ) );
    }

    shared_ptr< A7 > a = catalog[ "a" ];

    // check assignment
    BOOST_CHECK_NO_THROW( a -> intAtt = 3 );
    BOOST_CHECK( a -> intAtt == 3 );
    BOOST_CHECK_NO_THROW( a -> strAtt = "foo" );
    BOOST_CHECK( a -> strAtt == std::string( "foo" ) );
    BOOST_CHECK_NO_THROW( a -> boolAtt = true );
    BOOST_CHECK( a -> boolAtt );

    // arithmetic operators
    BOOST_CHECK( ++ a -> intAtt == 4 ); // prefix inc
    BOOST_CHECK( a -> intAtt ++ == 4 ); // postfix inc
    BOOST_CHECK( a -> intAtt == 5 );

    BOOST_CHECK( -- a -> intAtt == 4 ); // prefix dec
    BOOST_CHECK( a -> intAtt -- == 4 ); // postfix dec
    BOOST_CHECK( a -> intAtt == 3 );

    BOOST_CHECK( ( a->intAtt += 2 ) == 5 );
    BOOST_CHECK( a->intAtt == 5 );
    BOOST_CHECK( ( a->intAtt -= 2 ) == 3 );
    BOOST_CHECK( a->intAtt == 3 );
    BOOST_CHECK( ( a->intAtt /= 3 ) == 1 );
    BOOST_CHECK( a->intAtt == 1 );
    BOOST_CHECK( ( a->intAtt *= 3 ) == 3 );
    BOOST_CHECK( a->intAtt == 3 );

    BOOST_CHECK( a->intAtt + 1 == 4 );
    BOOST_CHECK( a->intAtt - 1 == 2 );
    BOOST_CHECK( a->intAtt * 2 == 6 );
    BOOST_CHECK( a->intAtt / 3 == 1 );

    BOOST_CHECK( 1 + a->intAtt == 4 );
    BOOST_CHECK( 3 - a->intAtt == 0 );
    BOOST_CHECK( 2 * a->intAtt == 6 );
    BOOST_CHECK( 3 / a->intAtt == 1 );

    BOOST_CHECK( a->intAtt + a->intAtt == 6 );
    BOOST_CHECK( a->intAtt - a->intAtt == 0 );
    BOOST_CHECK( a->intAtt * a->intAtt == 9 );
    BOOST_CHECK( a->intAtt / a->intAtt == 1 );

    // check some operators with const attribute
    const int x = 123;
    BOOST_CHECK( x + a->cintAtt == 0 );
    BOOST_CHECK( a->ulAtt - x == 123333 );
}

BOOST_AUTO_TEST_CASE( attributeStringOperators )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A7" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );

    BOOST_REQUIRE_NO_THROW( set( "str_attr" ).of( catalog[ "a" ] ).to( std::string( "foo" ) ) );
    BOOST_REQUIRE_NO_THROW( set( "str_attr_2" ).of( catalog[ "a" ] ).to( "bar" ) );

    shared_ptr< A7 > a = catalog[ "a" ];

    // check values
    BOOST_CHECK( a -> strAtt == std::string( "foo" ) );
    BOOST_CHECK( a -> strAtt2 == std::string( "bar" ) );

    // operators
    BOOST_CHECK( ( a -> strAtt += std::string( "42" ) ) == std::string( "foo42" ) );
    BOOST_CHECK( a -> strAtt == std::string( "foo42" ) );

    BOOST_CHECK( ( a -> strAtt2 + std::string( "42" ) ) == std::string( "bar42" ) );
    BOOST_CHECK( a -> strAtt2 == std::string( "bar" ) );

    BOOST_CHECK( std::string( "42" ) + a -> strAtt2 == std::string( "42bar" ) );
    BOOST_CHECK( a -> strAtt2 == std::string( "bar" ) );

    BOOST_CHECK( a -> strAtt + a -> strAtt2 == std::string( "foo42bar" ) );
}

BOOST_AUTO_TEST_CASE( attributesKo )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A7" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );

    // bad type
    BOOST_CHECK_THROW( set( "int_attr" ).of( catalog[ "a" ] ).to( "foo" ), WrongType );
    // BOOST_CHECK_THROW( set( "ul_attr" ).of( catalog[ "a" ] ).to( -100 ), WrongType ); // ### TODO
    BOOST_CHECK_THROW( set( "bool_attr" ).of( catalog[ "a" ] ).to( 100 ), WrongType );

    // of does not exist in the catalog
    BOOST_CHECK_THROW( set( "int_attr" ).of( catalog[ "does_not_exist" ] ).to( "foo" ), ElementNotFound );
    // attribute does not exist in the device
    BOOST_CHECK_THROW( set( "does_not_exist" ).of( catalog[ "a" ] ).to( "foo" ), ElementNotFound );
    BOOST_CHECK_THROW( set( "does_not_exist" ).of( catalog[ "a" ] ).to( std::string( "foo" ) ), ElementNotFound );
    BOOST_CHECK_THROW( set( "does_not_exist" ).of( catalog[ "a" ] ).to( true ), ElementNotFound );

    wallaroo_within( catalog )
    {
        // of does not exist in the catalog
        BOOST_CHECK_THROW( set( "int_attr" ).of( "does_not_exist" ).to( "foo" ), ElementNotFound );
        // attribute does not exist in the device
        BOOST_CHECK_THROW( set( "does_not_exist" ).of( "a" ).to( "foo" ), ElementNotFound );
    }

    // no catalog selected:
    BOOST_CHECK_THROW( set( "int_attr" ).of( "a" ).to( -123 ), CatalogNotSpecified );
}

BOOST_AUTO_TEST_CASE( stringAttributes )
{
    Catalog catalog;

    BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A7" ) );
    BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );

    wallaroo_within( catalog )
    {
        BOOST_REQUIRE_NO_THROW( set( "str_attr" ).of( "a" ).to( std::string( "mystring" ) ) );
        BOOST_REQUIRE_NO_THROW( set( "str_attr_2" ).of( "a" ).to( "mystring2" ) );
        BOOST_REQUIRE_NO_THROW( set( "int_attr" ).of( "a" ).to( "-123" ) );
        BOOST_REQUIRE_NO_THROW( set( "ul_attr" ).of( "a" ).to( "123456" ) );
        BOOST_REQUIRE_NO_THROW( set( "bool_attr" ).of( "a" ).to( "false" ) );
    }

    shared_ptr< A7 > a = catalog[ "a" ];
    BOOST_CHECK( a -> strAtt == std::string( "mystring" ) );
    BOOST_CHECK( a -> strAtt2 == std::string( "mystring2" ) );
    BOOST_CHECK( a -> intAtt == -123 );
    BOOST_CHECK( a -> ulAtt == 123456 );
    BOOST_CHECK( a -> boolAtt == false );
}

static void TestContent( Catalog& catalog )
{
    shared_ptr< A7 > a1 = catalog[ "a1" ];
    BOOST_CHECK( a1 -> intAtt == -123 );

    shared_ptr< A7 > a2 = catalog[ "a2" ];
    BOOST_CHECK( a2 -> boolAtt == false );
    BOOST_CHECK( a2 -> ulAtt == 123456 );

    shared_ptr< B7 > b1 = catalog[ "b1" ];
    BOOST_CHECK( b1->schAtt == 'z' );
    BOOST_CHECK( b1->uchAtt == 'x' );
    BOOST_CHECK( b1->ssiAtt == -6000 );
    BOOST_CHECK( b1->usiAtt == 6000 );
    BOOST_CHECK( b1->siAtt == -333333 );
    BOOST_CHECK( b1->uiAtt == 333333 );
    BOOST_CHECK( b1->sliAtt == -999999999 );
    BOOST_CHECK( b1->uliAtt == 999999999 );
    BOOST_CHECK( b1->fAtt == -1.0 );
    BOOST_CHECK( b1->dAtt == 2.0 );
    BOOST_CHECK( b1->ldAtt == 4.5 );
    BOOST_CHECK( b1->strAtt == std::string( "foo bar" ) );
    BOOST_CHECK( b1 -> boolAtt == false );
}

BOOST_AUTO_TEST_CASE( JsonAttributes )
{
    JsonConfiguration file( "test_attributes.json" );
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( file.Fill( catalog ) );
    TestContent( catalog );
}

BOOST_AUTO_TEST_CASE( XmlAttributes )
{
    XmlConfiguration file( "test_attributes.xml" );
    Catalog catalog;
    BOOST_REQUIRE_NO_THROW( file.Fill( catalog ) );
    TestContent( catalog );
}

BOOST_AUTO_TEST_CASE( initMethod )
{
    Catalog catalog;

    // create some objects
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c1", "C7" ) );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c2", "C7" ) );
    BOOST_REQUIRE_NO_THROW( catalog.Create( "c3", "C7" ) );

    // this should call Init for every object in the catalog
    catalog.Init();

    // check every object has been initialized
    shared_ptr< C7 > c1 = catalog[ "c1" ];
    shared_ptr< C7 > c2 = catalog[ "c2" ];
    shared_ptr< C7 > c3 = catalog[ "c3" ];
    BOOST_CHECK( c1 -> ready );
    BOOST_CHECK( c2 -> ready );
    BOOST_CHECK( c3 -> ready );
}

BOOST_AUTO_TEST_SUITE_END()

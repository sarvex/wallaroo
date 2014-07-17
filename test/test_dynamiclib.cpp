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

#include "wallaroo/catalog.h"
#include "wallaroo/dynamic_loader.h"

#include "plugin_interface.h"

using namespace wallaroo;

BOOST_AUTO_TEST_SUITE( DynamicLibs )

BOOST_AUTO_TEST_CASE( DynamicLoadingFailure )
{
    BOOST_CHECK_THROW( { Plugin::Load( "unexistent.dll" ); }, WrongFile );
}

BOOST_AUTO_TEST_CASE( DynamicLoading )
{
    try
    {
        Plugin::Load( "plugin" + Plugin::Suffix() );
        Catalog catalog;

        BOOST_REQUIRE_NO_THROW( catalog.Create( "a", "A6" ) );
        BOOST_REQUIRE_NO_THROW( catalog[ "a" ] );

        cxx0x::shared_ptr< I6 > obj = catalog[ "a" ];
        BOOST_CHECK( obj -> F() == 3 );

        BOOST_REQUIRE_NO_THROW( catalog.Create( "b", "B6" ) );
        BOOST_REQUIRE_NO_THROW( catalog[ "b" ] );

        obj = catalog[ "b" ];
        BOOST_CHECK( obj -> F() == 4 );
    }
    catch ( ... )
    {
        BOOST_ERROR( "exception thrown during shared library loading" );
    }
}

BOOST_AUTO_TEST_CASE( DynamicLoadingAttributes )
{
    try
    {
        Plugin::Load( "plugin" + Plugin::Suffix( ) );
        Catalog catalog;

        BOOST_REQUIRE_NO_THROW( catalog.Create( "c", "C6" ) );
        BOOST_REQUIRE_NO_THROW( catalog[ "c" ] );

        BOOST_REQUIRE_NO_THROW( set( "att1" ).of( catalog[ "c" ] ).to( -100 ) );
        BOOST_REQUIRE_NO_THROW( set( "att2" ).of( catalog[ "c" ] ).to( 58 ) );

        cxx0x::shared_ptr< I6 > obj = catalog[ "c" ];

        BOOST_CHECK( obj -> F() == -42 );
    }
    catch ( ... )
    {
        BOOST_ERROR( "exception thrown during shared library loading" );
    }
}


BOOST_AUTO_TEST_SUITE_END()
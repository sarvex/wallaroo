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

BOOST_AUTO_TEST_SUITE_END()
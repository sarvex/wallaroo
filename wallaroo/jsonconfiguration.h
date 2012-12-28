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

#ifndef WALLAROO_JSONCONFIGURATION_H_
#define WALLAROO_JSONCONFIGURATION_H_

#include "catalog.h"
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using namespace boost::property_tree;

namespace wallaroo
{

/** This class can parse an json file containing a list of objects to
* be created and their wiring.
* Then it can populate a @c Catalog with that objects.
*/
class JsonConfiguration
{
public:
    /** Create an JsonConfiguration from the path specified as parameter.
    * @param fileName the path of the file to parse
    * @throw WrongFile if the file does not exist or its format is wrong.
    */
    explicit JsonConfiguration( const std::string& fileName )
    {
        try
        {
            read_json( fileName, tree );
        }
        catch ( const json_parser_error& e )
        {
            throw WrongFile( e.what() );
        }
    }

    /** Fill the @c catalog with the objects and relations specified in the file.
    * @param catalog The catalog target of the new items of the file.
    * @throw WrongFile if the file contains a semantic error.
    */
    void Fill( Catalog& catalog )
    {
        try
        {
            BOOST_FOREACH( ptree::value_type &v, tree.get_child( "catalog.devices" ) )
            {
                ParseObject( catalog, v.second );
            }
            BOOST_FOREACH( ptree::value_type &v, tree.get_child( "catalog.wiring" ) )
            {
                ParseRelation( catalog, v.second );
            }
        }
        catch ( const ptree_error& e )
        {
            throw WrongFile( e.what() );
        }
    }

private:

    /**********************/

    template < typename T >
    struct TypeDesc;

    template <>
    struct TypeDesc< int >
    {
        static const char* Name() { return "int"; }
    };

    template <>
    struct TypeDesc< std::string >
    {
        static const char* Name() { return "string"; }
    };

    template <>
    struct TypeDesc< unsigned int >
    {
        static const char* Name() { return "unsigned int"; }
    };

    template <>
    struct TypeDesc< double >
    {
        static const char* Name() { return "double"; }
    };

    template < typename T1, typename T2 >
    static bool Create( 
            Catalog& catalog,
            const std::string& instance,
            const std::string& cl,
            boost::optional< const ptree& > tree1,
            boost::optional< const ptree& > tree2
        )
    {
        const std::string& type1 = tree1 -> get< std::string >( "type" );
        const std::string& type2 = tree2 -> get< std::string >( "type" );
        if ( type1 == TypeDesc< T1 >::Name() && type2 == TypeDesc< T2 >::Name() )
        {
            const T1& p1 = tree1 -> get< T1 >( "value" );
            const T2& p2 = tree2 -> get< T2 >( "value" );
            catalog.Create( instance, cl, p1, p2 );
            return true;
        }
        return false;
    }

    template < typename T >
    static bool Create(
            Catalog& catalog,
            const std::string& instance,
            const std::string& cl,
            boost::optional< const ptree& > tree
        )
    {
        const std::string& type = tree -> get< std::string >( "type" );
        if ( type == TypeDesc< T >::Name() )
        {
            const T& p = tree -> get< T >( "value" );
            catalog.Create( instance, cl, p );
            return true;
        }
        return false;
    }

    /**********************/

    void ParseObject( Catalog& catalog, const ptree& v )
    {
        const std::string& name = v.get< std::string >( "name" );
        const std::string& cl = v.get< std::string >( "class" );

        boost::optional< const ptree& > par1 = v.get_child_optional( "parameter1" );
        boost::optional< const ptree& > par2 = v.get_child_optional( "parameter2" );

        if ( par1 && par2 )
        {
            
            if ( Create< std::string, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, double >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< int, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, double >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< unsigned int, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, double >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< double, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, double >( catalog, name, cl, par1, par2 ) ) return;
        }
        else if ( par1 )
        {
            if ( Create< std::string >( catalog, name, cl, par1 ) ) return;
            if ( Create< int >( catalog, name, cl, par1 ) ) return;
            if ( Create< unsigned int >( catalog, name, cl, par1 ) ) return;
            if ( Create< double >( catalog, name, cl, par1 ) ) return;
        }
        else
        {
            catalog.Create( name, cl );
        }
    }

    void ParseRelation( Catalog& catalog, const ptree& v )
    {
        const std::string& source = v.get< std::string >( "source" );
        const std::string& dest = v.get< std::string >( "dest" );
        const std::string& role = v.get< std::string >( "plug" );

        use( catalog[ dest ] ).as( role ).of( catalog[ source ] );
    }

    ptree tree;
};

} // namespace

#endif

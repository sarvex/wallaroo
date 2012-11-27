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

#ifndef WALLAROO_XMLWIRINGFILE_H_
#define WALLAROO_XMLWIRINGFILE_H_

#include "catalog.h"
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

using namespace boost::property_tree;

namespace wallaroo
{

/** This class can parse an xml file containing a list of objects to
* be created and their wiring.
* Then it can populate a @c Catalog with that objects.
*/
class XmlWiringFile
{
public:
    /** Create an XmlWiringFile from the path specified as parameter.
    * @param fileName the path of the file to parse
    * @throw WrongFile if the file does not exist or its format is wrong.
    */
    explicit XmlWiringFile( const std::string& fileName )
    {
        try
        {
            read_xml( fileName, tree, xml_parser::no_comments | xml_parser::trim_whitespace );
        }
        catch ( const xml_parser_error& e )
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
            BOOST_FOREACH( ptree::value_type &v, tree.get_child( "catalog" ) )
            {
                if ( v.first == "object" ) ParseObject( catalog, v.second );
                else if ( v.first == "relation" ) ParseRelation( catalog, v.second );
            }
        }
        catch ( const ptree_error& e )
        {
            throw WrongFile( e.what() );
        }
    }

private:

    void ParseObject( Catalog& catalog, ptree& v )
    {
        const std::string name = v.get< std::string >( "name" );
        const std::string cl = v.get< std::string >( "class" );

        boost::optional< std::string > par1type = v.get_optional< std::string >( "par1.<xmlattr>.type" );
        boost::optional< std::string > par2type = v.get_optional< std::string >( "par2.<xmlattr>.type" );

        if ( par1type && par2type )
        {
            if ( *par1type == "string" )
            {
                const std::string par1 = v.get< std::string >( "par1" );
                if ( *par2type == "string" )
                {
                    const std::string par2 = v.get< std::string >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }
                else if ( *par2type == "int" )
                {
                    const int par2 = v.get< int >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }
                else if ( *par2type == "unsgined int" )
                {
                    const unsigned int par2 = v.get< unsigned int >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }   
            }
            else if ( *par1type == "int" )
            {
                const int par1 = v.get< int >( "par1" );
                if ( *par2type == "string" )
                {
                    const std::string par2 = v.get< std::string >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }
                else if ( *par2type == "int" )
                {
                    const int par2 = v.get< int >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }
                else if ( *par2type == "unsgined int" )
                {
                    const unsigned int par2 = v.get< unsigned int >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }   
            }
            else if ( *par1type == "unsigned int" )
            {
                const unsigned int par1 = v.get< unsigned int >( "par1" );
                if ( *par2type == "string" )
                {
                    const std::string par2 = v.get< std::string >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }
                else if ( *par2type == "int" )
                {
                    const int par2 = v.get< int >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }
                else if ( *par2type == "unsgined int" )
                {
                    const unsigned int par2 = v.get< unsigned int >( "par2" );
                    catalog.Create( name, cl, par1, par2 );
                }   
            }
        }
        else if ( par1type )
        {
            if ( *par1type == "string" )
            {
                const std::string par = v.get< std::string >( "par1" );
                catalog.Create( name, cl, par );
            }
            else if ( *par1type == "int" )
            {
                const int par = v.get< int >( "par1" );
                catalog.Create( name, cl, par );
            }
            else if ( *par1type == "unsigned int" )
            {
                const unsigned int par = v.get< unsigned int >( "par1" );
                catalog.Create( name, cl, par );
            }
        }
        else
        {
            catalog.Create( name, cl );
        }
    }

    void ParseRelation( Catalog& catalog, ptree& v )
    {
        const std::string source = v.get< std::string >( "source" );
        const std::string dest = v.get< std::string >( "dest" );
        const std::string role = v.get< std::string >( "role" );

        use( catalog[ dest ] ).as( role ).of( catalog[ source ] );
    }

    ptree tree;
};

} // namespace

#endif

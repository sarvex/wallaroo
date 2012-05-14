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

class XmlWiringFile
{
public:
    explicit XmlWiringFile( const std::string& fileName )
    {
        try
        {
            read_xml( fileName, tree, xml_parser::no_comments | xml_parser::trim_whitespace );
        }
        catch ( const xml_parser_error& e )
        {
            throw std::range_error( e.what() );
        }
    }
    void Fill( Catalog& catalog )
    {
        try
        {
            BOOST_FOREACH( ptree::value_type &v, tree.get_child( "catalog" ) )
            {
                if ( v.first == "object" )
                {
                    const std::string name = v.second.get< std::string >( "name" );
                    const std::string cl = v.second.get< std::string >( "class" );
                    catalog.Create( name, cl );
                }
                else if ( v.first == "relation" )
                {
                    const std::string source = v.second.get< std::string >( "source" );
                    const std::string dest = v.second.get< std::string >( "dest" );
                    const std::string role = v.second.get< std::string >( "role" );
                    catalog[ source ].Wire( role, catalog[ dest ] );
                }
            }
        }
        catch ( const ptree_error& e )
        {
            throw std::range_error( e.what() );
        }
    }
private:
    ptree tree;
};

} // namespace

#endif
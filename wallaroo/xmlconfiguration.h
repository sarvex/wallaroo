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

#ifndef WALLAROO_XMLCONFIGURATION_H_
#define WALLAROO_XMLCONFIGURATION_H_

#include "catalog.h"
#include "detail/ptreebasedcfg.h"
#include <boost/property_tree/xml_parser.hpp>

using namespace boost::property_tree;

namespace wallaroo
{

/**
* This class can parse a xml file containing a list of objects to
* be created and their wiring.
* Then it can populate a @c Catalog with that objects.
* The syntax of the xml file should be similar to:
\code
<wallaroo>
  <devices>
    <device>
      <name>instance1</name>
      <class>className1</class>
    </device>
    <device>
      <name>instance2</name>
      <class>className2</class>
      <parameter1>
        <type>string</type>
        <value>mystring</value>
      </parameter1>
      <parameter2>
        <type>int</type>
        <value>34</value>
      </parameter2>
    </device>
  </devices>
  <wiring>    
    <wire>
      <source>sourceInstance</source>
      <dest>targetInstance</dest>
      <plug>plugName</plug>
    </wire>
    <wire>
      <source>sourceInstance2</source>
      <dest>targetInstance2</dest>
      <plug>plugName2</plug>
    </wire>
  </wiring>
</wallaroo>
\endcode
*/
class XmlConfiguration : private detail::PtreeBasedCfg
{
public:
    /** Create a XmlConfiguration from the path specified as parameter.
    * @param fileName the path of the file to parse
    * @throw WrongFile if the file does not exist or its format is wrong.
    */
    explicit XmlConfiguration( const std::string& fileName ) :
        detail::PtreeBasedCfg( tree )
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
        detail::PtreeBasedCfg::Fill( catalog );
    }

private:

    ptree tree;
};

} // namespace

#endif

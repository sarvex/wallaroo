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
#include "ptreebasedcfg.h"
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

namespace wallaroo
{

/**
* This class can parse a json file containing a list of objects to
* be created and their wiring.
* Then it can populate a @c Catalog with that objects.
* The syntax of the json file should be similar to:
\code
{

  "wallaroo":
  {

    "devices":
    [
      {
        "name": "instance1",
        "class": "className1"
      },

      {
        "name": "instance2",
        "class": "className2",
        "parameter1":
          {
            "type": "string",
            "value": "mystring"
          },
        "parameter2":
          {
            "type": "int",
            "value": 34
          }
      }
    ],
    
    "wiring":
    [
      {
        "source": "sourceInstance",
        "dest": "targetInstance",
        "plug": "plugName"
      },

      {
        "source": "sourceInstance2",
        "dest": "targetInstance2",
        "plug": "plugName2"
      }
    ]

  }

}
\endcode
*/
class JsonConfiguration : private detail::PtreeBasedCfg
{
public:
    /** Create a JsonConfiguration from the path specified as parameter.
    * @param fileName the path of the file to parse
    * @throw WrongFile if the file does not exist or its format is wrong.
    */
    explicit JsonConfiguration( const std::string& fileName ) :
        detail::PtreeBasedCfg( tree )
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
        detail::PtreeBasedCfg::Fill( catalog );
    }

private:

    ptree tree;
};

} // namespace

#endif

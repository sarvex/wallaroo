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

#ifndef WALLAROO_DYNAMIC_LOADER_H_
#define WALLAROO_DYNAMIC_LOADER_H_

#include <string>
#include <vector>
#include "cxx0x.h"
#include "device.h"
#include "class.h"
#include "exceptions.h"
#include "detail/dyn_class_descriptor.h"
#include "detail/dynamic_library.h"

namespace wallaroo
{

/**
 * Represent a shared library containing wallaroo class definitions.
 *
 * Given the file name, Plugin loads a shared library 
 * and stores the descritptors of registered classes. Then, the classes
 * will be available for the dynamic creation via the Catalog::Create method.
 *
 * You can get a Plugin istance by using the Plugin::Load method. The instance lifetime
 * is managed by wallaroo: the object is destroyed (and the library is unloaded)
 * when there are no more reference, instances of registered classes, and descriptor.
 */
class Plugin
{
public:
    /** Load the shared library specified by the file name @c fileName thus
     * giving access via the Catalog::Create method to all the classes registered
     * inside with the macro WALLAROO_DYNLIB_REGISTER.
     * @param fileName The path of the shared library to load.
     * @throw WrongFile if the file does not exist or its format is wrong.
     */
    static cxx0x::shared_ptr< Plugin > Load( const std::string& fileName )
    {
        using namespace detail;
        cxx0x::shared_ptr< Plugin > p( new Plugin( fileName ) );
        typedef std::vector< Descriptor >* (*Function)(void);
        Function GetClasses = p -> library.GetFunction< Function >( "GetClasses" );
        if ( GetClasses == NULL ) throw WrongFile( fileName );
        std::vector< Descriptor >* descriptors = GetClasses();
        for ( size_t i = 0; i < descriptors -> size(); ++i )
            Class< void, void >::Register( (*descriptors)[ i ].name, (*descriptors)[ i ].create, p );
        return p;
    }
    /** Returns the platform-specific filename suffix
     * for shared libraries (including the period).
     * In debug mode, the suffix also includes a
     * "d" to specify the debug version of a library.
     */
    static std::string Suffix()
    {
        return detail::DynamicLibrary::Suffix();
    }
private:
    // throw WrongFile if the file does not exist or its format is wrong.
    Plugin( const std::string& fileName ) :
      library( fileName )
    {
    }
    detail::DynamicLibrary library;
};

} // namespace wallaroo

#endif // WALLAROO_DYNAMIC_LOADER_H_

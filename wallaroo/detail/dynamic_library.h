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

#ifndef WALLAROO_DETAIL_DYNAMIC_LIBRARY_H_
#define WALLAROO_DETAIL_DYNAMIC_LIBRARY_H_

#include <string>
#include "detail/platform_specific_lib_macros.h"
#include WALLAROO_DLL_IMPL_HEADER // select the right OS-specific header-file for PlatformSpecificDynamicLibrary


namespace wallaroo
{
namespace detail
{

// Manage a shared library as a resource. Load it on the ctor
// and unload it on the dtor.
// Export a method to get a function pointer given the symbol name.
class DynamicLibrary : private PlatformSpecificDynamicLibrary
{
public:
    /* Create a DynamicLibrary from the path specified as parameter
    * loading the OS library.
    * @param fileName the path of the dynamic library to load
    * @throw WrongFile if the file does not exist or its format is wrong.
    */
    explicit DynamicLibrary( const std::string& fileName ) :
        PlatformSpecificDynamicLibrary( fileName )
    {
    }
    // Release the OS library
    ~DynamicLibrary()
    {
    }
    /* Return a function pointer to the symbol @c funcName.
    * @param funcName the name of the function in the library to load.
    * @return the function pointer to the symbol required. NULL if the symbol was not found.
    */
    template < typename F >
    F GetFunction( const std::string& funcName )
    {
        return PlatformSpecificDynamicLibrary::GetFunction< F >( funcName );
    }
    /* Returns the platform-specific filename suffix
		for shared libraries (including the period).
		In debug mode, the suffix also includes a
		"d" to specify the debug version of a library.
    */
    static std::string Suffix()
    {
        return PlatformSpecificDynamicLibrary::Suffix();
    }
};

} // namespace detail
} // namespace wallaroo

#endif // WALLAROO_DETAIL_DYNAMIC_LIBRARY_H_

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

#ifndef WALLAROO_DETAIL_DYNAMIC_LIBRARY_WIN32_H_
#define WALLAROO_DETAIL_DYNAMIC_LIBRARY_WIN32_H_

#include <string>
#include <windows.h>


namespace wallaroo
{
namespace detail
{

// win32 specific implementation of shared library
class PlatformSpecificDynamicLibrary
{
public:
    // throw WrongFile if the file does not exist or its format is wrong.
    explicit PlatformSpecificDynamicLibrary( const std::string& fileName )
    {
        libHandle = LoadLibrary( fileName.c_str() );
        if ( ! libHandle ) throw WrongFile( fileName );
    }
    // Release the OS library
    ~PlatformSpecificDynamicLibrary()
    {
        FreeLibrary( libHandle );
    }
    // Return a function pointer to the symbol funcName. NULL if the symbol was not found.
    template < typename F >
    F GetFunction( const std::string& funcName )
    {
        F f = (F)GetProcAddress( libHandle, funcName.c_str() );
        return f;
    }
    /* Returns the platform-specific filename suffix
       for shared libraries (including the period).
       In debug mode, the suffix also includes a
       "d" to specify the debug version of a library. */
    static std::string Suffix()
    {
        #if defined(_DEBUG)
	        return "d.dll";
        #else
	        return ".dll";
        #endif
    }
private:
    HINSTANCE libHandle;
};

} // namespace detail
} // namespace wallaroo

#endif // WALLAROO_DETAIL_DYNAMIC_LIBRARY_WIN32_H_

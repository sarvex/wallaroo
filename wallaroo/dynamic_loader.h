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
#include "cxx0x.h"
#include "device.h"
#include "class.h"
#include "exceptions.h"
#include "dyn_class_descriptor.h"
#include "platform_specific_lib_macros.h"
#include WALLAROO_DLL_IMPL_HEADER


namespace wallaroo
{
namespace detail
{


class DynamicLibrary : private PlatformSpecificDynamicLibrary
{
public:
    /** Create a DynamicLibrary from the path specified as parameter
    * loading the OS library.
    * @param fileName the path of the dynamic library to load
    * @throw WrongFile if the file does not exist or its format is wrong.
    */
    explicit DynamicLibrary( const std::string& fileName ) :
        PlatformSpecificDynamicLibrary( fileName )
    {
    }
    /** Release the OS library
    */
    ~DynamicLibrary()
    {
    }
    /** Return a function pointer to the symbol @c funcName.
    * @param funcName the name of the function in the library to load.
    * @return the function pointer to the symbol required. NULL if the symbol was not found.
    */
    template < typename F >
    F GetFunction( const std::string& funcName )
    {
        return PlatformSpecificDynamicLibrary::GetFunction< F >( funcName );
    }
    /** Returns the platform-specific filename suffix
		for shared libraries (including the period).
		In debug mode, the suffix also includes a
		"d" to specify the debug version of a library.
    */
    static std::string Suffix()
    {
        return PlatformSpecificDynamicLibrary::Suffix();
    }
};

template < typename Builder, typename Deleter >
struct BuildFunctor
{
    BuildFunctor( const Builder& _builder, const Deleter& _deleter ) : 
        builder( _builder ),
        deleter( _deleter )
    {}
    cxx0x::shared_ptr< Device > operator()()
    {
        return cxx0x::shared_ptr< Device >( builder(), deleter );
    }
private:
    Builder builder;
    Deleter deleter;
};

} // namespace detail


class Plugin
{
public:
    /** @@@
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
    static std::string Suffix()
    {
        return detail::DynamicLibrary::Suffix();
    }
private:
    /** @@@
    * @throw WrongFile if the file does not exist or its format is wrong.
    */
    Plugin( const std::string& fileName ) :
      library( fileName )
    {
    }
    detail::DynamicLibrary library;
};

} // namespace wallaroo

#endif // WALLAROO_DYNAMIC_LOADER_H_

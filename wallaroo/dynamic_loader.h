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

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif


namespace wallaroo
{
namespace detail
{


#ifdef _WIN32

class DynamicLibrary
{
public:
    /** Create a DynamicLibrary from the path specified as parameter
    * loading the OS library.
    * @param fileName the path of the dynamic library to load
    * @throw WrongFile if the file does not exist or its format is wrong.
    */
    explicit DynamicLibrary( const std::string& fileName )
    {
        libHandle = LoadLibrary( fileName.c_str() );
        if ( ! libHandle ) throw WrongFile( fileName );
    }
    /** Release the OS library
    */
    ~DynamicLibrary()
    {
        FreeLibrary( libHandle );
    }
    /** Return a function pointer to the symbol @c funcName.
    * @param funcName the name of the function in the library to load.
    * @return the function pointer to the symbol required. NULL if the symbol was not found.
    */
    template < class F >
    F GetFunction( const std::string& funcName )
    {
        F f = (F)GetProcAddress( libHandle, funcName.c_str() );
        return f;
    }
private:
    HINSTANCE libHandle;
};

#else

class DynamicLibrary
{
public:
    explicit DynamicLibrary( const std::string& fileName )
    {
        libHandle = dlopen( fileName.c_str(), RTLD_LAZY );
        if ( ! libHandle ) throw WrongFile( fileName );
    }
    ~DynamicLibrary()
    {
        dlclose( libHandle );
    }
    template < class F >
    F GetFunction( const std::string& funcName )
    {
        F f = (F)dlsym( libHandle, funcName.c_str() );
        if ( dlerror() != NULL ) return NULL;
        return f;
    }
private:
    void* libHandle;
};

#endif

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
    Plugin( const std::string& fileName ) :
      library( fileName )
    {
        using namespace detail;

        typedef Device* (*Builder)(void);
        typedef void (*Deleter)(Device*);
        //typedef void (*GetName)(std::string*);
        typedef void (*GetName)(char*);

        Builder builder = library.GetFunction< Builder >( "Create" );
        Deleter deleter = library.GetFunction< Deleter >( "Destroy" );
        GetName getName = library.GetFunction< GetName >( "GetName" );
        if ( builder == NULL || deleter == NULL || getName == NULL )
            throw WrongFile( fileName );
        BuildFunctor< Builder, Deleter > functor( builder, deleter );
        //std::string name;
        char name[ 300 ];
        //getName( &name );
        getName( name );
        Class< void, void >::Register( name, functor );
    }
    ~Plugin()
    {
    }
private:
    detail::DynamicLibrary library;
};

} // namespace wallaroo

#endif // WALLAROO_DYNAMIC_LOADER_H_

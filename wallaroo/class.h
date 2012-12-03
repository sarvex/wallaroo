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

#ifndef WALLAROO_CLASS_H_
#define WALLAROO_CLASS_H_

#include <string>
#include <map>
#include "cxx0x.h"

namespace wallaroo
{

class Device;


template < class P1, class P2 >
class Class
{
    public :

        typedef cxx0x::shared_ptr< Device > Ptr;
        typedef cxx0x::function< Ptr( const P1& p1, const P2& p2 ) > FactoryMethod;
        Ptr NewInstance( const P1& p1, const P2& p2 )
        {
            if( fm )
                return( fm( p1, p2 ) ) ;
            else
                return( Ptr() ) ;
        }
        static Class ForName( const std::string& name )
        {
            Classes::const_iterator i = Registry().find( name );
            if ( i != Registry().end() )
                return i -> second ;
            return Class(); // default value
        }
    private :
        FactoryMethod fm;
        typedef std::map< std::string, Class< P1, P2 > > Classes;
#if 0
        template < class P1, class P2 > friend class RegEntry;
#endif
        template < class T, class P1, class P2 > friend class Registration;
        static void Register( const std::string& s, const FactoryMethod& m )
        {
            Registry().insert( std::make_pair( s, Class( m ) ) );
        }
        static Classes& Registry()
        {
            static Classes registry;
            return registry;
        }
        Class() : fm( NULL )
        {
        }
        Class( FactoryMethod m ) :
            fm( m )
        {
        }
};


template < class P >
class Class< P, void >
{
    public :

        typedef cxx0x::shared_ptr< Device > Ptr;
        typedef cxx0x::function< Ptr( const P& p ) > FactoryMethod;
        Ptr NewInstance( const P& p )
        {
            if( fm )
                return( fm( p ) ) ;
            else
                return( Ptr() ) ;
        }
        static Class ForName( const std::string& name )
        {
            Classes::const_iterator i = Registry().find( name );
            if ( i != Registry().end() )
                return i -> second ;
            return Class(); // default value
        }
    private :
        FactoryMethod fm;
        typedef std::map< std::string, Class< void, void > > Classes;
#if 0
        template < class P1, class P2 > friend class RegEntry;
#endif
        template < class T, class P1, class P2 > friend class Registration;
        static void Register( const std::string& s, const FactoryMethod& m )
        {
            Registry().insert( std::make_pair( s, Class( m ) ) );
        }
        static Classes& Registry()
        {
            static Classes registry;
            return registry;
        }
        Class() : fm( NULL )
        {
        }
        Class( FactoryMethod m ) :
            fm( m )
        {
        }
};



template <>
class Class< void, void >
{
    public :

        typedef cxx0x::shared_ptr< Device > Ptr;
        typedef cxx0x::function< Ptr() > FactoryMethod;
        Ptr NewInstance()
        {
            if( fm )
                return( fm() ) ;
            else
                return( Ptr() ) ;
        }
        static Class ForName( const std::string& name )
        {
            Classes::const_iterator i = Registry().find( name );
            if ( i != Registry().end() )
                return i -> second ;
            return Class(); // default value
        }
    private :
        FactoryMethod fm;
        typedef std::map< std::string, Class< void, void > > Classes;
#if 0
        template < class P1, class P2 > friend class RegEntry;
#endif
        template < class T, class P1, class P2 > friend class Registration;
        static void Register( const std::string& s, const FactoryMethod& m )
        {
            Registry().insert( std::make_pair( s, Class( m ) ) );
        }
        static Classes& Registry()
        {
            static Classes registry;
            return registry;
        }
        Class() : fm( NULL )
        {
        }
        Class( FactoryMethod m ) :
            fm( m )
        {
        }
};



template < class T, class P1, class P2 >
class Factory
{
public:
    static cxx0x::shared_ptr< Device > Create( const P1& p1, const P2& p2 )
    {
        return cxx0x::make_shared< T >( p1, p2 );
    }
};

template < class T >
class Factory< T, void, void >
{
public:
    static cxx0x::shared_ptr< Device > Create()
    {
        return cxx0x::make_shared< T >();
    }
};

#if 0
template < class P1, class P2 >
class RegEntry
{
public:
    RegEntry( const std::string& name, const typename Class< P1, P2 >::FactoryMethod& fm )
    {
        Class< P1, P2 >::Register( name, fm );
    }
};
#endif


template < class T, class P1 = void, class P2 = void >
class Registration
{
public:
    Registration( const std::string& name )
    {
        Class< P1, P2 >::FactoryMethod fm( &Factory< T, P1, P2 >::Create );
        Class< P1, P2 >::Register( name, fm );
    }
};

} // namespace

/** This macro must be used in your header file for declaring a class that will be 
 * automatically registered and thus retrieved throug the method @c Class::ForName( C ).
 * @param C The class name
 * @param P1 The type of the first parameter of the class constructor (possibly void)
 * @param P2 The type of the second parameter of the class constructor (possibly void)
 * @hideinitializer
 */
#if 0
#define REGISTER( C, P1, P2 ) \
    static const RegEntry< P1, P2 > C##r( #C, &Factory< C, P1, P2 >::Create ) ;
#else
#define REGISTER( C, P1, P2 ) \
    static const Registration< C, P1, P2 > C##r( #C ) ;
#endif

#endif // WALLAROO_CLASS_H_

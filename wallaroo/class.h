/*******************************************************************************
 * wallaroo - A library for configurable creation and wiring of C++ classes.
 * Copyright (C) 2012 Daniele Pallastrelli
 *
 * This file is part of wallaroo.
 * For more information, see http://wallaroo.googlecode.com/
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

#ifndef WALLAROO_CLASS_H_
#define WALLAROO_CLASS_H_

#include <string>
#include "detail/factory.h"
#include "cxx0x.h"
#include "part.h"

namespace wallaroo
{

// forward declarations
class Plugin;

/** This is the description of a class having a constructor that 
* takes two parameters of type @c P1 and @c P2 and implements
* the interface @c Part.
* The class provides a method to get an instance of the described class.
* It also provides a static registry of the instances of this class,
* and methods to register and retrieve them.
*/
template < class P1, class P2 >
class Class
{
    public :

        typedef cxx0x::shared_ptr< Part > Ptr;
        typedef cxx0x::function< Ptr( const P1& p1, const P2& p2 ) > FactoryMethod;

        /** Create an instance of the class described by this object.
        * @param p1 The first parameter to pass to the constructor
        * @param p2 The second parameter to pass to the constructor
        * @return a shared_ptr to the new instance (or the empty 
        * shared_ptr if the descriptor is not valid)
        */
        Ptr NewInstance( const P1& p1, const P2& p2 ) const
        {
            if( fm )
                return( fm( p1, p2 ) ) ;
            else
                return( Ptr() ) ;
        }

        /** Return the @c Class< P1, P2 > registered with the name @c name.
        */
        static Class ForName( const std::string& name )
        {
            typename Classes::const_iterator i = Registry().find( name );
            if ( i != Registry().end() )
                return i -> second;
            return Class(); // default value
        }
    private :
        FactoryMethod fm;
        typedef cxx0x::unordered_map< std::string, Class< P1, P2 > > Classes;
        template < class T, class T1, class T2 > friend class Registration;
        static void Register( const std::string& s, const FactoryMethod& m )
        {
            Registry().insert( std::make_pair( s, Class( m ) ) );
        }
        static Classes& Registry()
        {
            static Classes registry;
            return registry;
        }
        Class()
        {
        }
        Class( FactoryMethod m ) :
            fm( m )
        {
        }
};

/** This is the description of a class having a constructor that
* takes one parameter of type @c P and implements
* the interface @c Part.
* The class provides a method to get an instance of the described class.
* It also provides a static registry of the instances of this class,
* and methods to register and retrieve them.
*/
template < class P >
class Class< P, void >
{
    public :

        typedef cxx0x::shared_ptr< Part > Ptr;
        typedef cxx0x::function< Ptr( const P& p ) > FactoryMethod;

        /** Create an instance of the class described by this object.
        * @param p The parameter to pass to the constructor
        * @return a shared_ptr to the new instance (or the empty
        * shared_ptr if the descriptor is not valid)
        */
        Ptr NewInstance( const P& p ) const
        {
            if( fm )
                return( fm( p ) ) ;
            else
                return( Ptr() ) ;
        }

        /** Return the @c Class< P, void > registered with the name @c name.
        */
        static Class ForName( const std::string& name )
        {
            typename Classes::const_iterator i = Registry().find( name );
            if ( i != Registry().end() )
                return i -> second ;
            return Class(); // default value
        }
    private :
        FactoryMethod fm;
        typedef cxx0x::unordered_map< std::string, Class< P, void > > Classes;
        template < class T, class T1, class T2 > friend class Registration;
        static void Register( const std::string& s, const FactoryMethod& m )
        {
            Registry().insert( std::make_pair( s, Class( m ) ) );
        }
        static Classes& Registry()
        {
            static Classes registry;
            return registry;
        }
        Class()
        {
        }
        Class( FactoryMethod m ) :
            fm( m )
        {
        }
};


/** This is the description of a class having a constructor that
* takes no parameters and implements the interface @c Part.
* The class provides a method to get an instance of the described class.
* It also provides a static registry of the instances of this class,
* and methods to register and retrieve them.
*/
template <>
class Class< void, void >
{
    public :

        typedef cxx0x::shared_ptr< Part > Ptr;
        typedef cxx0x::function< Ptr() > FactoryMethod;

        /** Create an instance of the class described by this object.
        * @return a shared_ptr to the new instance (or the empty
        * shared_ptr if the descriptor is not valid)
        */
        Ptr NewInstance() const
        {
            if( fm )
            {
                Ptr p = fm();
                p -> Source( plugin ); // set the ref count to shared library
                return p;
            }
            else
                return( Ptr() );
        }

        /** Return the @c Class< void, void > registered with the name @c name.
        */
        static Class ForName( const std::string& name )
        {
            Classes::const_iterator i = Registry().find( name );
            if ( i != Registry().end() )
                return i -> second ;
            return Class(); // default value
        }
    private :
        FactoryMethod fm;
        cxx0x::shared_ptr< Plugin > plugin; // optional shared ptr to plugin, to release the shared library when is no more used
        typedef cxx0x::unordered_map< std::string, Class< void, void > > Classes;
        template < class T, class T1, class T2 > friend class Registration;
        friend class Plugin;
        static void Register( const std::string& s, const FactoryMethod& m )
        {
            Registry().insert( std::make_pair( s, Class( m ) ) );
        }
        static void Register( const std::string& s, const FactoryMethod& m, const cxx0x::shared_ptr< Plugin >& plugin )
        {
            Registry().insert( std::make_pair( s, Class( m, plugin ) ) );
        }
        static Classes& Registry()
        {
            static Classes registry;
            return registry;
        }
        Class()
        {
        }
        Class( FactoryMethod m ) :
            fm( m )
        {
        }
        Class( FactoryMethod m, const cxx0x::shared_ptr< Plugin >& p ) :
            fm( m ),
            plugin( p )
        {
        }
};


/** This class registers on its constructor a class @c T
*   with its factory method.
*/
template < class T, class P1 = void, class P2 = void >
class Registration
{
public:
    Registration( const std::string& name )
    {
        typename Class< P1, P2 >::FactoryMethod fm( &detail::Factory< T, P1, P2 >::Create );
        Class< P1, P2 >::Register( name, fm );
    }
};

} // wallaroo namespace

#endif // WALLAROO_CLASS_H_

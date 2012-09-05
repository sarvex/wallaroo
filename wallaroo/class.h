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
#include <list>
#include "cxx0x.h"

namespace wallaroo
{

template < class I, class P1, class P2 > class Registry;
template < class I, class P1, class P2 > class RegEntry;

/** It describes a class with a constructor that takes
* two parameters of type @c P1 and @c P2 and implements
* the interface @c I
*/
template < class I, class P1, class P2 >
class Class
{
    public :

        typedef cxx0x::shared_ptr< I > (*FactoryMethod)( P1, P2 ) ;
        typedef std::list< Class< I, P1, P2 > > ClassSet;
        /** Create an instance of the class described by this object.
        * @param p1 The first parameter to pass to the constructor
        * @param p2 The second parameter to pass to the constructor
        * @return a shared_ptr to the new instance (or the empty 
        * shared_ptr if the descriptor is not valid)
        */
        cxx0x::shared_ptr< I > NewInstance( const P1& p1, const P2& p2 )
        {
            if( fm != NULL )
                return( fm( p1, p2 ) ) ;
            else
                return( cxx0x::shared_ptr< I >() ) ;
        }
        /** Return the name of the class described by this object
        */
        const std::string& Name() const { return name; }
        /** Return a list of all the @c Class< I, P1, P2 > registered.
        */
        static ClassSet All()
        {
            return Reg().All();
        }
        /** Return the @c Class< I, P1, P2 > registered with the name @c name.
        */
        static Class ForName( const std::string& name )
        {
            return Reg().ForName( name );
        }
    private :
        static Registry< I, P1, P2 >& Reg()
        {
            static Registry< I, P1, P2 > reg;
            return reg;
        }
        const std::string name ;
        FactoryMethod fm;
        friend class Registry< I, P1, P2 >;
        friend class RegEntry< I, P1, P2 >;
        static void Register( const std::string& s, FactoryMethod m )
        {
            Reg().Add( Class( s, m ) );
        }
        Class() : name( "" ), fm( NULL )
        {
        }
        Class( const std::string& s, FactoryMethod m ) :
            name( s ), fm( m )
        {
        }
};

/** It describes a class with a constructor that takes
* two parameters of type @c P1 and @c P2
*/
template < class I, class P >
class Class< I, P, void >
{
    public :

        typedef cxx0x::shared_ptr< I > (*FactoryMethod)( P ) ;
        typedef std::list< Class< I, P, void > > ClassSet;
        /** Create an instance of the class described by this object.
        * @param p The parameter to pass to the constructor
        * @return a shared_ptr to the new instance (or the empty 
        * shared_ptr if the descriptor is not valid)
        */
        cxx0x::shared_ptr< I > NewInstance( const P& p )
        {
            if( fm != NULL )
                return( fm( p ) ) ;
            else
                return( cxx0x::shared_ptr< I >() ) ;
        }
        /** Return the name of the class described by this object
        */
        const std::string& Name() const { return name; }
        /** Return a list of all the @c Class< I, P1, P2 > registered.
        */
        static ClassSet All()
        {
            return Reg().All();
        }
        /** Return the @c Class< I, P1, P2 > registered with the name @c name.
        */
        static Class ForName( const std::string& name )
        {
            return Reg().ForName( name );
        }
    private :
        static Registry< I, P, void >& Reg()
        {
            static Registry< I, P, void > reg;
            return reg;
        }
        const std::string name ;
        FactoryMethod fm;
        friend class Registry< I, P, void >;
        friend class RegEntry< I, P, void >;
        static void Register( const std::string& s, FactoryMethod m )
        {
            Reg().Add( Class( s, m ) );
        }
        Class() : name( "" ), fm( NULL )
        {
        }
        Class( const std::string& s, FactoryMethod m ) :
            name( s ), fm( m )
        {
        }
};

/** It describes a class with a default constructor.
*/
template < class I >
class Class< I, void, void >
{
    public :

        typedef cxx0x::shared_ptr< I > (*FactoryMethod)() ;
        typedef std::list< Class< I, void, void > > ClassSet;
        /** Create an instance of the class described by this object.
         * @return a shared_ptr to the new instance (or the empty 
         * shared_ptr if the descriptor is not valid)
        */
        cxx0x::shared_ptr< I > NewInstance()
        {
            if( fm != NULL )
                return( fm() ) ;
            else
                return( cxx0x::shared_ptr< I >() ) ;
        }
        /** Return the name of the class described by this object
        */
        const std::string& Name() const { return name; }
        /** Return a list of all the @c Class< I, P1, P2 > registered.
        */
        static ClassSet All()
        {
            return Reg().All();
        }
        /** Return the @c Class< I, P1, P2 > registered with the name @c name.
        */
        static Class ForName( const std::string& name )
        {
            return Reg().ForName( name );
        }
    private :
        static Registry< I, void, void >& Reg()
        {
            static Registry< I, void, void > reg;
            return reg;
        }
        const std::string name ;
        FactoryMethod fm;
        friend class Registry< I, void, void >;
        friend class RegEntry< I, void, void >;
        static void Register( const std::string& s, FactoryMethod m )
        {
            Reg().Add( Class( s, m ) );
        }
        Class() : name( "" ), fm( NULL )
        {
        }
        Class( const std::string& s, FactoryMethod m ) :
            name( s ), fm( m )
        {
        }
};


template < class I, class P1, class P2 >
class Registry
{
public:

    typedef typename Class< I, P1, P2 >::ClassSet ClassSet;

    void Add( Class< I, P1, P2 > c )
    {
        classes.insert( std::make_pair( c.Name(), c ) );
    }
    Class< I, P1, P2 > ForName( const std::string& name ) const
    {
        typename ClassMap::const_iterator i = classes.find( name );
        if ( i == classes.end() )
            return Class< I, P1, P2 >();  // valore di default
        else
            return i -> second ;
    }
    ClassSet All() const
    {
        ClassSet result;
        for ( typename ClassMap::const_iterator i = classes.begin(); i != classes.end(); ++i )
            result.push_back( i -> second );
        return result;
    }

private:
    typedef std::map< std::string, Class< I, P1, P2 > > ClassMap;
    friend class Class< I, P1, P2 >;
    Registry() {}
    ClassMap classes;
};

template< class T, class I, class P1, class P2 >
class Registered
{
    public :
        static cxx0x::shared_ptr< I > NewInstance( P1 p1, P2 p2 )
        {
            return( cxx0x::shared_ptr< I >( new T( p1, p2 ) ) ) ;
        }
};

template< class T, class I, class P >
class Registered< T, I, P, void >  
{
    public :
        static cxx0x::shared_ptr< I > NewInstance( P p )
        {
            return( cxx0x::shared_ptr< I >( new T( p ) ) ) ;
        }
};

template< class T, class I >
class Registered< T, I, void, void >
{
    public :
        static cxx0x::shared_ptr< I > NewInstance()
        {
            return( cxx0x::shared_ptr< I >( new T() ) ) ;
        }
};


template < class I, class P1, class P2 >
class RegEntry
{
    public :
        typedef typename Class< I, P1, P2 >::FactoryMethod FactoryMethod;
        RegEntry( const char s[], FactoryMethod f )
        {
            Class< I, P1, P2 >::Register( s, f );
        }
};

} // namespace

/** This macro must be used in your header file for declaring a class that will be 
* automatically registered and thus retrieved throug the method @c Class::ForName( C ).
* @param C The class name
* @param P1 The type of the first parameter of the class constructor (possibly void)
* @param P2 The type of the second parameter of the class constructor (possibly void)
*/
#define REGISTER( C, P1, P2 ) \
    static const RegEntry< Device, P1, P2 > C##r( #C, &Registered<C,Device,P1,P2>::NewInstance ) ;

/** This macro must be used in your implementation file for defining a class that will be
* automatically registered and thus retrieved throug the method @c Class::ForName( C ).
* @param C The class name
* @param P1 The type of the first parameter of the class constructor (possibly void)
* @param P2 The type of the second parameter of the class constructor (possibly void)
*/
#define REGISTERED_CLASS( C, P1, P2 ) \
    class C : public Registered< C, Device, P1, P2 >


#endif // WALLAROO_CLASS_H_

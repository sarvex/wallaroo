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
#include <memory>
#include <list>

namespace wallaroo
{

template < class I, class P1, class P2 > class Registry;
template < class I, class P1, class P2 > class RegEntry;

// è il descrittore di una classe con un costruttore
// che prende un parametro di tipo P1 e uno di tipo P2
template < class I, class P1, class P2 >
class Class
{
    public :

        typedef std::auto_ptr< I > (*FactoryMethod)( P1, P2 ) ;

        typedef std::list< Class< I, P1, P2 > > ClassSet;
        std::auto_ptr< I > NewInstance( const P1& p1, const P2& p2 )
        {
            if( fm != NULL )
                return( fm( p1, p2 ) ) ;
            else
                return( std::auto_ptr< I >() ) ;
        }
        const std::string& Name() const { return name; }
        static ClassSet All()
        {
            return Reg().All();
        }
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

// è il descrittore di una classe con un costruttore
// che prende un parametro di tipo P
template < class I, class P >
class Class< I, P, void >
{
    public :

        typedef std::auto_ptr< I > (*FactoryMethod)( P ) ;

        typedef std::list< Class< I, P, void > > ClassSet;
        std::auto_ptr< I > NewInstance( const P& p )
        {
            if( fm != NULL )
                return( fm( p ) ) ;
            else
                return( std::auto_ptr< I >() ) ;
        }
        const std::string& Name() const { return name; }
        static ClassSet All()
        {
            return Reg().All();
        }
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

// specializzazione della classe Class nel caso di P=void
// (per le classi con il costruttore di default)
template < class I >
class Class< I, void, void >
{
    public :

        typedef std::auto_ptr< I > (*FactoryMethod)() ;

        typedef std::list< Class< I, void, void > > ClassSet;
        std::auto_ptr< I > NewInstance()
        {
            if( fm != NULL )
                return( fm() ) ;
            else
                return( std::auto_ptr< I >() ) ;
        }
        const std::string& Name() const { return name; }
        static ClassSet All()
        {
            return Reg().All();
        }
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
        static std::auto_ptr< I > NewInstance( P1 p1, P2 p2 )
        {
            return( std::auto_ptr< I >( new T( p1, p2 ) ) ) ;
        }
};

template< class T, class I, class P >
class Registered< T, I, P, void >  
{
    public :
        static std::auto_ptr< I > NewInstance( P p )
        {
            return( std::auto_ptr< I >( new T( p ) ) ) ;
        }
};

template< class T, class I >
class Registered< T, I, void, void >
{
    public :
        static std::auto_ptr< I > NewInstance()
        {
            return( std::auto_ptr< I >( new T() ) ) ;
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

#define REGISTER( C, P1, P2 ) \
    static const RegEntry< Device, P1, P2 > r( #C, &Registered<C,Device,P1,P2>::NewInstance ) ;

#define REGISTERED_CLASS( C, P1, P2 ) \
    class C : public Registered< C, Device, P1, P2 >


#endif // WALLAROO_CLASS_H_

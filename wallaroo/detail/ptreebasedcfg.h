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

#ifndef WALLAROO_DETAIL_PTREEBASEDCFG_H_
#define WALLAROO_DETAIL_PTREEBASEDCFG_H_

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "wallaroo/catalog.h"
#include "wallaroo/dynamic_loader.h"

using namespace boost::property_tree;

namespace wallaroo
{
namespace detail
{

// ********************************************************
// supported types

template < typename T >
struct TypeDesc {};

template <>
struct TypeDesc< char >
{
    static const char* Name() { return "char"; }
};

template <>
struct TypeDesc< unsigned char >
{
    static const char* Name() { return "unsigned char"; }
};

template <>
struct TypeDesc< int >
{
    static const char* Name() { return "int"; }
};

template <>
struct TypeDesc< unsigned int >
{
    static const char* Name() { return "unsigned int"; }
};

template <>
struct TypeDesc< long >
{
    static const char* Name() { return "long"; }
};

template <>
struct TypeDesc< double >
{
    static const char* Name() { return "double"; }
};

template <>
struct TypeDesc< bool >
{
    static const char* Name() { return "bool"; }
};

template <>
struct TypeDesc< std::string >
{
    static const char* Name() { return "string"; }
};


// ********************************************************

template < typename T1, typename T2 >
static bool Create( 
Catalog& catalog,
    const std::string& instance,
    const std::string& cl,
    boost::optional< const ptree& > tree1,
    boost::optional< const ptree& > tree2
)
{
    const std::string& type1 = tree1 -> get< std::string >( "type" );
    const std::string& type2 = tree2 -> get< std::string >( "type" );
    if ( type1 == TypeDesc< T1 >::Name() && type2 == TypeDesc< T2 >::Name() )
    {
        const T1& p1 = tree1 -> get< T1 >( "value" );
        const T2& p2 = tree2 -> get< T2 >( "value" );
        catalog.Create( instance, cl, p1, p2 );
        return true;
    }
    return false;
}

template < typename T >
static bool Create(
    Catalog& catalog,
    const std::string& instance,
    const std::string& cl,
    boost::optional< const ptree& > tree
)
{
    const std::string& type = tree -> get< std::string >( "type" );
    if ( type == TypeDesc< T >::Name() )
    {
        const T& p = tree -> get< T >( "value" );
        catalog.Create( instance, cl, p );
        return true;
    }
    return false;
}

// ********************************************************

// This class can parse a boost::ptree structure containing a list of objects to
// be created and their wiring.
// Then it can populate a Catalog with that objects.
class PtreeBasedCfg
{
public:
    // Create a PtreeBasedCfg using the boost::ptree passed as parameter.
    explicit PtreeBasedCfg( const ptree& _tree ) :
        tree( _tree )
    {
    }

    // Load the plugins specified in the ptree.
    // throw WrongFile if the ptree contains a semantic error.
    void LoadPlugins()
    {
        try
        {
            Foreach( "wallaroo.plugins", boost::bind( &PtreeBasedCfg::ParsePlugin, this, _1 ) );
        }
        catch ( const ptree_error& e )
        {
            throw WrongFile( e.what() );
        }
    }

    // Fill the catalog with the objects and relations specified in the ptree.
    // throw WrongFile if the ptree contains a semantic error.
    void Fill( Catalog& catalog )
    {
        try
        {
            Foreach( "wallaroo.devices", boost::bind( &PtreeBasedCfg::ParseObject, this, boost::ref( catalog ), _1 ) );
            Foreach( "wallaroo.wiring", boost::bind( &PtreeBasedCfg::ParseRelation, this, boost::ref( catalog ), _1 ) );
        }
        catch ( const ptree_error& e )
        {
            throw WrongFile( e.what() );
        }
    }

private:

    template < typename F >
    void Foreach( const std::string& key, F f )
    {
        boost::optional< const ptree & > value = tree.get_child_optional( key );
        if ( value )
        {
            BOOST_FOREACH( const ptree::value_type &v, *value )
            {
                f( v.second );
            }
        }
    }

    void ParsePlugin( const ptree& v )
    {
        const std::string& shared = v.get_value< std::string >();
        Plugin::Load( shared + Plugin::Suffix() );
    }

    void ParseObject( Catalog& catalog, const ptree& v )
    {
        const std::string& name = v.get< std::string >( "name" );
        const std::string& cl = v.get< std::string >( "class" );

        boost::optional< const ptree& > par1 = v.get_child_optional( "parameter1" );
        boost::optional< const ptree& > par2 = v.get_child_optional( "parameter2" );

        if ( par1 && par2 )
        {

            if ( Create< std::string, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, unsigned char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, double >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, bool >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< std::string, long >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< int, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, unsigned char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, double >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, bool >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< int, long >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< unsigned int, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, unsigned char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, double >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, bool >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned int, long >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< double, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, unsigned char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, double >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, bool >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< double, long >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< bool, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< bool, char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< bool, unsigned char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< bool, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< bool, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< bool, double >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< bool, bool >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< bool, long >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< long, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< long, char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< long, unsigned char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< long, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< long, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< long, double >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< long, bool >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< long, long >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< char, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< char, char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< char, unsigned char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< char, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< char, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< char, double >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< char, bool >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< char, long >( catalog, name, cl, par1, par2 ) ) return;

            if ( Create< unsigned char, std::string >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned char, char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned char, unsigned char >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned char, int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned char, unsigned int >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned char, double >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned char, bool >( catalog, name, cl, par1, par2 ) ) return;
            if ( Create< unsigned char, long >( catalog, name, cl, par1, par2 ) ) return;
        }
        else if ( par1 )
        {
            if ( Create< std::string >( catalog, name, cl, par1 ) ) return;
            if ( Create< char >( catalog, name, cl, par1 ) ) return;
            if ( Create< unsigned char >( catalog, name, cl, par1 ) ) return;
            if ( Create< int >( catalog, name, cl, par1 ) ) return;
            if ( Create< unsigned int >( catalog, name, cl, par1 ) ) return;
            if ( Create< long >( catalog, name, cl, par1 ) ) return;
            if ( Create< double >( catalog, name, cl, par1 ) ) return;
            if ( Create< bool >( catalog, name, cl, par1 ) ) return;
        }
        else
        {
            catalog.Create( name, cl );
        }
    }

    void ParseRelation( Catalog& catalog, const ptree& v )
    {
        const std::string& source = v.get< std::string >( "source" );
        const std::string& dest = v.get< std::string >( "dest" );
        const std::string& role = v.get< std::string >( "plug" );

        use( catalog[ dest ] ).as( role ).of( catalog[ source ] );
    }

    const ptree& tree;
};

} // detail
} // namespace

#endif // WALLAROO_DETAIL_PTREEBASEDCFG_H_

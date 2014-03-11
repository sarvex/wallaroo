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

#ifndef WALLAROO_DETAIL_DYN_CLASS_DESCRIPTOR_H_
#define WALLAROO_DETAIL_DYN_CLASS_DESCRIPTOR_H_

#include <string>
#include <vector>
#include "wallaroo/cxx0x.h"

namespace wallaroo
{

class Device; // forward declaration

namespace detail
{

// Store name and factory for a class
struct Descriptor
{
    // Insert a new Descriptor into DB
    template < typename T >
    static void Insert( const std::string& className );

    // Unique instance of Descriptor's DB
    static std::vector< Descriptor >& DB();

    typedef cxx0x::shared_ptr< Device > Ptr;
    // NOTE: when compiled with visual studio 2012, std::function gives protection error when
    //       the dll is unloaded. We must use a plain function pointer, instead. (thanks Lars Fiedler).
    //typedef cxx0x::function< Ptr() > FactoryMethod;
    typedef Ptr (*FactoryMethod)(void);

    FactoryMethod create; // the factory method
    std::string name; // the class name
};

// This is a utility class that register a new Descriptor on its ctor
template < typename T >
class DynRegistration
{
public:
    explicit DynRegistration( const std::string& name );
};

} // namespace detail
} // namespace wallaroo

#endif // WALLAROO_DYN_CLASS_DESCRIPTOR_H_


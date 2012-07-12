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

#include <iostream>
//#include <algorithm>
#include <boost/bind.hpp>
#include "b.h"

REGISTER( B, void, void )

B::B()  :
    c( "x", this ),
    cList( "xList", this )
{
}

void B::F()
{
    std::cout << "B::F method begin" << std::endl;
    std::cout << "invoking x -> G():" << std::endl;
    c -> G();
    std::cout << "iterating over xList:" << std::endl;
    //std::for_each( cList.begin(), cList.end(), std::mem_fun( &C::G ) );
#if 0
    std::for_each( cList.begin(), cList.end(), boost::bind( &C::G, _1 ) );
#else
    for ( MultiplePlug< C >::iterator i = cList.begin(); i != cList.end(); ++i )
    {
        boost::shared_ptr< C > s = i -> lock();
        if ( s ) 
            s -> G();
        else
            std::cerr << "an element in MultiplePlug has been deleted!" << std::endl;
    }
#endif
    std::cout << "B::F method end" << std::endl;
}

B::~B()
{
}


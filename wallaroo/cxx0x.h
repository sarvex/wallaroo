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

#ifndef WALLAROO_CXX0X_H_
#define WALLAROO_CXX0X_H_


#ifdef __GXX_EXPERIMENTAL_CXX0X__
    #define WALLAROO_HAS_CXX0X
#endif

#if defined(__cplusplus) && (__cplusplus  > 199711L)
    #define WALLAROO_HAS_CXX0X
#endif

#if defined(nullptr_t)
	#define WALLAROO_HAS_CXX0X
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#define WALLAROO_HAS_CXX0X
#endif


#ifdef WALLAROO_FORCE_USE_BOOST
    #undef WALLAROO_HAS_CXX0X
#endif

#ifdef WALLAROO_FORCE_USE_STD
	#define WALLAROO_HAS_CXX0X
#endif


#ifdef WALLAROO_HAS_CXX0X
    #include <memory>
    namespace cxx0x = std;
#else
    #include <boost/tr1/memory.hpp>
    namespace cxx0x = std::tr1;
	namespace boost
	{
		using namespace std::tr1;
	}
#endif


#endif

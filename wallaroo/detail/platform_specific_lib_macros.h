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

#ifndef WALLAROO_DETAIL_PLATFORM_SPECIFIC_LIB_MACROS_H_
#define WALLAROO_DETAIL_PLATFORM_SPECIFIC_LIB_MACROS_H_


#if defined(__FreeBSD__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(_AIX) || defined(__TOS_AIX__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(hpux) || defined(_hpux)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(__digital__) || defined(__osf__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(__NetBSD__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(__OpenBSD__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(sgi) || defined(__sgi)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(sun) || defined(__sun)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(__QNX__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(unix) || defined(__unix) || defined(__unix__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#elif defined(_WIN32_WCE)
	#define WALLAROO_DETAIL_OS_FAMILY_WINDOWS 1
#elif defined(_WIN32) || defined(_WIN64)
	#define WALLAROO_DETAIL_OS_FAMILY_WINDOWS 1
#elif defined(__CYGWIN__)
	#define WALLAROO_DETAIL_OS_FAMILY_UNIX 1
#else
  #error "Unknown Platform."
#endif

#if defined(WALLAROO_DETAIL_OS_FAMILY_WINDOWS)
    #define WALLAROO_DLL_PREFIX extern "C" __declspec(dllexport)
    #define WALLAROO_DLL_IMPL_HEADER "wallaroo/detail/dynamic_library_WIN32.h"
#elif defined(WALLAROO_DETAIL_OS_FAMILY_UNIX)
    #define WALLAROO_DLL_PREFIX extern "C" 
    #define WALLAROO_DLL_IMPL_HEADER "wallaroo/detail/dynamic_library_UNIX.h"
#else
    #error "Unknown Platform."
#endif

#endif // WALLAROO_DETAIL_PLATFORM_SPECIFIC_LIB_MACROS_H_


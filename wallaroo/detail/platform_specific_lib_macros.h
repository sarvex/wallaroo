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


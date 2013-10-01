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

#ifndef WALLAROO_DYNAMIC_LIB_H_
#define WALLAROO_DYNAMIC_LIB_H_

#include <vector>
#include "detail/dyn_class_descriptor.h"
#include "detail/dyn_class_descriptor_impl.h"
#include "detail/platform_specific_lib_macros.h"

/** This macro must be used in the shared libraries
 * to register a class. When a class is registered, you can create an instance
 * using wallaroo::Catalog::Create().
 * Please note you can put multiple registration clauses, if you have multiple classes
 * defined in the same shared library.
 * @param C The class name
 * @hideinitializer
 */
#define WALLAROO_DYNLIB_REGISTER( C ) \
    static wallaroo::detail::DynRegistration< C > C##p( #C );

// This function is exported by the shared library when you include this header
// file. It provides a container of descriptors of the classes exported by
// the library.
WALLAROO_DLL_PREFIX
std::vector< wallaroo::detail::Descriptor >* GetClasses()
{
    return &wallaroo::detail::Descriptor::DB();
}

#endif // WALLAROO_DYNAMIC_LIB_H_


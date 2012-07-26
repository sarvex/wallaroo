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

#ifndef WALLAROO_EXCEPTIONS_H_
#define WALLAROO_EXCEPTIONS_H_

#include <stdexcept>

namespace wallaroo
{

/** Base class for all Wallaroo errors. Derives from
* @c std::runtime_error. Call member function @c what to get human
* readable message associated with the error.
*/
class WallarooError : public std::runtime_error
{
public:
    /** Instantiate a WallarooError instance with the given message.
    * @param what The message to associate with this error.
    */
    WallarooError( const std::string &what ) : 
        std::runtime_error( what ) 
    {
    }

    ~WallarooError() throw()
    {
    }
};


/** Error indicating that specified Device does not exist anymore because it has
* been deleted. Derives from WallarooError.
*/
class DeletedDeviceError : public WallarooError
{
public:
    /// Instantiate a DeletedDeviceError
    DeletedDeviceError() :
        WallarooError( "The device has been deleted" )
    {
    }

    ~DeletedDeviceError() throw()
    {
    }
};


} // namespace

#endif
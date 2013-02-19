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


/** Error indicating that a @c use expression has been used outside a 
*   wallaroo_within section. Derives from WallarooError.
*/
class CatalogNotSpecified : public WallarooError
{
public:
    /// Instantiate a CatalogNotSpecified
    CatalogNotSpecified() :
        WallarooError( "The catalog has not been specified. You must use a wallaroo_within section." )
    {
    }

    ~CatalogNotSpecified() throw()
    {
    }
};


/** Error indicating that a cast has been performed with the wrong types.
*   Derives from WallarooError.
*/
class WrongType : public WallarooError
{
public:
    /// Instantiate a WrongType
    WrongType() :
        WallarooError( "The type is wrong for the cast." )
    {
    }

    ~WrongType() throw()
    {
    }
};


/** Error indicating that an item was not found in the catalog or in the class.
*   Derives from WallarooError.
*/
class ElementNotFound : public WallarooError
{
public:
    /// Instantiate a ElementNotFound
    ElementNotFound( const std::string& _element ) :
        WallarooError( _element + " not found in the container" ),
        element( _element )
    {
    }
    ~ElementNotFound() throw()
    {
    }
    const std::string& Element() const
    {
        return element;
    }
private:
    const std::string element;
};


/** Error indicating that an item was already inserted in the catalog or in the class.
*   Derives from WallarooError.
*/
class DuplicatedElement : public WallarooError
{
public:
    /// Instantiate a DuplicatedElement
    DuplicatedElement( const std::string& _element ) :
        WallarooError( _element  + " already in the container" ),
        element( _element )
    {
    }
    ~DuplicatedElement() throw()
    {
    }
    const std::string& Element() const
    {
        return element;
    }
private:
    const std::string element;
};


/** Error indicating that the file does not exist or contains a semantic error.
*   Derives from WallarooError.
*/
class WrongFile : public WallarooError
{
public:
    /// Instantiate a WrongFile
    explicit WrongFile( const std::string& msg ) :
        WallarooError( msg )
    {
    }

    ~WrongFile() throw()
    {
    }
};

/** Error indicating that a plug has not been wired correctly.
*   Derives from WallarooError.
*/
class WiringError : public WallarooError
{
public:
    /// Instantiate a WiringError
    WiringError( const std::string& _element ) :
        WallarooError( _element  + " wiring error" ),
        element( _element )
    {
    }
    ~WiringError() throw()
    {
    }
    const std::string& Element() const
    {
        return element;
    }
private:
    const std::string element;
};

} // namespace

#endif

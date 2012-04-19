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

#ifndef CONSTDIGITALINPUT_H_
#define CONSTDIGITALINPUT_H_

#include <string>
#include "digitalinput.h"
#include "wallaroo/registered.h"

REGISTERED_CLASS( ConstDigitalInput, std::string, unsigned int ), public DigitalInput
// class ConstDigitalInput : public DigitalInput
{
public:
    ConstDigitalInput( const std::string& _device, unsigned int _bitPosition );
    virtual bool Read() const;
    virtual ~ConstDigitalInput();
private:
    const std::string device;
    const unsigned int bitPosition;
};

#endif
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

#include "rectangle.h"

WALLAROO_REGISTER( Rectangle, unsigned int, unsigned int );

Rectangle::Rectangle( unsigned int _width, unsigned int _height ) :
    width( _width ),
    height( _height ),
    xPos( 0 ),
    yPos( 0 )
{
}

Box Rectangle::BoundingBox() const
{
    return Box( xPos, yPos, xPos + width, yPos + height );
}

void Rectangle::Draw( cxx0x::shared_ptr< Canvas > canvas ) const
{
    for ( unsigned int x = xPos; x <= xPos + width; ++x )
    {
        canvas -> Point( x, yPos );
        canvas -> Point( x, yPos + height );
    }
    for ( unsigned int y = yPos; y <= yPos + height; ++y )
    {
        canvas -> Point( xPos, y );
        canvas -> Point( xPos + width, y );
    }
}

void Rectangle::MoveX( int delta )
{
    xPos += delta;
}

void Rectangle::MoveY( int delta )
{
    yPos += delta;
}

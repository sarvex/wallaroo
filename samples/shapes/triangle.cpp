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

#include "triangle.h"

WALLAROO_REGISTER( Triangle, unsigned int, unsigned int );

Triangle::Triangle( unsigned int _base, unsigned int _height ) :
    base( _base ),
    height( _height ),
    xPos( 0 ),
    yPos( 0 )
{
}

Box Triangle::BoundingBox() const
{
    return Box( xPos - ( base/2 ), yPos, xPos + ( base / 2 ), yPos + height );
}

void Triangle::Draw( cxx0x::shared_ptr< Canvas > canvas ) const
{
    // draw the base:
    const unsigned int halfBase = base / 2;
    for ( size_t deltaX = 0; deltaX < base; ++deltaX )
        canvas -> Point( xPos - halfBase + deltaX, yPos );

    // draw the sides
    if ( halfBase >= height )
    {
        for ( size_t deltaX = 0; deltaX < halfBase; ++deltaX )
        {
            int x1 = xPos - halfBase + deltaX;
            int x2 = xPos + halfBase - deltaX;
            int y = yPos + static_cast< int >( floor( 0.5 + ( 2.0 * (double)height / (double)base ) * deltaX ) );
            canvas -> Point( x1, y );
            canvas -> Point( x2, y );
        }
    }
    else
    {
        for ( size_t deltaY = 0; deltaY < height; ++deltaY )
        {
            int deltaX = static_cast< int >( floor( 0.5 + deltaY * (double)base / (double)height / 2.0 ) );
            int x1 = xPos - halfBase + deltaX;
            int x2 = xPos + halfBase - deltaX;
            int y = yPos + deltaY;
            canvas -> Point( x1, y );
            canvas -> Point( x2, y );
        }
    }
}

void Triangle::MoveX( int delta )
{
    xPos += delta;
}

void Triangle::MoveY( int delta )
{
    yPos += delta;
}

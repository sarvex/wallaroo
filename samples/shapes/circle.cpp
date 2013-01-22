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

#include <cmath>
#include "circle.h"

WALLAROO_REGISTER( Circle, unsigned int );

Circle::Circle( unsigned int _radius ) :
    radius( _radius ),
    xCenter( 0 ),
    yCenter( 0 )
{
}

Box Circle::BoundingBox() const
{
    return Box( xCenter-radius, yCenter-radius, xCenter+radius, yCenter+radius );
}

void Circle::Draw( cxx0x::shared_ptr< Canvas > canvas ) const
{
    using namespace std;

    static const unsigned int increment = 5;

    for ( unsigned int angle = 0; angle < 360; angle += increment )
    {
        const double rad = static_cast< double >( angle * 6.28 / 360.0 );
        unsigned int x = static_cast< unsigned int >( floor( 0.5 + xCenter + radius * sin( rad ) ) );
        unsigned int y = static_cast< unsigned int >( floor( 0.5 + yCenter + radius * cos( rad ) ) );
        canvas -> Point( x, y );
    }
}

void Circle::MoveX( int delta )
{
    xCenter += delta;
}

void Circle::MoveY( int delta )
{
    yCenter += delta;
}

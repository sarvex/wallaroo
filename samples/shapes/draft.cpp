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

#include "draft.h"

WALLAROO_REGISTER( Draft );

Draft::Draft() :
    canvas( "canvas", RegistrationToken() ),
    shapes( "shapes", RegistrationToken() )
{
}

void Draft::Align()
{
    static const unsigned int spacing = 2;
    
    unsigned int currentX = 0;

    for ( Plug< Shape, collection >::iterator i = shapes.begin(); i != shapes.end(); ++i )
    {
        cxx0x::shared_ptr< Shape > s = i -> lock();
        assert( s );
        Box box = s -> BoundingBox();

        s -> MoveX( currentX - box.x_min );
        currentX += ( box.Width() + spacing );

        s -> MoveY( 1 - box.y_min );
    }
}

void Draft::Draw()
{
    for ( Plug< Shape, collection >::iterator i = shapes.begin(); i != shapes.end(); ++i )
    {
        cxx0x::shared_ptr< Shape > s = i -> lock();
        assert( s );
        s -> Draw( canvas );
    }

    canvas -> Show();
}


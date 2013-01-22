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

#ifndef BOX_H_
#define BOX_H_

struct Box
{
    Box( int x1, int y1, int x2, int y2 ) : 
        x_min( std::min( x1, x2 ) ),
        x_max( std::max( x1, x2 ) ),
        y_min( std::min( y1, y2 ) ),
        y_max( std::max( y1, y2 ) )
    {}
    unsigned int Width() const { return x_max - x_min; }
    unsigned int Height() const { return y_max - y_min; }

    int x_min;
    int y_min;
    int x_max;
    int y_max;
};

#endif

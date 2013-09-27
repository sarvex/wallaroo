################################################################################
# wallaroo - A library for configurable creation and wiring of C++ classes
# Copyright (C) 2012 Daniele Pallastrelli 
# 
#
# This file is part of wallaroo.
# For more information, see http://wallaroo.googlecode.com/
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
################################################################################

CC = g++
CFLAGS = -Wall -I../.. $(UCFLAGS)
LFLAGS = $(ULFLAGS) -ldl
DEPS = consoleplayer.h bet.h currency.h wheel.h bin.h game.h player.h
OBJ = bin.o main.o consoleplayer.o wheel.o game.o

EXE = roulette
PLUGINS = oddevenbet.so redblackbet.so straightbet.so

all: $(EXE) $(PLUGINS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# NOTE you need to pass -E to the linker to provide RTTI across process boundaries!
$(EXE): $(OBJ)
	$(CC) -Wl,-E -o $@ $^ $(CFLAGS) $(LFLAGS)

# NOTE you need to pass -E to the linker to provide RTTI across process boundaries!
%.so: %.cpp %consolefactory.cpp bin.cpp %.h %consolefactory.h
	$(CC) -fPIC -shared -Wl,-E -Wl,-soname,$@ -o $@ $^ $(LFLAGS) $(CFLAGS)

clean:
	rm -f *.o *~ core $(EXE) *.so

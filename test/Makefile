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

# usage:
# make UCFLAGS=-I<BOOST_DIR> ULFLAGS="-L<BOOST_DIR>/stage/lib -lboost_unit_test_framework"
# or
# make UCFLAGS="-std=c++0x -I<BOOST_DIR>" ULFLAGS="-L<BOOST_DIR>/stage/lib -lboost_unit_test_framework"

CC = g++
CFLAGS = -Wall -I.. -DBOOST_TEST_DYN_LINK $(UCFLAGS)
LFLAGS = $(ULFLAGS) -ldl
OBJ = test_cfg_file.o test_multiplicitycheck.o test_wiring.o test_creation.o test_dynamiclib.o driver.o
OBJ_PLUGIN = plugin.o
EXE = test_suite
PLUGIN = plugin.so

all: $(EXE) $(PLUGIN) test

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -Wl,-E -o $@ $^ $(LFLAGS)

$(PLUGIN): $(OBJ_PLUGIN)
	$(CC) -fPIC -shared -Wl,-E -Wl,-soname,$@ -o $@ $^ $(LFLAGS)

test:
	./$(EXE)

clean:
	rm -f *.o *~ core $(EXE) *.so

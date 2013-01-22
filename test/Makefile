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
# make UCFLAGS=-I/mnt/hd/libs/boost_1_51_0 ULFLAGS="-L/mnt/hd/libs/boost_1_51_0/stage/lib -lboost_unit_test_framework-gcc44-mt-1_51"
# or
# make UCFLAGS="-std=c++0x -I/mnt/hd/libs/boost_1_51_0" ULFLAGS="-L/mnt/hd/libs/boost_1_51_0/stage/lib -lboost_unit_test_framework-gcc44-mt-1_51"

CC = g++
CFLAGS = -Wall -I.. -DBOOST_TEST_DYN_LINK $(UCFLAGS)
LFLAGS = $(ULFLAGS)
OBJ = test_cfg_file.o test_xml_loader.o test_multiplicitycheck.o test_wiring.o test_creation.o driver.o
EXE = test_suite

all: $(EXE) test

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) $(LFLAGS) -o $@ $^

test:
	./$(EXE)

clean:
	rm -f *.o *~ core $(EXE)

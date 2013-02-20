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
DEPS = alarm.h digitaloutput.h mineplant.h sumppump.h twolevelsumpprobe.h analoginput.h gasalarm.h pumpengine.h tablebasedanaloginput.h console.h gassensor.h quantitytable.h tablebaseddigitalinput.h continuoussumpprobe.h levelprobe.h safeengine.h tablebaseddigitaloutput.h digitalinput.h levelsensor.h sumpprobe.h thresholdgassensor.h
OBJ = alarm.o gasalarm.o mineplant.o sumppump.o tablebaseddigitaloutput.o console.o levelsensor.o pumpengine.o tablebasedanaloginput.o thresholdgassensor.o continuoussumpprobe.o main.o safeengine.o tablebaseddigitalinput.o twolevelsumpprobe.o

EXE = mineplant

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *~ core $(EXE)

#
# Makefile for the Heating Control host software that
# runs on the Raspberry Pi.
#
# Adrian Bowyer
# 29 July 2016
#

# Where are we?
HEATING = .

# Name of the C++ compiler
CC = g++

# Flags to send the compiler
FL = -pedantic

# Debug or optimize (-pg, -g or -O)
DEBUG = -pg

# Where is libserial
SERIALDIR = $(HEATING)/libserial-0.6.0rc1/src

# Object, Runnable, Include, source and serial library directories
ODIR = $(HEATING)/obj
RDIR = $(HEATING)/bin
IDIR = $(HEATING)/include
SDIR = $(HEATING)/src
LDIR = $(SERIAL)

# Libserial objects
SERIAL = \
	$(SERIALDIR)/PosixSignalDispatcher.o \
	$(SERIALDIR)/SerialPort.o \
	$(SERIALDIR)/SerialStream.o \
	$(SERIALDIR)/SerialStreamBuf.o

# Arguments to send to the compiler
FLAGS = $(DEBUG) $(FL) -I$(IDIR) -I$(SERIALDIR)

# The include files (not the libserial ones)
INCLUDE = \
		$(IDIR)/Device.h \
		$(IDIR)/Heating.h \
		$(IDIR)/HeatProfile.h \
		$(IDIR)/Line.h \
		$(IDIR)/Wireless.h

# The object files
OBJECTS = \
		$(ODIR)/Device.o \
		$(ODIR)/Heating.o \
		$(ODIR)/HeatProfile.o \
		$(ODIR)/Line.o \
		$(ODIR)/Wireless.o

# Targets....

all:	$(OBJECTS)
	$(CC) -o $(RDIR)/Heating $(OBJECTS) $(SERIAL)

clean:
		rm -rf $(ODIR)/*; rm -rf $(RDIR)/*

$(ODIR)/Device.o:	$(SDIR)/Device.cpp $(INCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/Device.o $(SDIR)/Device.cpp

$(ODIR)/Heating.o:	$(SDIR)/Heating.cpp $(INCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/Heating.o $(SDIR)/Heating.cpp

$(ODIR)/HeatProfile.o:	$(SDIR)/HeatProfile.cpp $(INCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/HeatProfile.o $(SDIR)/HeatProfile.cpp

$(ODIR)/Line.o:	$(SDIR)/Line.cpp $(INCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/Line.o $(SDIR)/Line.cpp

$(ODIR)/Wireless.o:	$(SDIR)/Wireless.cpp $(INCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/Wireless.o $(SDIR)/Wireless.cpp







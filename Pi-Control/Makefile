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

# Arguments to send to the compiler
FLAGS = $(DEBUG) $(FL) -I$(IDIR) -I$(SERIALDIR)

# The include files (not the libserial ones)
INCLUDE = \
		$(IDIR)/Device.h \
		$(IDIR)/Heating.h \
		$(IDIR)/HeatProfile.h \
		$(IDIR)/Line.h \
		$(IDIR)/Wireless.h

SINCLUDE = \
		$(SERIALDIR)/PosixSignalDispatcher.h \
		$(SERIALDIR)/PosixSignalHandler.h \
		$(SERIALDIR)/SerialPort.h \
		$(SERIALDIR)/SerialStream.h \
		$(SERIALDIR)/SerialStreamBuf.h

# The object files
OBJECTS = \
		$(ODIR)/Device.o \
		$(ODIR)/Heating.o \
		$(ODIR)/HeatProfile.o \
		$(ODIR)/Line.o \
		$(ODIR)/Wireless.o \
		$(SERIALDIR)/PosixSignalDispatcher.o \
		$(SERIALDIR)/SerialPort.o \
		$(SERIALDIR)/SerialStream.o \
		$(SERIALDIR)/SerialStreamBuf.o

# Targets....

all:	$(OBJECTS)
	$(CC) -o $(RDIR)/Heating $(OBJECTS)

clean:
		rm -rf $(ODIR)/*; rm -rf $(RDIR)/*; rm -rf $(SERIALDIR)/*.o

$(ODIR)/Device.o:	$(SDIR)/Device.cpp $(INCLUDE) $(SINCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/Device.o $(SDIR)/Device.cpp

$(ODIR)/Heating.o:	$(SDIR)/Heating.cpp $(INCLUDE) $(SINCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/Heating.o $(SDIR)/Heating.cpp

$(ODIR)/HeatProfile.o:	$(SDIR)/HeatProfile.cpp $(INCLUDE) $(SINCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/HeatProfile.o $(SDIR)/HeatProfile.cpp

$(ODIR)/Line.o:	$(SDIR)/Line.cpp $(INCLUDE) $(SINCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/Line.o $(SDIR)/Line.cpp

$(ODIR)/Wireless.o:	$(SDIR)/Wireless.cpp $(INCLUDE) $(SINCLUDE)
		$(CC) -c $(FLAGS) -o $(ODIR)/Wireless.o $(SDIR)/Wireless.cpp

$(SERIALDIR)/SerialPort.o:	$(SERIALDIR)/SerialPort.cpp $(SINCLUDE)
	$(CC) -I$(SERIALDIR) -c $(SERIALDIR)/SerialPort.cpp -o $(SERIALDIR)/SerialPort.o

$(SERIALDIR)/PosixSignalDispatcher.o:	$(SERIALDIR)/PosixSignalDispatcher.cpp $(SINCLUDE)
	$(CC) -I$(SERIALDIR) -c $(SERIALDIR)/PosixSignalDispatcher.cpp -o $(SERIALDIR)/PosixSignalDispatcher.o

$(SERIALDIR)/SerialStream.o:	$(SERIALDIR)/SerialStream.cc $(SINCLUDE)
	$(CC) -I$(SERIALDIR) -c $(SERIALDIR)/SerialStream.cc -o $(SERIALDIR)/SerialStream.o

$(SERIALDIR)/SerialStreamBuf.o:	$(SERIALDIR)/SerialStreamBuf.cc $(SINCLUDE)
	$(CC) -I$(SERIALDIR) -c $(SERIALDIR)/SerialStreamBuf.cc -o $(SERIALDIR)/SerialStreamBuf.o







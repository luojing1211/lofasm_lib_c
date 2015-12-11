SOURCES = $(wildcard *.cpp)
EXECUTABLE = $(patsubst %.cpp,%,$(SOURCES))
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
CC=gcc
CXX=g++
RM=rm -f
IDIR1 = ../include
IDIR2 = ../../lofasm_data_lib/include
CPPFLAGS=-g -I$(IDIR1) -I$(IDIR2)
LDFLAGS=-g
LDLIBS = ../lib/lib_de_dispersion.a ../../lofasm_data_lib/lib/lib_lofasm_data.a

all:$(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE) *~

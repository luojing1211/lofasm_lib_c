PROJECT = lofasm_data_lib.a
CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Iinclude -Wall
SOURCES= $(wildcard src/*.cpp)
OBJECTS = $(addprefix lib/,$(notdir $(SOURCES:.cpp=.o)))
OBJDIR = lib
all:$(PROJECT)


$(OBJECTS): $(SOURCES)
	$(CXX) $(CPPFLAGS) -c -o $@ $(@:lib/%.o=src/%.cpp)


$(PROJECT): $(OBJECTS)
	libtool -o $(OBJDIR)/$(PROJECT) -static $(OBJECTS)

clean:
	$(RM) $(OBJECTS) $(OBJDIR)/$(PROJECT)

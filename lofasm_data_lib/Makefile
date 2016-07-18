PROJECT = lib_lofasm_data.a
CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Iinclude
SOURCES= $(wildcard src/*.cpp)
OBJECTS = $(addprefix lib/,$(notdir $(SOURCES:.cpp=.o)))
OBJDIR = lib
all:$(PROJECT)


$(OBJECTS): $(SOURCES)
	$(CXX) $(CPPFLAGS) -c -o $@ $(@:lib/%.o=src/%.cpp)


$(PROJECT): $(OBJECTS)
	ar rcs $(OBJDIR)/$(PROJECT) $(OBJECTS)

clean:
	$(RM) $(OBJECTS) $(OBJDIR)/$(PROJECT)

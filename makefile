CC      = g++
LDLIBS  = -lglfw -lglad -lGL 
LDFLAGS = -ggdb -Wall -pedantic
CFLAGS  = -I./src

SRCDIR  = src
SRCEXT  = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

OBJECTS= $(SOURCES:.cpp=.o)
EXECUTABLE=build/metaballs


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)


.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)


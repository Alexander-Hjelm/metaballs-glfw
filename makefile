CC      = g++

# Link OpenGL library / framework depending on the system
libgl_Linux = -lGL
libgl_Darwin = -framework OpenGL

LDLIBS  = -lglfw -lglad $(libgl_$(shell uname -s))
LDFLAGS = -ggdb -Wall -pedantic
CFLAGS  = -I./src
INC = -I./include

SRCDIR  = src
SRCEXT  = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

OBJECTS = $(SOURCES:.cpp=.o)
BUILDIR = build/
EXECUTABLE = build/metaballs

# Create build directory if not exist
$(shell mkdir -p $(BUILDIR))

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)


.cpp.o:
	$(CC) $(INC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)


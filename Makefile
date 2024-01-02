CC := g++

SOURCEDIR := ./src
SOURCEINCLUDEDIR := $(SOURCEDIR)
INCLUDEDIR := ./include
LIBDIR := ./lib

EXECUTABLE := Spelunker.out

LFLAGS := -L$(LIBDIR)
LDFLAGS := -lsfml-graphics -lsfml-network -lsfml-window -lsfml-audio -lsfml-system -ljsoncpp

SOURCES := $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

%.o: %.cpp
	$(CC) -c -Wall -o $@ $^ -I$(INCLUDEDIR) -I$(SOURCEINCLUDEDIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS) $(LDFLAGS)

all: $(EXECUTABLE)

clean:
	rm -rf $(SOURCEDIR)/*.o

phony: clean

run: all
	./$(EXECUTABLE)
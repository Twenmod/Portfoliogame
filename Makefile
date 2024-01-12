CC := g++
SOURCEDIR := ./src
SOURCEINCLUDEDIR := $(SOURCEDIR)
INCLUDEDIR := ./include
LIBDIR := ./lib

EXECUTABLE := Spelunker.out

CFLAGS := -Wall -Wpedantic -g -std=c++17
LFLAGS := -L$(LIBDIR)
LDFLAGS := -lsfml-graphics -lsfml-network -lsfml-window -lsfml-audio -lsfml-system -ljsoncpp -Wall

SOURCES := $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $^ -I$(INCLUDEDIR) -I$(SOURCEINCLUDEDIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS) $(LDFLAGS) $(CFLAGS)

all: $(EXECUTABLE)

clean:
	rm -rf $(SOURCEDIR)/*.o

phony: clean

run: all
	./$(EXECUTABLE)
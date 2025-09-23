CXXFLAGS=-Wall -ggdb -std=c++11 -pedantic $(shell pkg-config --cflags sdl2 sdl2_ttf sdl2_image)
LIBS=$(shell pkg-config --libs sdl2 sdl2_ttf sdl2_image)

main: main.cc
	$(CXX) $(CXXFLAGS) -o main main.cc movement.cc growth.cc snake.cc positions.cc globals.cpp $(LIBS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
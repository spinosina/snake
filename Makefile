CXXFLAGS=-Wall -ggdb -std=c++11 -pedantic $(shell pkg-config --cflags sdl2 sdl2_ttf)
LIBS=$(shell pkg-config --libs sdl2 sdl2_ttf)

main: snake.cc
	$(CXX) $(CXXFLAGS) -o main snake.cc movement.cc growth.cc $(LIBS)

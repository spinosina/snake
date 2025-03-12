CXXFLAGS=-Wall -ggdb -std=c++11 -pedantic $(shell pkg-config --cflags sdl2 sdl2_ttf sdl2_image)
LIBS=$(shell pkg-config --libs sdl2 sdl2_ttf sdl2_image)

main: snake.cc
	$(CXX) $(CXXFLAGS) -o main snake.cc movement_temp.cc growth_temp.cc Snake_temp.cc Positions_temp.cc $(LIBS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <SDL.h>

struct pos {
    float x;
    float y;
};

struct posChanged {
    pos posChanged;
    std::string directionChanged;
};

int nextMoveIsRect(SDL_FRect rect, SDL_FRect food, SDL_FRect obstacle, std::string direction);
pos getNewCoordinates(void);
void onButtonMove(std::string direction);
void removeUselessPos();
void checkIfOutOfWindow(struct body body);

#endif


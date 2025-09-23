#include <string>
#include <SDL.h>
#include "positions.h"
#include "globals.h"


// questa classe serve a tener traccia delle posizioni di snake

Position::Position(float x, float y, std::string direction) {
    this->x = x;
    this->y = y;
    this->direction = direction;
}

float Position::getX() {
    return this->x;
}

float Position::getY() {
    return this->y;
}

std::string Position::getDirection() {
    return this->direction;
}

void Position::setX(float x) {
    this->x = x;
    return;
}

void Position::setY(float y) {
    this->y = y;
    return;
}

void Position::setDirection(std::string direction) {
    this->direction = direction;
    return;
}

Position Position::getNewCoordinatesForFood() {
    int x=-1; int y=-1;
    
    while ((x%L != 0) == true || (x == DIM_H) == true || (x == pivot.rect.x) == true 
            || (x == obstacle.getRectAltDX().getX()) || (x == obstacle.getRectAltSX().getX())
            || (x == obstacle.getRectDwnDX().getX()) || (x == obstacle.getRectDwnSX().getX())) {
        x = 0 + (rand() % (DIM_H -  0 + 1));
    }

    while ((y%L != 0)==true || (y == DIM_H) == true || (y == pivot.rect.y) == true
            || (y == obstacle.getRectAltDX().getY()) || (y == obstacle.getRectAltSX().getY())
            || (y == obstacle.getRectDwnDX().getY()) || (y == obstacle.getRectDwnSX().getY())) {
        y = 0 + (rand() % (DIM_H - 0 + 1));
    }

    float xF = (float)x;
    float yF = (float)y;

    //printf("%snuove coordinate calcolate per food %f, %f%s\n", YELLOW, xF, yF, RESET);

    return Position(xF, yF, "");
}

Position Position::getNewCoordinates() {
    int x=-1; int y=-1;
    
    while ((x%L != 0) == true || (x == DIM_H) == true || (x == pivot.rect.x) == true || (x == food.getX() == true)) {
        x = 0 + (rand() % (DIM_H -  0 + 1));
    }

    while ((y%L != 0)==true || (y == DIM_H) == true || (y == pivot.rect.y) == true || (y == food.getY() == true)) {
        y = 0 + (rand() % (DIM_H - 0 + 1));
    }

    float xF = (float)x;
    float yF = (float)y;

    //printf("%snuove coordinate calcolate per ostacolo %f, %f%s\n", RED, xF, yF, RESET);

    return Position(xF, yF, "");
}
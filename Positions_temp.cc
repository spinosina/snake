#include <string>
#include <SDL.h>
#include "Positions_temp.h"
#include "globals_temp.h"


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
    
    while ((x%L != 0) == true || (x == DIM_H) == true || (x == pivot.rect.x) == true) {
        x = 0 + (rand() % (DIM_H -  0 + 1));
    }

    while ((y%L != 0)==true || (y == DIM_H) == true || (y == pivot.rect.y) == true) {
        y = 0 + (rand() % (DIM_H - 0 + 1));
    }

    float xF = (float)x;
    float yF = (float)y;

    printf("\n\nnuove coordinate calcolate %f, %f\n", xF, yF);

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

    //printf("\n\nnuove coordinate calcolate %f, %f\n", xF, yF);

    return Position(xF, yF, "");
}
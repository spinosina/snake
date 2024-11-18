#include <string>
#include "Snake_temp.h"
#include <SDL.h>

// classe per definire un quadrato singolo che può essere una parte del corpo di snake
// oppure parte di un ostacolo, o ancora il cibo

Square::Square(SDL_FRect rect) {
    this->rect = rect;
}
Square::Square(float x, float y, int w, int h) {
    this->rect.x = x; this->rect.y = y;
    this->rect.w = w; this->rect.h = h;
}

float Square::getX() {
    return this->rect.x;
}

float Square::getY() {
    return this->rect.y;
}

float Square::getW() {
    return this->rect.w;
}

float Square::getH() {
    return this->rect.h;
}

void Square::setX(float x) {
    this->rect.x = x;
}

void Square::setY(float y) {
    this->rect.y = y;
}


// body rappresenta un quadrato orientato: cioè uno Square che ha una direzione
// per cui si tiene traccia di ogni elemento di snake

Body::Body(float x, float y, int w, int h, std::string direction) : Square(x, y, w, h) { // richiamo il costruttore di square
    this->direction = direction;
}

std::string Body::getDirection() {
    return this->direction;
}

void Body::setDirection(std::string direction) {
    this->direction = direction;
}

void Body::setX(float x) {
    this->rect.x = x;
}

void Body::setY(float y) {
    this->rect.y = y;
}

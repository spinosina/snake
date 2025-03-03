#include <string>
#include "Snake_temp.h"
#include "Positions_temp.h"
#include <SDL.h>

#define L 20
#define DIM_H 600 //componente orizzontale della risoluzione
#define DIM_V 600 //componente verticale della risoluzione

// classe per definire un quadrato singolo che può essere una parte del corpo di snake
// oppure parte di un ostacolo, o ancora il cibo
Square::Square() {
    this->rect = {0, 0, 0, 0};
}

Square::Square(SDL_FRect rect) {
    this->rect = rect;
}
Square::Square(float x, float y, int w, int h) {
    this->rect.x = x; this->rect.y = y;
    this->rect.w = w; this->rect.h = h;
}

void Square::updatePosForFood() {
    Position posForFood = Position(0, 0, "");
    posForFood = posForFood.getNewCoordinates();
    this->setX(posForFood.x);
    this->setY(posForFood.y);
}

void Square::updatePos() {
    Position pos = Position(0, 0, "");
    pos = pos.getNewCoordinates();
    this->setX(pos.x);
    this->setY(pos.y);
}

float Square::getX() {
    return this->rect.x;
}

float Square::getY() {
    return this->rect.y;
}

int Square::getW() {
    return this->rect.w;
}

int Square::getH() {
    return this->rect.h;
}

void Square::setX(float x) {
    this->rect.x = x;
}

void Square::setY(float y) {
    this->rect.y = y;
}

// pivot rappresenta il primo elemento del vectoBody, lo definiamo a parte cosi da poter
// usare i vantaggi della variabile atomic
Pivot::Pivot(float x, float y, int w, int h, int direction) : Square(x, y, w, h) { // richiamo il costruttore di square
    this->direction.store(direction, std::memory_order_relaxed);
}

int Pivot::getDirection() {
    return this->direction.load(std::memory_order_relaxed);
}

void Pivot::setDirection(int direction) {
    this->direction.store(direction, std::memory_order_relaxed);
}

void Pivot::setX(float x) {
    this->rect.x = x;
}

void Pivot::setY(float y) {
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


Obstacle::Obstacle() { // richiamo il costruttore di square

    // inizializza tutti gli Square partendo da quello in alto a SX che viene generato casualmente
    this->rectAltSX = Square(0, 0, L, L);
    this->rectAltSX.updatePos();
    this->rectAltDX = Square(this->rectAltSX.getX(), this->rectAltSX.getY()+L, L, L);
    this->rectDownSX = Square(this->rectAltSX.getX()+L, this->rectAltSX.getY(), L, L);
    this->rectDownDX = Square(this->rectAltSX.getX()+L, this->rectAltSX.getY()+L, L, L);
}

void Obstacle::setCoordinates() {
    //printf("setCoordinates() quadrato alto sinistra: %f,%f\n", this->rectAltSX.getX(), this->rectAltSX.getY());

    // coordinate del quadrato in alto a destra
    this->rectAltDX.setX(this->rectAltSX.getX()+L);
    this->rectAltDX.setY(this->rectAltSX.getY());

    // coordinate del quadrato in basso a sinistra
    this->rectDownSX.setX(this->rectAltSX.getX());
    this->rectDownSX.setY(this->rectAltSX.getY()+L);

    //coordinate del quadrato in basso a destra
    this->rectDownDX.setX(this->rectAltSX.getX()+L);
    this->rectDownDX.setY(this->rectAltSX.getY()+L);
}

Square Obstacle::getRectAltSX() {
    return this->rectAltSX;
}
Square Obstacle::getRectAltDX() {
    return this->rectAltDX;
}
Square Obstacle::getRectDwnSX() {
    return this->rectDownSX;
}
Square Obstacle::getRectDwnDX() {
    return this->rectDownDX;
}

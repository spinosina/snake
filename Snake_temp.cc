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

void Square::updatePos() {
    Position posForFood = Position(0, 0, "");
    posForFood = posForFood.getNewCoordinates();
    this->setX(posForFood.x);
    this->setY(posForFood.y);
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
    // inizializza tutti a rect per poi settare le coordinate con l'apposita funzione
    Position posForObstacle = Position(0, 0, "");
    posForObstacle = posForObstacle.getNewCoordinates(); 

    // controllo se il quadratone esce dalla finestra
    int x = 0, y = 0;
    while ( ((x%L != 0) || (x == DIM_H)) && (((x+20)%L != 0) || ((x+20) == DIM_H)) 
        && ((y%L != 0) || (y == DIM_H)) && (((y+20)%L != 0) || ((y+20) == DIM_V))) {
            posForObstacle = posForObstacle.getNewCoordinates();
    }

    this->rectAltSX = Square(posForObstacle.getX(), posForObstacle.getY(), L, L);
    this->rectAltDX = Square(this->rectAltSX.getX(), this->rectAltSX.getY()+L, L, L);
    this->rectDownSX = Square(this->rectAltSX.getX()+L, this->rectAltSX.getY(), L, L);
    this->rectDownDX = Square(this->rectAltSX.getX()+L, this->rectAltSX.getY()+L, L, L);
}

void Obstacle::setCoordinates() {
    // coordinate del quadrato in alto a destra
    this->rectAltDX.setX(this->rectAltSX.getX()+1);

    // coordinate del quadrato in basso a sinistra
    this->rectDownSX.setY(this->rectAltSX.getY()-1);

    //coordinate del quadrato in basso a destra
    this->rectDownDX.setX(this->rectAltSX.getX()+1);
    this->rectDownDX.setY(this->rectAltSX.getY()-1);
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

#ifndef SNAKE_H
#define SNAKE_H

#include <SDL.h>

// classe per definire un quadrato singolo che può essere una parte del corpo di snake
// oppure parte di un ostacolo, o ancora il cibo
class Square {
public:
    SDL_FRect rect;
    Square(SDL_FRect rect);
    Square(float x, float y, int w, int h);
    float getX();
    float getY();
    float getW();
    float getH();
    void setX(float x);
    void setY(float y);
};

// body rappresenta un quadrato orientato: cioè uno Square che ha una direzione
// per cui si tiene traccia di ogni elemento di snake
class Body : public Square {
public:
    std::string direction;
    Body(float x, float y, int w, int h, std::string direction);
    std::string getDirection();
    void setDirection(std::string direction);
    void setX(float x);
    void setY(float y);
};

#endif
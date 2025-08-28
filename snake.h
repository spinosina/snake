#ifndef SNAKE_H
#define SNAKE_H

#include <SDL.h>
#include <atomic>

// classe per definire un quadrato singolo che può essere una parte del corpo di snake
// oppure parte di un ostacolo, o ancora il cibo
class Square {
public:
    SDL_FRect rect;
    Square();
    Square(SDL_FRect rect);
    Square(float x, float y, int w, int h);
    void updatePosForFood();
    void updatePos();
    float getX();
    float getY();
    int getW();
    int getH();
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
};

class Pivot : public Square {
    public:
        // -1 == nessuna direzione
        // 1 == up, 2 == right, 3 == down, 4 == left
        std::atomic<int> direction;
        Pivot(float x, float y, int w, int h, int direction);
        int getDirection();
        void setDirection(int direction);
    };

// obstacle rappresenta un insieme di 4 rect a partire da square, le cui coordinate sono
// posizionate in maniera tale da formare un unico grande quadrato
class Obstacle : public Square {
public:
    Square rectAltSX;
    Square rectAltDX;
    Square rectDownSX;
    Square rectDownDX;
    Obstacle();
    void setCoordinates();
    Square getRectAltSX();
    Square getRectAltDX();
    Square getRectDwnSX();
    Square getRectDwnDX();
};
#endif
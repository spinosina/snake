#ifndef POSITIONS_H
#define POSITIONS_H

#include <SDL.h>

class Position {
public: 
    float x;
    float y;
    std::string direction;
    Position(float x, float y, std::string direction);
    float getX();
    float getY();
    std::string getDirection();
    void setX(float x);
    void setY(float y);
    void setDirection(std::string direction);
    Position getNewCoordinates();
};
#endif
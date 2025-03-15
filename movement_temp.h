#ifndef MOVEMENT_TEMP_H
#define MOVEMENT_TEMP_H

#include <string> 
#include "Snake_temp.h"

int nextMoveIsRect(Square rect, Square food, Obstacle obstacle, std::string direction);
int nextMoveIsSnake(Square rect);
std::string findInVectPos(Square rect);
void checkIfOutOfWindow(int i);
void moveSnake();
void onButtonMove(std::string direction);
void removeUselessPos();
void moveObstacle(Obstacle& obstacle);
#endif

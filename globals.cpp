#include <vector>
#include <atomic>

#include "globals.h"

std::string getDirectionFromPivot(int directionNum)
{
    switch (directionNum)
    {
    case 1:
        return "SDLK_UP";
        break;
    case 2:
        return "SDLK_RIGHT";
        break;
    case 3:
        return "SDLK_DOWN";
        break;
    case 4:
        return "SDLK_LEFT";
        break;
    }
    return "";
}

int getPivotFromDirection(std::string direction)
{
    if (strcmp(direction.c_str(), "SDLK_UP")==0)
        return 1;
    else if (strcmp(direction.c_str(), "SDLK_RIGHT")==0)
        return 2;
    else if (strcmp(direction.c_str(), "SDLK_DOWN")==0)
        return 3;
    else if (strcmp(direction.c_str(), "SDLK_LEFT")==0)
        return 4;

    return -1;
}

std::string getPivotNewSkinAfterCollision(int directionNum) {
    switch (directionNum)
    {
    case 1:
        pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaSu.png";
        break;
    case 2:
        pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaDx.png";
        break;
    case 3:
        pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaGiu.png";
        break;
    case 4:
        pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaSx.png";
        break;
    }

    return "";
}

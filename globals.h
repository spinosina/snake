#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <atomic>

#include "snake.h"
#include "positions.h"

#define L 64
#define DIM_H 640 // componente orizzontale della risoluzione
#define DIM_V 704 // componente verticale della risoluzione

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

extern std::vector<Body> vectorBody;
extern std::vector<Position> vectorPosChanged;
extern std::atomic<bool> endThread;
// extern bool endThread;
extern Square food;
extern Pivot pivot;
extern std::string pivotSkinPath;
extern std::string pivotSkinPath;
extern std::string bodySkinPath;
extern std::string foodSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_2_Oliva.png";
extern std::string obstacleAltSxSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_AltSX.png";
extern std::string obstacleAltDxSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_AltDX.png";
extern std::string obstacleDownSxSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_DwnSX.png";
extern std::string obstacleDownDxSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_DwnDX.png";
extern Obstacle obstacle;
extern int currentScore;

// enumerato per gli stati del gioco
enum class GameState
{
    MENU,
    GAME,
    GAME_OVER,
    SKIN_SELECTION,
    QUIT
};

// recupero la direzione in stringa dal valore della direzione numerica del pivot
// che ha come campo un intero atomico
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
};

// recupero la direzione sottoforma di intero da inserire all'interno del valore atomico del pivot
// in base alla direzione sottoforma di stringa
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
};

// recupero il path da dare al pivot a seconda della direzione che ha
// nel suo valore atomico
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
}

#endif
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
extern std::string bodySkinPath;
extern std::string foodSkinPath;
extern std::string obstacleAltSxSkinPath;
extern std::string obstacleAltDxSkinPath;
extern std::string obstacleDownSxSkinPath;
extern std::string obstacleDownDxSkinPath;
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
extern std::string getDirectionFromPivot(int directionNum);

// recupero la direzione sottoforma di intero da inserire all'interno del valore atomico del pivot
// in base alla direzione sottoforma di stringa
extern int getPivotFromDirection(std::string direction);

// recupero il path da dare al pivot a seconda della direzione che ha
// nel suo valore atomico
extern std::string getPivotNewSkinAfterCollision(int directionNum);

#endif
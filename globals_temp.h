#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>

#include "Snake_temp.h"
#include "Positions_temp.h"

#define L 32
#define DIM_H 608 //componente orizzontale della risoluzione
#define DIM_V 608 //componente verticale della risoluzione

    extern std::vector<Body> vectorBody;
    extern std::vector<Position> vectorPosChanged;
    extern bool endThread;
    extern Square food;
    extern Pivot pivot;
    extern Obstacle obstacle;
    extern int currentScore;
#endif
#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <atomic>

#include "Snake_temp.h"
#include "Positions_temp.h"

#define L 64
#define DIM_H 640 //componente orizzontale della risoluzione
#define DIM_V 704 //componente verticale della risoluzione

    extern std::vector<Body> vectorBody;
    extern std::vector<Position> vectorPosChanged;
    extern std::atomic<bool> endThread;
    //extern bool endThread;
    extern Square food;
    extern Pivot pivot;
    extern Obstacle obstacle;
    extern int currentScore;
#endif
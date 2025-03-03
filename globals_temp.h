#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>

#include "Snake_temp.h"
#include "Positions_temp.h"

    extern std::vector<Body> vectorBody;
    extern std::vector<Position> vectorPosChanged;
    extern bool endThread;
    extern Square food;
    extern Pivot pivot;
    extern int currentScore;
#endif
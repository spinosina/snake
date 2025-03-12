#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "growth_temp.h"
#include "globals_temp.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define L 32

// funzione che crea un nuovo rect (una nuova parte del corpo di snake) 
// passandogli l'ultimo blocco aggiunto al corpo per gestire le coordinate e la direzione
void growing(Body body) {
    Body newBody = Body(body.getX(), body.getY(), body.getW(), body.getH(), body.direction);
    
    if (body.getDirection() == "SDLK_DOWN") 
        newBody.setY(newBody.getY() - L);

    else if (body.getDirection() == "SDLK_UP") 
        newBody.setY(newBody.getY() + L);

    else if (body.getDirection() == "SDLK_RIGHT") 
        newBody.setX(newBody.getX() - L);

    else if (body.getDirection() == "SDLK_LEFT") 
        newBody.setX(newBody.getX() + L);

    vectorBody.push_back(newBody);

    for (int i = 0; i < vectorBody.size(); i++) {
        printf("%sla direzione attuale di %d è %s%s\n", RED, i, vectorBody[i].direction.c_str(), RESET);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <vector>
#include "growth.h"
#include "globals.h"

#include <SDL.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define L 20


// funzione che crea un nuovo rect (una nuova parte del corpo di snake) 
// passandogli l'ultimo blocco aggiunto al corpo per gestire le coordinate e la direzione
void growing(struct body body) {
    float prevX = -1; float prevY = -1;
    std::string prevDirection = "";

    prevX = body.rect.x;
    prevY = body.rect.y;
    prevDirection = body.direction;

    //printf("l'elemento precedente si trovava in: %f,%f in direzione %s\n", prevX, prevY, prevDirection.c_str());
    
    float actualX = prevX; float actualY = prevY;
    std::string actualDirection = prevDirection;

    if (actualDirection == "SDLK_DOWN") 
        actualY -= L;
    else if (actualDirection == "SDLK_UP") 
        actualY += L;
    else if (actualDirection == "SDLK_RIGHT") 
        actualX -= L;
    else if (actualDirection == "SDLK_LEFT") 
        actualX += L;

    //printf("l'elemento attuale si trova in: %f,%f in direzione %s\n", actualX, actualY, actualDirection.c_str());

    SDL_FRect rect1 = {actualX, actualY, L, L};

    struct body body1;
    body1.rect = rect1;
    body1.direction = actualDirection;

    vectorBody.push_back(body1);

    for (int i = 0; i < vectorBody.size(); i++) {
        printf("%sla direzione attuale di %d è %s%s\n", RED, i, vectorBody[i].direction.c_str(), RESET);
    }
    
    return;
}
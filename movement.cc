#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include "movement.h"
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
#define MAX 600
#define MIN 0
#define DIM_H 600 //componente orizzontale della risoluzione
#define DIM_V 600 //componente verticale della risoluzione

struct body {
    SDL_FRect rect;
    std::string direction;
};
 
// questa funzione ritorna 0 se il rect incontra un food, 1 se incontra 
// un obstacle, 2 altrimenti.
// nel primo caso aumenta la sua dimensione, nel secondo fa terminare il loop
// nel terzo non fa nulla: questo rappresenta il caso in cui non ci sono state intersezioni
int nextMoveIsRect(SDL_FRect rect, SDL_FRect food, SDL_FRect obstacle, std::string direction) {
    // se c'è un'intersezione tra rect e food, food deve cambiare posizione e rect deve allungarsi
    int xRect = rect.x; int yRect = rect.y;
    int xFood = food.x; int yFood = food.y;
    int xObst = obstacle.x; int yObst = obstacle.y;

    if (direction == "SDLK_DOWN") 
        yRect += L;
    else if (direction == "SDLK_UP") 
        yRect -= L;
    else if (direction == "SDLK_RIGHT") 
        xRect += L;
    else if (direction == "SDLK_LEFT") 
        xRect -= L;

    if (xRect == xFood && yRect == yFood) {
        onButtonMove(direction);
        return 0;
    }
    else if (xRect == xObst && yRect == yObst) {
        //printf("collisione con obstacle\n");
        return 1;
    }
    else {
        return 2;
    }
}

// questa funzione calcola le prossime coordinate di food, controllando se
// la nuova posizione ha x e y divisibili per L per avere un'intersezione perfetta
// in caso di collisione
pos getNewCoordinates(void) {
    struct pos coord;
    int x = -1; int y = -1;
    while ((x%L != 0) == true || (x == 600) == true) {
        x = MIN + (rand() % (MAX -  MIN + 1));
    }

    while ((y%L != 0)==true || (y == 600) == true) {
        y = MIN + (rand() % (MAX - MIN + 1));
    }

    printf("variabile x randomica: %d\n", x);
    printf("variabile y randomica: %d\n", y);

    coord.x = x;
    coord.y = y;
    return coord;
}

// questa funzione coordina il movimento di snake e di tutte le sue parti del corpo
// gestendone anche le curve
/*void onButtonMove(std::string direction) {
    printf("direzione di pivot: %s, direzione cliccata %s\n", vectorBody[0].direction.c_str(), direction.c_str());
    std::string pivotDirectionBeforeChange = vectorBody[0].direction;
    
    // faccio la curva solo se la direzione che abbiamo cliccato è diversa da quella di pivot
    if (vectorBody[0].direction != direction) {

        // salvo la posizione in cui è stata cambiata direzione
        pos pivotPosition = {vectorBody[0].rect.x, vectorBody[0].rect.y};
        struct posChanged posChanged;
        posChanged.posChanged = pivotPosition;
        posChanged.directionChanged = direction;
        printf("la posizione di curva è: %f,%f in direzione %s\n", pivotPosition.x, pivotPosition.y, direction.c_str());

        vectorPosChanged.push_back(posChanged);

        // ciclo sul vettore delle posizioni
        for (int i = 0; i < vectorPosChanged.size(); i++) {
            bool foundMatch = false;
            //ciclo sul vettore del corpo di snake
            for (int j = 0; j < vectorBody.size(); j++) {
                // confronto le posizioni
                if ((vectorBody[j].rect.x == vectorPosChanged[i].posChanged.x) && (vectorBody[j].rect.y == vectorPosChanged[i].posChanged.y)) {
                    // trovato l'elemento che sta passando per quella posizione: cambio la direzione alla parte del corpo di snake
                    vectorBody[j].direction = vectorPosChanged[i].directionChanged;
                    
                    // tutte le altre direzioni prendono il valore della direzione dell'elemento precedente
                    for (int k = 0; k < vectorBody.size(); k++) {
                        if (k ==0) {
                            printf("%snew direction for elem %d = %s%s\n", YELLOW, k, vectorBody[k].direction.c_str(), RESET);
                        }
                        else if (k ==1) {
                            vectorBody[j].direction = pivotDirectionBeforeChange;
                            printf("%snew direction for elem %d = %s%s\n", YELLOW, k, vectorBody[k].direction.c_str(), RESET);
                        }
                        else {
                            vectorBody[j].direction = vectorBody[j-1].direction;
                            printf("%snew direction for elem %d = %s%s\n", YELLOW, k, vectorBody[k].direction.c_str(), RESET);
                        }

                        if (vectorBody[k].direction == "SDLK_DOWN") {
                            vectorBody[k].rect.y += L;
                        }
                        else if (vectorBody[k].direction == "SDLK_UP") {
                            vectorBody[k].rect.y -= L;
                        }
                        else if (vectorBody[k].direction == "SDLK_RIGHT") {
                            vectorBody[k].rect.x += L;
                        }
                        else if (vectorBody[k].direction == "SDLK_LEFT") {
                            vectorBody[k].rect.x -= L;
                        }                        
                    }
                    foundMatch = true;
                    // non c'è bisogno di continuare a scorrere
                    break;
                } 
                // caso in cui siamo alla fine dello scorrimento del corpo di snake e non abbiamo trovato intersezioni: 
                // elimino l'elemento dal vettore delle posizioni, già ci sono passati tutti
                // inoltre inserisco controllo se l'elemento del vettore posizione che sto scorrendo è il primo 
                else if ((j = vectorBody.size()-1) && (i == 0) && (foundMatch == false)) {
                    vectorPosChanged.erase(vectorPosChanged.begin());
                }
            }
        }
    }

    else {
        //scandisco tutti gli elementi e li muovo
        for (int i = 0; i < vectorBody.size(); i++) {
            // cambio la direzione (il primo elemento prende la direzione del pivot prima di un eventuale change)
            // gli altri elementi prendono la direzione dell'elemento prima di loro
            printf("movement elem %d del vettore ha direzione %s e coordinate %f,%f\n", i, direction.c_str(), vectorBody[i].rect.x, vectorBody[i].rect.y);

            // dopodichè muovo in base alla NUOVA direction
            if (vectorBody[i].direction == "SDLK_DOWN") {
                vectorBody[i].rect.y += L;
            }
            else if (vectorBody[i].direction == "SDLK_UP") {
                vectorBody[i].rect.y -= L;
            }
            else if (vectorBody[i].direction == "SDLK_RIGHT") {
                vectorBody[i].rect.x += L;
            }
            else if (vectorBody[i].direction == "SDLK_LEFT") {
                vectorBody[i].rect.x -= L;
            }
        }
    }
}*/

std::string findInVectPos(SDL_FRect rect) {
    std::string direction = "NotFound";
    for (int i = 0; i < vectorPosChanged.size(); i++) {
        if (vectorPosChanged[i].posChanged.x == rect.x && vectorPosChanged[i].posChanged.y == rect.y) {
            direction = vectorPosChanged[i].directionChanged;
            break;
        }
    }
    return direction;
}

void removeUselessPos() {
    bool found = false;
    printf("%sla size di vectorPosChanged: %zu %s\n", YELLOW, vectorPosChanged.size(), RESET);
    for (int i = 0; i < vectorPosChanged.size(); i++) {
        found = false;
        for (int j = 0; j < vectorBody.size(); j++) {
            if (vectorPosChanged[i].posChanged.x == vectorBody[j].rect.x && vectorPosChanged[i].posChanged.y == vectorBody[j].rect.y) {
                found = true;
            }
        }
        if (found == false) 
            vectorPosChanged.erase(vectorPosChanged.begin());
    }
    printf("%sla size di vectorPosChanged: %zu %s\n", YELLOW, vectorPosChanged.size(), RESET);
    return;
}

// questa funzione riposiziona correttamente ogni parte del corpo di snake quando esce dalla
// window, quindi quando le coordinate sono <0 o >DIM_H (che è la componente di altezza massima)
// o >DIM_V (che è la componente di ampiezza massima)
void checkIfOutOfWindow(int i) {
    if (vectorBody[i].rect.x < 0) {
        vectorBody[i].rect.x = DIM_H;
        printf("la x era < 0, ora è: %f", vectorBody[i].rect.x);
    } else if (vectorBody[i].rect.x == MAX) {
        vectorBody[i].rect.x = 0;
        printf("la x era > MAX, ora è: %f", vectorBody[i].rect.x);
    } else if (vectorBody[i].rect.y < 0) {
        vectorBody[i].rect.y = DIM_V;
        printf("la y era < 0, ora è: %f", vectorBody[i].rect.y);
    } else if (vectorBody[i].rect.y == MAX) {
        vectorBody[i].rect.y = 0;
        printf("la y era > MAX, ora è: %f", vectorBody[i].rect.y);
    }
    return;
}

void onButtonMove(std::string direction) {
    //printf("direzione di pivot: %s, direzione cliccata %s\n", vectorBody[0].direction.c_str(), direction.c_str());
    std::string pivotDirectionBeforeChange = vectorBody[0].direction;

    // aggiungo il punto di svolta al vettore vectorPosChanged
    if (vectorBody[0].direction != direction) {

            // salvo la posizione in cui è stata cambiata direzione
            pos pivotPosition = {vectorBody[0].rect.x, vectorBody[0].rect.y};
            struct posChanged posChanged;
            posChanged.posChanged = pivotPosition;
            posChanged.directionChanged = direction;
            //printf("la posizione di curva è: %f,%f in direzione %s\n", pivotPosition.x, pivotPosition.y, direction.c_str());

            vectorPosChanged.push_back(posChanged);
    }

    // per ognuno dei punti di svolta
    for (int i = vectorBody.size()-1; i >= 0; i--) {
        if (vectorPosChanged.size() != 0) {
            // per ogni elemento cerco se si trova in un punto di svolta
            struct posChanged actualPosBody;
            actualPosBody.posChanged.x = vectorBody[i].rect.x;
            actualPosBody.posChanged.y = vectorBody[i].rect.y;
            actualPosBody.directionChanged = vectorBody[i].direction;

            std::string found = findInVectPos(vectorBody[i].rect);

            if (found != "NotFound"){
                printf("New direction: %s\n", found.c_str());
                vectorBody[i].direction = found;
            }
            else 
                printf("Element not found.\n");
        }

        if (vectorBody[i].direction == "SDLK_DOWN") {
                vectorBody[i].rect.y += L;
        }
        else if (vectorBody[i].direction == "SDLK_UP") {
                vectorBody[i].rect.y -= L;
        }
        else if (vectorBody[i].direction == "SDLK_RIGHT") {
                vectorBody[i].rect.x += L;
        }
        else if (vectorBody[i].direction == "SDLK_LEFT") {
                vectorBody[i].rect.x -= L;
        }
        checkIfOutOfWindow(i);
    }
    return;
}
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <thread>

#include "movement_temp.h"
#include "Snake_temp.h"
#include "Positions_temp.h"
#include "globals_temp.h"
#include "growth_temp.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

#define L 20
#define DIM_H 600 //componente orizzontale della risoluzione
#define DIM_V 600 //componente verticale della risoluzione

// questa funzione ritorna 0 se il rect incontra un food, 1 se incontra 
// un obstacle, 2 altrimenti.
// nel primo caso aumenta la sua dimensione, nel secondo fa terminare il loop
// nel terzo non fa nulla: questo rappresenta il caso in cui non ci sono state intersezioni
int nextMoveIsRect(Square rect, Square food, Obstacle obstacle, std::string direction) {
    // se c'è un'intersezione tra rect e food, food deve cambiare posizione e rect deve allungarsi
    if (direction == "SDLK_DOWN") 
        rect.setY(rect.getY() + L);

    else if (direction == "SDLK_UP") 
        rect.setY(rect.getY() - L);

    else if (direction == "SDLK_RIGHT") 
        rect.setX(rect.getX() + L);

    else if (direction == "SDLK_LEFT") 
        rect.setX(rect.getX() - L);
        
    if (rect.getX() == food.getX() && rect.getY() == food.getY()) {
        printf("%scollision con food %s\n", YELLOW, RESET);
        onButtonMove(direction);
        return 0;
    }

    else if ((rect.getX() == obstacle.rectAltSX.getX() || rect.getX() == obstacle.rectAltDX.getX() || 
                rect.getX() == obstacle.rectDownSX.getX() || rect.getX() == obstacle.rectDownDX.getX())
                && (rect.getY() == obstacle.rectAltSX.getY() || rect.getY() == obstacle.rectAltDX.getY() || 
                rect.getY() == obstacle.rectDownSX.getY() || rect.getY() == obstacle.rectDownDX.getY())) 
        return 1;
    
    else
        return 2;
}

std::string findInVectPos(Square rect) {
    std::string direction = "NotFound";
    for (int i = 0; i < vectorPosChanged.size(); i++) {
        if (vectorPosChanged[i].getX() == rect.getX() && vectorPosChanged[i].getY() == rect.getY()) {
            return vectorPosChanged[i].getDirection();
        }
    }
    return direction;
}

// questa funzione riposiziona correttamente ogni parte del corpo di snake quando esce dalla
// window, quindi quando le coordinate sono <0 o >DIM_H (che è la componente di altezza massima)
// o >DIM_V (che è la componente di ampiezza massima)
void checkIfOutOfWindow(int i) {
    if (i != -1) {
        if (vectorBody[i].getX() < 0) {
            vectorBody[i].rect.x = DIM_H;
            printf("la x era < 0, ora è: %f", vectorBody[i].getX());
        } 
        
        else if (vectorBody[i].getX() == DIM_H) {
            vectorBody[i].rect.x = 0;
            printf("la x era > MAX, ora è: %f", vectorBody[i].getX());
        } 
        
        else if (vectorBody[i].getY() < 0) {
            vectorBody[i].rect.y = DIM_V;
            printf("la y era < 0, ora è: %f", vectorBody[i].getY());
        }
        
        else if (vectorBody[i].getY() == DIM_H) {
            vectorBody[i].rect.y = 0;
            printf("la y era > MAX, ora è: %f", vectorBody[i].getY());
        }
    } else {
        if (pivot.getX() < 0) {
            pivot.rect.x = DIM_H;
            printf("la x era < 0, ora è: %f", pivot.getX());
        } 
        
        else if (pivot.getX() == DIM_H) {
            pivot.rect.x = 0;
            printf("la x era > MAX, ora è: %f", pivot.getX());
        } 
        
        else if (pivot.getY() < 0) {
            pivot.rect.y = DIM_V;
            printf("la y era < 0, ora è: %f", pivot.getY());
        }
        
        else if (pivot.getY() == DIM_H) {
            pivot.rect.y = 0;
            printf("la y era > MAX, ora è: %f", pivot.getY());
        }
    }
    return;
}

void moveSnake() {
    //int nextMove = -1;
    while (!endThread) {
        if (pivot.direction.load() == -1)
            printf("La direzione è vuota: ho appena iniziato\n");
        else {
            std::string currDir = "";
            float yDirect = pivot.rect.y;
            float xDirect= pivot.rect.x;
            switch (pivot.direction.load())
            {
            case 1:
                currDir = "SDLK_UP";
                yDirect-=L;
                break;
            case 2:
                currDir = "SDLK_RIGHT";
                xDirect+=L;
                break;
            case 3:
                currDir = "SDLK_DOWN";
                yDirect+=L;
                break;
            case 4:
                currDir = "SDLK_LEFT";
                xDirect-=L;
                break;
            }
            printf("La direzione è %s\n", currDir.c_str());
            int nextMove = nextMoveIsRect(pivot.rect, food, obstacle, currDir);
            // caso in cui rect mangia food
            if (nextMove == 0) {
                // incremento lo score
                currentScore++;

                // riposiziono food
                food.updatePos();

                // snake cresce
                if (vectorBody.size() == 0) {
                    Body newBody = {pivot.rect.x, pivot.rect.y, L, L, currDir};
                    vectorBody.push_back(newBody);
                }
                    
                else
                    growing(vectorBody[vectorBody.size()-1]);
            }

            // caso in cui rect incontra un ostacolo 
            else if (nextMove == 1) {
                return;
            }

            // caso in cui non ci sono intersezioni
            else {
                if (vectorBody.size() == 0) {
                    pivot.direction.store(pivot.direction.load(), std::memory_order_relaxed);
                    pivot.rect.y = yDirect;
                    pivot.rect.x = xDirect;
                }
                else {
                    onButtonMove(currDir);
                    removeUselessPos();
                }
            }
        }
    
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return;
}

void onButtonMove(std::string direction) {
    std::string pivotDirectionBeforeChange = "";

    switch (pivot.direction.load())
    {
    case 1:
        pivotDirectionBeforeChange = "SDLK_UP";
        break;
    case 2:
        pivotDirectionBeforeChange = "SDLK_RIGHT";
        break;
    case 3:
        pivotDirectionBeforeChange = "SDLK_DOWN";
        break;
    case 4:
        pivotDirectionBeforeChange = "SDLK_LEFT";
        break;
    }
        

    // aggiungo il punto di svolta al vettore vectorPosChanged
    if (pivotDirectionBeforeChange != direction) {
        Position directionChanged = Position(pivot.rect.x, pivot.rect.y, direction);
        vectorPosChanged.push_back(directionChanged);
    }

    for (int i = vectorBody.size()-1; i >= 0; i--) {
        // se c'è almeno un elemento nel vettore delle posizioni
        if (vectorPosChanged.size() != 0) {
            // per ogni elemento cerco se si trova in un punto di svolta
            std::string found = findInVectPos(vectorBody[i]);

            if (found != "NotFound"){
                printf("New direction: %s\n", found.c_str());
                vectorBody[i].setDirection(found);
            }
            else 
                printf("Element not found.\n");
        }

        if (vectorBody[i].getDirection() == "SDLK_DOWN") {
                vectorBody[i].rect.y += L;
        }
        else if (vectorBody[i].getDirection() == "SDLK_UP") {
                vectorBody[i].rect.y -= L;
        }
        else if (vectorBody[i].getDirection() == "SDLK_RIGHT") {
                vectorBody[i].rect.x += L;
        }
        else if (vectorBody[i].getDirection() == "SDLK_LEFT") {
                vectorBody[i].rect.x -= L;
        }
        checkIfOutOfWindow(i);
    }
    
    // alla fine del ciclo che parte dalla fine del serpente alla testa (il pivot)
    // rifaccio tutte le azioni sul pivot 
    if (vectorPosChanged.size() != 0) {
        // per ogni elemento cerco se si trova in un punto di svolta
        std::string found = findInVectPos(pivot.rect);

        if (found != "NotFound"){
            printf("New direction: %s\n", found.c_str());

            int directionInt = -1;
            if (strcmp(found.c_str(), "SDLK_UP")==0)
                directionInt = 1;
            else if (strcmp(found.c_str(), "SDLK_RIGHT")==0)
                directionInt = 2;
            else if (strcmp(found.c_str(), "SDLK_DOWN")==0)
                directionInt = 3;
            else if (strcmp(found.c_str(), "SDLK_LEFT")==0)
                directionInt = 4;

            pivot.direction.store(directionInt, std::memory_order_relaxed);
        }
        else 
            printf("Element not found.\n");
    }
    if (pivot.direction.load(std::memory_order_relaxed) == 3) {
        pivot.rect.y += L;
    }
    else if (pivot.direction.load(std::memory_order_relaxed) == 1) {
            pivot.rect.y -= L;
    }
    else if (pivot.direction.load(std::memory_order_relaxed) == 2) {
            pivot.rect.x += L;
    }
    else if (pivot.direction.load(std::memory_order_relaxed) == 4) {
            pivot.rect.x -= L;
    }

    checkIfOutOfWindow(-1);
    printf("il pivot ha queste coordinate %f,%f\n", pivot.rect.x, pivot.rect.y);

    return;
}

// questa funzione serve a eliminare tutte quelle posizioni salvate nel vettore
// delle posizioni di svolta ogni qual volta che tutto il corpo di snake le ha attraversate
void removeUselessPos() {
    bool found = false;
    printf("%sla size di vectorPosChanged: %zu %s\n", YELLOW, vectorPosChanged.size(), RESET);

    for (int i = 0; i < vectorPosChanged.size(); i++) {
        found = false;
        for (int j = 0; j < vectorBody.size(); j++) {
            if (vectorPosChanged[i].getX() == vectorBody[j].getX() && vectorPosChanged[i].getY() == vectorBody[j].getY()) {
                found = true;
            }
        }
        if (found == false) 
            vectorPosChanged.erase(vectorPosChanged.begin());
    }
    printf("%sla size di vectorPosChanged: %zu %s\n", YELLOW, vectorPosChanged.size(), RESET);
    return;
}

void moveObstacle(Obstacle& obstacle) {

    while(!endThread) {

        bool collision = true;

        // facciamo un check per controllare che questa nuova posizione non sovrapponga il food
        // nè il body

        while (collision) {

            obstacle.rectAltSX.updatePos();

            if ( (obstacle.rectAltSX.getX() == food.getX()) && (obstacle.rectAltSX.getY() == food.getY())
                || (obstacle.rectAltDX.getX() == food.getX()) && (obstacle.rectAltDX.getY() == food.getY())
                || (obstacle.rectDownSX.getX() == food.getX()) && (obstacle.rectDownSX.getY() == food.getY())
                || (obstacle.rectDownDX.getX() == food.getX()) && (obstacle.rectDownDX.getY() == food.getY())) {

                    obstacle.rectAltSX.updatePos();
            }
            else
                collision = false;
        }  

        obstacle.setCoordinates();

        // printf("%scoordinate square alto sinistra: %f,%f\n", RED, obstacle.getRectAltSX().getX(), obstacle.getRectAltSX().getY());
        // printf("%scoordinate square alto destra: %f,%f\n", GREEN, obstacle.getRectAltDX().getX(), obstacle.getRectAltDX().getY());
        // printf("%scoordinate square basso sinistra: %f,%f\n", BLUE, obstacle.getRectDwnSX().getX(), obstacle.getRectDwnSX().getY());
        // printf("%scoordinate square basso destra: %f,%f\n", YELLOW, obstacle.getRectDwnDX().getX(), obstacle.getRectDwnDX().getY());

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return;
}
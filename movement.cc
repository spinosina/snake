#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <thread>

#include "movement.h"
#include "snake.h"
#include "positions.h"
#include "globals.h"
#include "growth.h"

// FUNCTIONS EXPLAINED IN HEADER

int nextMoveIsRect(Square rect, Square food, Obstacle obstacle, std::string direction) {
    // prossimo passo
    if (direction == "SDLK_DOWN") 
        rect.setY(rect.getY() + L);

    else if (direction == "SDLK_UP") 
        rect.setY(rect.getY() - L);

    else if (direction == "SDLK_RIGHT") 
        rect.setX(rect.getX() + L);

    else if (direction == "SDLK_LEFT") 
        rect.setX(rect.getX() - L);
        
    // collisione con food
    if (rect.getX() == food.getX() && rect.getY() == food.getY()) {
        onButtonMove(direction);
        return 0;
    }

    // collisione con l'ostacolo o con i lati: cambio skin per lo scontro
    else if (((rect.getX() == obstacle.rectAltSX.getX() || rect.getX() == obstacle.rectAltDX.getX() || 
                rect.getX() == obstacle.rectDownSX.getX() || rect.getX() == obstacle.rectDownDX.getX())
                && (rect.getY() == obstacle.rectAltSX.getY() || rect.getY() == obstacle.rectAltDX.getY() || 
                rect.getY() == obstacle.rectDownSX.getY() || rect.getY() == obstacle.rectDownDX.getY()))
                || (rect.getX() == DIM_H || rect.getX() < 0 || rect.getY() == DIM_H || rect.getY() < 0)) {
        
        if (direction=="SDLK_UP" && pivot.direction.load(std::memory_order_relaxed)==2){
            pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaCurva_DXSU.png";
        }
        else if (direction=="SDLK_UP" && pivot.direction.load(std::memory_order_relaxed)==4){
            pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaCurva_SXSU.png";
        }
        else if (direction=="SDLK_DOWN" && pivot.direction.load(std::memory_order_relaxed)==2){
            pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaCurva_DXGIU.png";
        }
        else if (direction=="SDLK_DOWN" && pivot.direction.load(std::memory_order_relaxed)==4){
            pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaCurva_SXGIU.png";
        }
        else if (direction=="SDLK_RIGHT" && pivot.direction.load(std::memory_order_relaxed)==1){
            pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaCurva_SUDX.png";
        }
        else if (direction=="SDLK_RIGHT" && pivot.direction.load(std::memory_order_relaxed)==3){
            pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaCurva_GIUSX.png";
        }
        else if (direction=="SDLK_LEFT" && pivot.direction.load(std::memory_order_relaxed)==1){
            pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaCurva_SUSX.png";
        }
        else if (direction=="SDLK_LEFT" && pivot.direction.load(std::memory_order_relaxed)==3){
            pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaCurva_GIUDX.png";
        }
        return 1;
    }

    // collisione con sè stesso
    else if (nextMoveIsSnake(rect) == 3) 
        return 3;
    
    // no collision
    else
        return 2;
}

int nextMoveIsSnake(Square rect) {
    float xPivot = rect.getX();
    float yPivot = rect.getY();
    for (int i = 0; i<vectorBody.size(); i++) {
        if ((vectorBody[i].rect.x == xPivot) && (vectorBody[i].rect.y == yPivot))
            return 3;
    }
    return -1;
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

// NO MORE USED
void checkIfOutOfWindow(int i) {
    float x; float y;
    if (i != -1) {
        x = vectorBody[i].getX();
        y = vectorBody[i].getY();
    } else {
        x = pivot.getX();
        y = pivot.getY();
    }
     
    if (x < 0) {
        x = DIM_H;
        printf("la x era < 0, ora è: %f\n", x);
    } 
    
    else if (x >= DIM_H) {
        x = 0;
        printf("la x era > MAX, ora è: %f\n", x);
    } 
    
    else if (y < 0) {
        y = DIM_V-L;
        printf("la y era < 0, ora è: %f\n", y);
    }
    
    else if (y >= DIM_V) {
        y = 0;
        printf("la y era > MAX, ora è: %f\n", y);
    }

    if (i != -1) {
        vectorBody[i].setX(x);
        vectorBody[i].setY(y);
    } else {
        pivot.setX(x);
        pivot.setY(y);
    }

    return;
}

void moveSnake() {
    //int nextMove = -1;
    while (!endThread) {
        std::string currDir = "";
        float yDirect = pivot.rect.y;
        float xDirect= pivot.rect.x;
        float yForNewElem = pivot.rect.y;
        float xForNewElem= pivot.rect.x;
        currDir = getDirectionFromPivot(pivot.direction.load());
        if (currDir.c_str() == "SDLK_UP")
            yDirect-=L;
        else if (currDir.c_str() == "SDLK_RIGHT")
            xDirect+=L;
        else if (currDir.c_str() == "SDLK_DOWN")
            yDirect+=L;
        else if (currDir.c_str() == "SDLK_LEFT")
            xDirect-=L;

        //printf("%sall'inizio il pivot ha queste coordinate: %f, %f\n%s", RED, pivot.rect.x, pivot.rect.y, RESET);
        //printf("%slo sposto a queste coordinate: %f, %f\n%s", YELLOW, xDirect, yDirect, RESET);

        int nextMove = nextMoveIsRect(pivot.rect, food, obstacle, currDir);
        // caso in cui rect mangia food
        if (nextMove == 0) {
            // incremento lo score e riposiziono il food
            currentScore++;
            food.updatePos();

            // se non c'è ancora un corpo lo creo e modifico la skin inserendo la coda
            if (vectorBody.size() == 0) {
                Body newBody = {xForNewElem, yForNewElem, L, L, currDir};
                vectorBody.push_back(newBody);
                pivotSkinPath = getPivotNewSkinAfterCollision(pivot.direction.load());
            }
            // sennò faccio crescere il corpo
            else
                growing(vectorBody[vectorBody.size()-1]);
        }

        // caso in cui rect incontra un ostacolo o se stesso
        else if (nextMove == 1 || nextMove == 3) {
            endThread = true;
        }

        // caso in cui no intersezioni
        else {
            // se non c'è ancora il corpo sposto solo la testa
            if (vectorBody.size() == 0) {
                pivot.direction.store(pivot.direction.load(), std::memory_order_relaxed);
                pivot.setX(xDirect);
                pivot.setY(yDirect);
            }
            // sennò chiamo la funzione per far crescere anche il corpo
            // ed eliminare eventuali posizioni di curva già superate
            else {
                onButtonMove(currDir);
                removeUselessPos();
            }
        }
    
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return;
}

void onButtonMove(std::string direction) {
    std::string pivotDirectionBeforeChange = getDirectionFromPivot(pivot.direction.load());

    // se sto cambiando direzione sono in una curva
    // quindi aggiungo il punto di svolta al vettore vectorPosChanged
    if (pivotDirectionBeforeChange != direction) {
        Position directionChanged = Position(pivot.rect.x, pivot.rect.y, direction);
        vectorPosChanged.push_back(directionChanged);
    }

    // sposto il serpente dalla coda alla testa
    for (int i = vectorBody.size()-1; i >= 0; i--) {
        // se c'è almeno un elemento nel vettore delle posizioni di curva
        if (vectorPosChanged.size() != 0) {
            // per ogni elemento cerco se si trova in un punto di curva, se c'è 
            // imposto la nuova direzione del corpo
            std::string found = findInVectPos(vectorBody[i]);

            if (found != "NotFound"){
                vectorBody[i].setDirection(found);
            }
        }

        // poi in base alla direzione del rect sposto la parte del corpo
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
        //checkIfOutOfWindow(i);
    }
    
    // alla fine dello spostamento del corpo sposto il pivot
    if (vectorPosChanged.size() != 0) {
        // cerco se il pivot si trova in un punto di svolta, se sì
        // cambio la sua direzione
        std::string found = findInVectPos(pivot.rect);
        if (found != "NotFound")
            pivot.direction.store(getPivotFromDirection(found));
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

    //checkIfOutOfWindow(-1);
    return;
}

void removeUselessPos() {
    bool found = false;

    for (int i = 0; i < vectorPosChanged.size(); i++) {
        found = false;
        for (int j = 0; j < vectorBody.size(); j++) {
            if (vectorPosChanged[i].getX() == vectorBody[j].getX() && vectorPosChanged[i].getY() == vectorBody[j].getY()) {
                found = true;
            }
        }
        // cancello la posizione se già attraversata da tutto il corpo
        if (found == false) 
            vectorPosChanged.erase(vectorPosChanged.begin());
    }
    return;
}

// REVIEW THIS FUNCTION
void moveObstacle(Obstacle& obstacle) {

    while(!endThread) {

        bool collision = true;

        // facciamo un check per controllare che questa nuova posizione non sovrapponga il food
        // nè il body

        while (collision) {  // deve essere true per continuare il ciclo

            obstacle.rectAltSX.updatePos();

            // controllo se viene generato in collisione col food
            if (((obstacle.rectAltSX.getX() == food.getX()) && (obstacle.rectAltSX.getY() == food.getY()))
                || ((obstacle.rectAltDX.getX() == food.getX()) && (obstacle.rectAltDX.getY() == food.getY()))
                || ((obstacle.rectDownSX.getX() == food.getX()) && (obstacle.rectDownSX.getY() == food.getY()))
                || ((obstacle.rectDownDX.getX() == food.getX()) && (obstacle.rectDownDX.getY() == food.getY()))) {

                    obstacle.rectAltSX.updatePos();
            }
            else 
                collision = false;

            if (collision == false) {
                // controllo se viene generato in collisione col serpente
                for (int i = 0; i < vectorBody.size(); i++) {
                    if (((obstacle.rectAltSX.getX() == vectorBody[i].getX()) && (obstacle.rectAltSX.getY() == vectorBody[i].getY()))
                    || ((obstacle.rectAltDX.getX() == vectorBody[i].getX()) && (obstacle.rectAltDX.getY() == vectorBody[i].getY()))
                    || ((obstacle.rectDownSX.getX() == vectorBody[i].getX()) && (obstacle.rectDownSX.getY() == vectorBody[i].getY()))
                    || ((obstacle.rectDownDX.getX() == vectorBody[i].getX()) && (obstacle.rectDownDX.getY() == vectorBody[i].getY()))) {

                        obstacle.rectAltSX.updatePos();
                    }
                    else
                        collision = false;
                }
            } else
                collision = true;
        }  

        obstacle.setCoordinates();

        // printf("%scoordinate square alto sinistra: %f,%f\n", RED, obstacle.getRectAltSX().getX(), obstacle.getRectAltSX().getY());
        // printf("%scoordinate square alto destra: %f,%f\n", GREEN, obstacle.getRectAltDX().getX(), obstacle.getRectAltDX().getY());
        // printf("%scoordinate square basso sinistra: %f,%f\n", BLUE, obstacle.getRectDwnSX().getX(), obstacle.getRectDwnSX().getY());
        // printf("%scoordinate square basso destra: %f,%f\n", YELLOW, obstacle.getRectDwnDX().getX(), obstacle.getRectDwnDX().getY());

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    return;
}
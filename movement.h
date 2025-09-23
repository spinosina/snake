#ifndef MOVEMENT_TEMP_H
#define MOVEMENT_TEMP_H

#include <string> 
#include "snake.h"

// questa funzione ritorna 0 se il rect incontra un food, 1 se incontra 
// un obstacle , sè stesso o il muro, 2 altrimenti.
// nel primo caso aumenta la sua dimensione, nel secondo fa terminare il loop
// nel terzo non fa nulla: questo rappresenta il caso in cui non ci sono state intersezioni

// se c'è un'intersezione tra rect e food, food deve cambiare posizione e rect deve allungarsi
int nextMoveIsRect(Square rect, Square food, Obstacle obstacle, std::string direction);
int nextMoveIsSnake(Square rect);

// controllo se il serpente è in una delle posizioni in cui c'è stata una curva
std::string findInVectPos(Square rect);

// NO MORE USED
// questa funzione riposiziona correttamente ogni parte del corpo di snake quando esce dalla
// window, quindi quando le coordinate sono <0 o >DIM_H (che è la componente di altezza massima)
// o >DIM_V (che è la componente di ampiezza massima)
void checkIfOutOfWindow(int i);


// funzioni fulcro del movimento. la prima calcola la posizione successiva del serpente e se ci sono collisioni.
// in assenza di collisioni la seconda si occupa dello spostamento di snake e di fargli fare la curva se necessario.
void moveSnake();
void onButtonMove(std::string direction);

// questa funzione serve a eliminare tutte quelle posizioni salvate nel vettore
// delle posizioni di svolta ogni qual volta che tutto il corpo di snake le ha attraversate
void removeUselessPos();

// ricalcolo la posizione dell'ostacolo
void moveObstacle(Obstacle& obstacle);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <thread>

#include "movement_temp.h"
#include "growth_temp.h"
#include "globals_temp.h"

#include "Snake_temp.h"
#include "Positions_temp.h"
#include "SDL_init.h"
#include <SDL.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

#define DIM_H 600 //componente orizzontale della risoluzione
#define DIM_V 600 //componente verticale della risoluzione

#define L 20

std::vector<Body> vectorBody;
std::vector<Position> vectorPosChanged;
bool endThread = false;

int main(void) {
    // il servizio di cui necessitiamo è uno schermo a video
    SDL sdl;
    sdl.init();

    SDL_Window* window = sdl.initWindow();
    
    SDL_Renderer* renderer = sdl.initRenderer(window);

    // creiamo l'evento su cui ci mettiamo in ascolto
    SDL_Event event;
    SDL_WaitEvent(&event);

    // inizializziamo il generatore di numeri casuali
    srand(time(NULL));

    // creiamo il rettangolo con le coordinate di partenza e la direzione : pivot
    Body pivot = {0, 0, L, L, ""};
    // aggiungiamo il rettangolo al vettore
    vectorBody.push_back(pivot);

    // creiamo il food e l'ostacolo 
    Square food = Square(0, 0, L, L); 
    food.updatePos();
    
    Obstacle obstacle = Obstacle();
    
    //lancio il thread di aggiornamento posizione
    std::thread positionThread(moveObstacle, std::ref(obstacle));

    // variabile del ciclo principale
    bool end = false;

    // variabile randomica
    std::srand(std::time(0));  // Inizializza il seme con l'ora corrente

    // variabile del movimento
    int nextMove = -1;

    // ciclo principale di gioco
    while (!end) {
        while(SDL_PollEvent(&event)) {
            // usciamo al click sulla x
            if (event.type == SDL_QUIT) {
                end = true;
                endThread = true;
            }

            // mi muovo in basso
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {

                if (vectorBody[0].getDirection() == "") {
                    printf("caso direction == ""\n");
                    vectorBody[0].setDirection("SDLK_DOWN");
                }
                
                Square pivot = Square(vectorBody[0].rect);
                nextMove = nextMoveIsRect(pivot, food, obstacle, "SDLK_DOWN");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // riposiziono food
                    food.updatePos();

                    // snake cresce
                    growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
                    endThread = true;
                }

                // caso in cui non ci sono intersezioni
                else {
                    if (vectorBody.size() == 1)
                        vectorBody[0].rect.y += L;
                    else {
                        onButtonMove("SDLK_DOWN");
                        removeUselessPos();
                    }
                } 
            }

            // mi muovo in alto
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {

                if (vectorBody[0].getDirection() == "") {
                    printf("caso direction == ""\n");
                    vectorBody[0].setDirection("SDLK_UP");
                }

                nextMove = nextMoveIsRect(vectorBody[0].rect, food, obstacle, "SDLK_UP");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // riposiziono food
                    food.updatePos();

                    // snake cresce
                    growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
                    endThread = true;
                } 

                // caso in cui non ci sono intersezioni
                else {
                    if (vectorBody.size() == 1)
                        vectorBody[0].rect.y -= L;
                    else {
                        onButtonMove("SDLK_UP");
                        removeUselessPos();
                    }
                }
            }

            // mi muovo a sinistra
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {

                if (vectorBody[0].getDirection() == "") {
                    printf("caso direction == ""\n");
                    vectorBody[0].setDirection("SDLK_LEFT");
                }

                nextMove = nextMoveIsRect(vectorBody[0].rect, food, obstacle, "SDLK_LEFT");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // riposiziono food
                    food.updatePos();

                    // snake cresce
                    growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
                    endThread = true;
                }

                // caso in cui non ci sono intersezioni
                else {
                    if (vectorBody.size() == 1)
                        vectorBody[0].rect.x -= L; 
                    else {
                        onButtonMove("SDLK_LEFT");
                        removeUselessPos();
                    }
                }
            }

            // mi muovo a destra
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {

                if (vectorBody[0].getDirection() == "") {
                    printf("caso direction == ""\n");
                    vectorBody[0].setDirection("SDLK_RIGHT");
                }

                nextMove = nextMoveIsRect(vectorBody[0].rect, food, obstacle, "SDLK_RIGHT");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // riposiziono food
                    food.updatePos();

                    // snake cresce
                    growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
                    endThread = true;
                }

                // caso in cui non ci sono intersezioni
                else {
                    if (vectorBody.size() == 1)
                        vectorBody[0].rect.x += L; 
                    else {
                        onButtonMove("SDLK_RIGHT");
                        removeUselessPos();
                    }
                }
            }
            break;
        }

        // setto il colore di sfondo
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Imposta il colore della griglia (nero)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Disegna le linee verticali della griglia
        for (int x = L; x < DIM_V; x += L) {
            SDL_RenderDrawLine(renderer, x, 0, x, DIM_H);
        }

        // Disegna le linee orizzontali della griglia
        for (int y = L; y < DIM_H; y += L) {
            SDL_RenderDrawLine(renderer, 0, y, DIM_V, y);
        }

        // setto il colore di ogni parte di snake
        for (int i = 0; i < vectorBody.size(); i++) {
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            SDL_RenderFillRectF(renderer, &(vectorBody[i].rect));
        }

        // setto il colore del food
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRectF(renderer, &(food.rect));

        // setto il colore dell'ostacolo
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(renderer, &(obstacle.rectAltSX.rect));
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(renderer, &(obstacle.rectAltDX.rect));
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(renderer, &(obstacle.rectDownSX.rect));
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(renderer, &(obstacle.rectDownDX.rect));

        // applico al renderer
        SDL_RenderPresent(renderer);
    }
    // Attendi la terminazione del thread
    positionThread.join();
    
    sdl.destroyAll(window, renderer);
    return 0;
}

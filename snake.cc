#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <vector>

#include "movement.h"
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

#define DIM_H 600 //componente orizzontale della risoluzione
#define DIM_V 600 //componente verticale della risoluzione
#define CELL_SIZE 20

#define L 20
#define MAX 600
#define MIN 0

std::vector<struct body> vectorBody;
std::vector<struct posChanged> vectorPosChanged;

int main(void) {
    // il servizio di cui necessitiamo è uno schermo a video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR IN VIDEO INIT: %s\n", SDL_GetError());
        return 1;
    }

    // creiamo la finestra
    SDL_Window* window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                          DIM_H, DIM_V, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("ERROR IN WINDOW INIT: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }

    // creiamo il renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL) {
        printf("ERROR IN SCREEN INIT: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        return 1;
    }

    // creiamo l'evento su cui ci mettiamo in ascolto
    SDL_Event event;
    SDL_WaitEvent(&event);

    // creiamo il rettangolo con le coordinate di partenza e la direzione : pivot
    SDL_FRect rect1 = {0, 0, L, L};
    std::string direction = "";
    struct body pivot;
    pivot.rect = rect1;
    pivot.direction = direction;
    // aggiungiamo il rettangolo al vettore
    vectorBody.push_back(pivot);

    // creiamo il food e l'ostacolo
    SDL_FRect food = {0, 100, L, L}; 
    SDL_FRect obstacle = {100, 0, L, L};

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
            if (event.type == SDL_QUIT)
                end = true;

            // mi muovo in basso
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {

                if (direction == "") {
                    printf("caso direction == ""\n");
                    direction = "SDLK_DOWN";
                    vectorBody[0].direction = "SDLK_DOWN";
                }

                nextMove = nextMoveIsRect(vectorBody[0].rect, food, obstacle, "SDLK_DOWN");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // ricavo delle nuove coordinate randomiche e riposiziono food
                    struct pos coord;
                    coord = getNewCoordinates();

                    food.x = coord.x;
                    food.y = coord.y;

                    // snake cresce
                    growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
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

                if (direction == "") {
                    printf("caso direction == ""\n");
                    direction = "SDLK_UP";
                    vectorBody[0].direction = "SDLK_UP";
                }

                nextMove = nextMoveIsRect(vectorBody[0].rect, food, obstacle, "SDLK_UP");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // ricavo delle nuove coordinate randomiche e riposiziono food
                    struct pos coord;
                    coord = getNewCoordinates();

                    food.x = coord.x;
                    food.y = coord.y;

                    // snake cresce
                    growing(vectorBody[vectorBody.size()-1]);
                } 

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
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

                if (direction == "") {
                    printf("caso direction == ""\n");
                    direction = "SDLK_LEFT";
                    vectorBody[0].direction = "SDLK_LEFT";
                }

                nextMove = nextMoveIsRect(vectorBody[0].rect, food, obstacle, "SDLK_LEFT");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // ricavo delle nuove coordinate randomiche e riposiziono food
                    struct pos coord;
                    coord = getNewCoordinates();

                    food.x = coord.x;
                    food.y = coord.y;

                    // snake cresce
                    growing(vectorBody[vectorBody.size()-1]);
                } 

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
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

                if (direction == "") {
                    printf("caso direction == ""\n");
                    direction = "SDLK_RIGHT";
                    vectorBody[0].direction = "SDLK_RIGHT";
                }

                nextMove = nextMoveIsRect(vectorBody[0].rect, food, obstacle, "SDLK_RIGHT");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // ricavo delle nuove coordinate randomiche e riposiziono food
                    struct pos coord;
                    coord = getNewCoordinates();

                    food.x = coord.x;
                    food.y = coord.y;

                    // snake cresce
                    growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
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
        for (int x = CELL_SIZE; x < DIM_V; x += CELL_SIZE) {
            SDL_RenderDrawLine(renderer, x, 0, x, DIM_H);
        }

        // Disegna le linee orizzontali della griglia
        for (int y = CELL_SIZE; y < DIM_H; y += CELL_SIZE) {
            SDL_RenderDrawLine(renderer, 0, y, DIM_V, y);
        }

        // setto il colore di ogni parte di snake
        for (int i = 0; i < vectorBody.size(); i++) {
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            SDL_RenderFillRectF(renderer, &(vectorBody[i].rect));
        }

        // setto il colore del food
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRectF(renderer, &food);

        // setto il colore dell'ostacolo
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(renderer, &obstacle);

        // applico al renderer
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    printf("Exiting ...");
    return 0;
}

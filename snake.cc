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
#include <SDL_messagebox.h>

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
Square food = Square(0, 0, L, L);

// creiamo il pivot con le coordinate di partenza e la direzione : pivot
// -1 == nessuna direzione
// 1 == up, 2 == right, 3 == down, 4 == left
Pivot pivot(0.0, 0.0, L, L, -1);

// variabile dello score
int currentScore = 1;

bool endThread = false;

int main(void) {
    // il servizio di cui necessitiamo è uno schermo a video
    SDL sdl;
    sdl.init();

    SDL_Window* window = sdl.initWindow();

    SDL_Renderer* renderer = sdl.initRenderer(window);

    if (TTF_Init() == -1) {
        printf("Errore inizializzazione SDL_ttf: %s\n", TTF_GetError());
        return -1;
    }
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 20);
    if (!font) {
        printf("Errore: il font non è stato caricato! %s\n", TTF_GetError());
        return -1;
    }
    // creiamo l'evento su cui ci mettiamo in ascolto
    SDL_Event event;
    SDL_WaitEvent(&event);

    // inizializziamo il generatore di numeri casuali
    srand(time(NULL));

    // randomizziamo la posizione di food e dell'ostacolo  
    food.updatePosForFood();
    Obstacle obstacle = Obstacle();
    
    //lancio il thread di aggiornamento posizione dell'ostacolo
    std::thread positionThread(moveObstacle, std::ref(obstacle));

    //lancio il thread di spostamento dello snake
    std::thread movingSnakeThread(moveSnake);

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

                if (pivot.direction.load(std::memory_order_relaxed) == -1) {
                    printf("caso direction == ""\n");
                    pivot.direction.store(3, std::memory_order_relaxed);
                }
                 
                nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_DOWN");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // incremento lo score
                    currentScore++;

                    // riposiziono food
                    food.updatePos();

                    // snake cresce
                    if (vectorBody.size() == 0) {
                        Body newBody = {pivot.rect.x, pivot.rect.y-L, L, L, "SDLK_DOWN"};
                        vectorBody.push_back(newBody);
                    }
                        
                    else
                        growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
                    endThread = true;
                    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "final score: ", window);
                }

                // caso in cui non ci sono intersezioni
                else {
                    if (vectorBody.size() == 0)
                        pivot.rect.y += L;
                    else {
                        onButtonMove("SDLK_DOWN");
                        removeUselessPos();
                    }
                } 
            }

            // mi muovo in alto
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {

                if (pivot.direction.load(std::memory_order_relaxed) == -1) {
                    printf("caso direction == ""\n");
                    pivot.direction.store(1, std::memory_order_relaxed);
                }

                nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_UP");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // incremento lo score
                    currentScore++;

                    // riposiziono food
                    food.updatePos();

                    // snake cresce
                    if (vectorBody.size() == 0) {
                        Body newBody = {pivot.rect.x, pivot.rect.y+L, L, L, "SDLK_UP"};
                        vectorBody.push_back(newBody);
                    }
                        
                    else
                        growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
                    endThread = true;
                    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "final score: ", window);
                } 

                // caso in cui non ci sono intersezioni
                else {
                    if (vectorBody.size() == 0)
                        pivot.rect.y -= L;
                    else {
                        onButtonMove("SDLK_UP");
                        removeUselessPos();
                    }
                }
            }

            // mi muovo a sinistra
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {

                if (pivot.direction.load(std::memory_order_relaxed) == -1) {
                    printf("caso direction == ""\n");
                    pivot.direction.store(4, std::memory_order_relaxed);
                }

                nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_LEFT");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // incremento lo score
                    currentScore++;

                    // riposiziono food
                    food.updatePos();

                    // snake cresce
                    if (vectorBody.size() == 0) {
                        Body newBody = {pivot.rect.x+L, pivot.rect.y, L, L, "SDLK_LEFT"};
                        vectorBody.push_back(newBody);
                    }
                        
                    else
                        growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
                    endThread = true;
                    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "final score: ", window);
                }

                // caso in cui non ci sono intersezioni
                else {
                    if (vectorBody.size() == 0)
                        pivot.rect.x -= L;
                    else {
                        onButtonMove("SDLK_LEFT");
                        removeUselessPos();
                    }
                }
            }

            // mi muovo a destra
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {

                if (pivot.direction.load(std::memory_order_relaxed) == -1) {
                    printf("caso direction == ""\n");
                    pivot.direction.store(2, std::memory_order_relaxed);
                }

                nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_RIGHT");
                // caso in cui rect mangia food
                if (nextMove == 0) {
                    // incremento lo score
                    currentScore++;

                    // riposiziono food
                    food.updatePos();

                    // snake cresce
                    if (vectorBody.size() == 0) {
                        Body newBody = {pivot.rect.x-L, pivot.rect.y, L, L, "SDLK_RIGHT"};
                        vectorBody.push_back(newBody);
                    }
                        
                    else
                        growing(vectorBody[vectorBody.size()-1]);
                }

                // caso in cui rect incontra un ostacolo 
                else if (nextMove == 1) {
                    end = true;
                    endThread = true;
                    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "final score: ", window);
                }

                // caso in cui non ci sono intersezioni
                else {
                    if (vectorBody.size() == 0)
                        pivot.rect.x += L; 
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

        SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
        SDL_RenderFillRectF(renderer, &(pivot.rect));

        // setto il colore di ogni parte di snake
        for (int i = 0; i < vectorBody.size(); i++) {
            SDL_SetRenderDrawColor(renderer, 173, 255, 47, 255);
            SDL_RenderFillRectF(renderer, &(vectorBody[i].rect));
        }

        // setto il colore del food
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
        SDL_RenderFillRectF(renderer, &(food.rect));

        // setto il colore dell'ostacolo
        SDL_SetRenderDrawColor(renderer, 72, 61, 139, 255);
        SDL_RenderFillRectF(renderer, &(obstacle.rectAltSX.rect));
        SDL_SetRenderDrawColor(renderer, 72, 61, 139, 255);
        SDL_RenderFillRectF(renderer, &(obstacle.rectAltDX.rect));
        SDL_SetRenderDrawColor(renderer, 72, 61, 139, 255);
        SDL_RenderFillRectF(renderer, &(obstacle.rectDownSX.rect));
        SDL_SetRenderDrawColor(renderer, 72, 61, 139, 255);
        SDL_RenderFillRectF(renderer, &(obstacle.rectDownDX.rect));

        sdl.drawInfoBar(renderer, font, currentScore);
        // applico al renderer
        SDL_RenderPresent(renderer);
    }
    printf("FINAL SCORE = %d\n", currentScore);
    const char* finalScore = "FINAL SCORE: " + currentScore;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", finalScore, window);

    // Attendi la terminazione del thread
    positionThread.join();
    movingSnakeThread.join();
    
    sdl.destroyAll(window, renderer);
    return 0;
}

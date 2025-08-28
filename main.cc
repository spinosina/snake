#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

#include "movement.h"
#include "growth.h"
#include "globals.h"

#include "snake.h"
#include "positions.h"
#include "SDLinit.h"
#include <SDL.h>
#include <SDL_messagebox.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"


std::vector<Body> vectorBody;
std::vector<Position> vectorPosChanged;
Square food = Square(0, 0, L, L);
Obstacle obstacle = Obstacle();

// creiamo il pivot con le coordinate di partenza e la direzione : pivot
// -1 == nessuna direzione
// 1 == up, 2 == right, 3 == down, 4 == left
Pivot pivot(0.0, 0.0, L, L, -1);

// variabile dello score
int currentScore = 0;

std::atomic<bool> endThread(false);

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

    // inizializzo la skin della testa di snake
    SDL_Texture* pivotSkin = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Down.png", renderer);
    if (!pivotSkin) {
        printf("Error in pivotSkin. Exit ...");
        return -1; 
    }

    // inizializzo la skin del corpo di snake
    SDL_Texture* pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Body_Down.png", renderer);
    if (!pivotSkin) {
        printf("Error in pivotBody. Exit ...");
        return -1; 
    }

    // inizializzo la skin del food
    SDL_Texture* foodSkin = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_2_Oliva.png", renderer);
    if (!foodSkin) {
        printf("Error in foodSkin. Exit ...");
        return -1;
    }

    // inizializzo la skin dell'ostacolo
    SDL_Texture* obstSkinAltSx = sdl.loadTexture( "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_AltSX.png", renderer);
    if (!obstSkinAltSx) {
        printf("Error in obstSkinAltSx. Exit ...");
        return -1;
    }
    SDL_Texture* obstSkinAltDx = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_AltDX.png", renderer);
    if (!obstSkinAltDx) {
        printf("Error in obstSkinAltDx. Exit ...");
        return -1;
    }
    SDL_Texture* obstSkinDwnSx = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_DwnSX.png", renderer);
    if (!obstSkinDwnSx) {
        printf("Error in obstSkinDwnSx. Exit ...");
        return -1;
    }
    SDL_Texture* obstSkinDwnDx = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_DwnDX.png", renderer);
    if (!obstSkinDwnDx) {
        printf("Error in obstSkinDwnDx. Exit ...");
        return -1;
    }

    // creiamo l'evento su cui ci mettiamo in ascolto
    SDL_Event event;
    SDL_WaitEvent(&event);

    // inizializziamo il generatore di numeri casuali
    srand(time(NULL));
    // variabile randomica
    std::srand(std::time(0));  // Inizializza il seme con l'ora corrente

    // randomizziamo la posizione di food e dell'ostacolo  
    food.updatePosForFood();
    
    //lancio il thread di aggiornamento posizione dell'ostacolo
    std::thread positionThread(moveObstacle, std::ref(obstacle));
    //lancio il thread di spostamento dello snake
    std::thread movingSnakeThread(moveSnake);

    // variabile del movimento
    int nextMove = -1;

    // ciclo principale di gioco
    while (!endThread) {
        while(SDL_PollEvent(&event)) {
            // usciamo al click sulla x
            if (event.type == SDL_QUIT)
                endThread = true;

            // ricaviamo la direzione del pivot
            int getDir = pivot.direction.load(std::memory_order_relaxed);

            // mi muovo in basso
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                
                // caso in cui snake si sta muovendo per la prima volta
                if (pivot.direction.load(std::memory_order_relaxed) == -1) {
                    printf("caso direction == ""\n");
                    pivot.direction.store(3, std::memory_order_relaxed);
                }

                // la prima cosa che verifichiamo è se l'utente sta andando nella direzione 
                // opposta a quella precedente: in quel caso continuerà per la sua strada
                // in questo caso se vado verso l'alto (1) non posso andare verso giù (3)
                // !!!QUESTO FUNZIONA SOLO SE LA SIZE DEL BODY è DIVERSA DA 0!!!
                // SE INVECE ABBIAMO SOLO IL PIVOT NON C'è PROBLEMA
                if (getDir == 1 && vectorBody.size() != 0) {
                    printf(" CANNOT MOVE IN OPPOSITE DIRECTION!! \n");
                    onButtonMove("SDLK_UP");
                    removeUselessPos();
                } else {
                    SDL_DestroyTexture(pivotSkin);
                    pivotSkin = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Down.png", renderer);
                    
                    // tramite la funzione nextMoveIsRect ricavo l'info su cosa succederà al prossimo movimento 
                    // di snake: o mangia il food, o incontra l'ostacolo (o se stesso) o prosegue
                    nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_DOWN");
                    // caso in cui rect mangia food
                    if (nextMove == 0) {
                        // incremento lo score
                        currentScore++;

                        // riposiziono food
                        food.updatePos();

                        // snake cresce
                        // se non esiste ancora il body lo creo
                        if (vectorBody.size() == 0) {
                            Body newBody = {pivot.rect.x, pivot.rect.y-L, L, L, "SDLK_DOWN"};
                            vectorBody.push_back(newBody);
                        }
                        // sennò lo faccio crescere  
                        else
                            growing(vectorBody[vectorBody.size()-1]);
                    }

                    // caso in cui rect incontra un ostacolo o se stesso
                    else if (nextMove == 1 || nextMove == 3) 
                        endThread = true;

                    // caso in cui non ci sono intersezioni
                    else {
                        if (vectorBody.size() == 0) {
                            pivot.direction.store(3, std::memory_order_relaxed);
                            pivot.rect.y += L;
                            //checkIfOutOfWindow(-1);
                        }
                        else {
                            onButtonMove("SDLK_DOWN");
                            removeUselessPos();
                        }
                    } 
                }
            }

            // mi muovo in alto
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {

                // caso in cui snake si sta muovendo per la prima volta
                if (pivot.direction.load(std::memory_order_relaxed) == -1) {
                    printf("caso direction == ""\n");
                    pivot.direction.store(1, std::memory_order_relaxed);
                }

                // la prima cosa che verifichiamo è se l'utente sta andando nella direzione 
                // opposta a quella precedente: in quel caso continuerà per la sua strada
                // in questo caso se vado verso il basso (3) non posso andare verso su (1)
                // !!!QUESTO FUNZIONA SOLO SE LA SIZE DEL BODY è DIVERSA DA 0!!!
                // SE INVECE ABBIAMO SOLO IL PIVOT NON C'è PROBLEMA
                if (getDir == 3 && vectorBody.size() != 0) {
                    printf(" CANNOT MOVE IN OPPOSITE DIRECTION!! \n");
                    onButtonMove("SDLK_DOWN");
                    removeUselessPos();
                } else {
                    SDL_DestroyTexture(pivotSkin);
                    pivotSkin = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake.png", renderer);

                    // tramite la funzione nextMoveIsRect ricavo l'info su cosa succederà al prossimo movimento 
                    // di snake: o mangia il food, o incontra l'ostacolo (o se stesso) o prosegue
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

                    // caso in cui rect incontra un ostacolo o se stesso
                    else if (nextMove == 1 || nextMove == 3) 
                        endThread = true;

                    // caso in cui non ci sono intersezioni
                    else {
                        // se non esiste ancora il body lo creo
                        if (vectorBody.size() == 0) {
                            pivot.direction.store(1, std::memory_order_relaxed);
                            pivot.rect.y -= L;
                            //checkIfOutOfWindow(-1);
                        }
                        else {
                            onButtonMove("SDLK_UP");
                            removeUselessPos();
                        }
                    }
                }
            }

            // mi muovo a sinistra
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {
                
                // caso in cui snake si sta muovendo per la prima volta
                if (pivot.direction.load(std::memory_order_relaxed) == -1) {
                    printf("caso direction == ""\n");
                    pivot.direction.store(4, std::memory_order_relaxed);
                }

                // la prima cosa che verifichiamo è se l'utente sta andando nella direzione 
                // opposta a quella precedente: in quel caso continuerà per la sua strada
                // in questo caso se sono a destra (2) non posso andare a sinistra (4)
                // !!!QUESTO FUNZIONA SOLO SE LA SIZE DEL BODY è DIVERSA DA 0!!!
                // SE INVECE ABBIAMO SOLO IL PIVOT NON C'è PROBLEMA
                if (getDir == 2 && vectorBody.size() != 0) {
                    printf(" CANNOT MOVE IN OPPOSITE DIRECTION!! \n");
                    onButtonMove("SDLK_RIGHT");
                    removeUselessPos();
                } else {
                    SDL_DestroyTexture(pivotSkin);
                    pivotSkin = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Left.png", renderer);

                    // tramite la funzione nextMoveIsRect ricavo cosa succederà al prossimo movimento 
                    // di snake: o mangia il food, o incontra l'ostacolo o prosegue
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
                        // sennò lo faccio crescere  
                        else
                            growing(vectorBody[vectorBody.size()-1]);
                    }

                    // caso in cui rect incontra un ostacolo o se stesso
                    else if (nextMove == 1 || nextMove == 3)
                        endThread = true;

                    // caso in cui non ci sono intersezioni
                    else {
                        if (vectorBody.size() == 0) {
                            pivot.direction.store(4, std::memory_order_relaxed);
                            pivot.rect.x -= L;
                            //checkIfOutOfWindow(-1);
                        }
                        else {
                            onButtonMove("SDLK_LEFT");
                            removeUselessPos();
                        }
                    }
                }
            }

            // mi muovo a destra
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {

                // caso in cui snake si sta muovendo per la prima volta
                if (pivot.direction.load(std::memory_order_relaxed) == -1) {
                    printf("caso direction == ""\n");
                    pivot.direction.store(2, std::memory_order_relaxed);
                }

                // la prima cosa che verifichiamo è se l'utente sta andando nella direzione 
                // opposta a quella precedente: in quel caso continuerà per la sua strada
                // in questo caso se sono a sinistra (4) non posso andare a destra (2)
                // !!!QUESTO FUNZIONA SOLO SE LA SIZE DEL BODY è DIVERSA DA 0!!!
                // SE INVECE ABBIAMO SOLO IL PIVOT NON C'è PROBLEMA
                if (getDir == 4 && vectorBody.size() != 0) {
                    printf(" CANNOT MOVE IN OPPOSITE DIRECTION!! \n");
                    onButtonMove("SDLK_LEFT");
                    removeUselessPos();
                } else {
                    SDL_DestroyTexture(pivotSkin);
                    pivotSkin = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Right.png", renderer);

                    // tramite la funzione nextMoveIsRect ricavo cosa succederà al prossimo movimento 
                    // di snake: o mangia il food, o incontra l'ostacolo o prosegue
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

                    // caso in cui rect incontra un ostacolo o se stesso
                    else if (nextMove == 1 || nextMove == 3)
                        endThread = true;

                    // caso in cui non ci sono intersezioni
                    else {
                        if (vectorBody.size() == 0) {
                            pivot.direction.store(2, std::memory_order_relaxed);
                            pivot.rect.x += L; 
                            //checkIfOutOfWindow(-1);
                        }
                        else {
                            onButtonMove("SDLK_RIGHT");
                            removeUselessPos();
                        }
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

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetTextureBlendMode(pivotSkin, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyF(renderer, pivotSkin, nullptr, &(pivot.rect));

        // setto il colore di ogni parte di snake
        for (int i = 0; i < vectorBody.size(); i++) {

            // chiamo la findInVectPos che mi ritorna la direzione verso cui sto girando se e solo se
            // quel pezzo di body è dentro il vettore delle posizioni cambiate
            std::string directionChanged = findInVectPos(vectorBody[i].rect);
            if (directionChanged == "NotFound") {
                if (vectorBody[i].getDirection() == "SDLK_UP")
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Body.png", renderer);
                else if (vectorBody[i].getDirection() == "SDLK_RIGHT")
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Body_Right.png", renderer);
                else if (vectorBody[i].getDirection() == "SDLK_LEFT")
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Body_Left.png", renderer);
                else if (vectorBody[i].getDirection() == "SDLK_DOWN")
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Body_Down.png", renderer);
                
            } else {
                // se non è l'ultimo elemento e se esiste un elemento precedente
                if ((i+1 != vectorBody.size()) && (i != vectorBody.size())) {

                    // GESTIONE DELLE CURVE --- senso antiorario
                    // da up a left
                    if (directionChanged == "SDLK_LEFT" && vectorBody[i+1].direction == "SDLK_UP") {
                        pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Curva_Up_to_Left.png", renderer);
                    } 
                    // da left a down
                    else if (directionChanged == "SDLK_DOWN" && vectorBody[i+1].direction == "SDLK_LEFT") {
                        pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Curva_Left_to_Down.png", renderer);
                    }
                    // da down a right
                    else if (directionChanged == "SDLK_RIGHT" && vectorBody[i+1].direction == "SDLK_DOWN") {
                        pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Curva_Down_to_Right.png", renderer);
                    }
                    // da right a up
                    else if (directionChanged == "SDLK_UP" && vectorBody[i+1].direction == "SDLK_RIGHT") {
                        pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Curva_Right_to_Up.png", renderer);
                    }

                    // GESTIONE DELLE CURVE --- senso orario
                    // da up a right
                    if (directionChanged == "SDLK_RIGHT" && vectorBody[i+1].direction == "SDLK_UP") {
                        pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Curva_Up_to_Right.png", renderer);
                    } 
                    // da right a down
                    else if (directionChanged == "SDLK_DOWN" && vectorBody[i+1].direction == "SDLK_RIGHT") {
                        pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Curva_Right_to_Down.png", renderer);
                    }
                    // da down a left
                    else if (directionChanged == "SDLK_LEFT" && vectorBody[i+1].direction == "SDLK_DOWN") {
                        pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Curva_Down_to_Left.png", renderer);
                    }
                    // da left a up
                    else if (directionChanged == "SDLK_UP" && vectorBody[i+1].direction == "SDLK_LEFT") {
                        pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try_1_Snake_Curva_Left_to_Up.png", renderer);
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_SetTextureBlendMode(pivotBody, SDL_BLENDMODE_BLEND);
            SDL_RenderCopyF(renderer, pivotBody, nullptr,&(vectorBody[i].rect));
        }

        // setto il colore del food
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
        //SDL_RenderFillRectF(renderer, &(food.rect));
        SDL_SetTextureBlendMode(foodSkin, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyF(renderer, foodSkin, nullptr,&(food.rect));
        

        // setto il colore dell'ostacolo
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_SetTextureBlendMode(obstSkinAltSx, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyF(renderer, obstSkinAltSx, nullptr,&(obstacle.rectAltSX.rect));

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_SetTextureBlendMode(obstSkinAltDx, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyF(renderer, obstSkinAltDx, nullptr,&(obstacle.rectAltDX.rect));

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_SetTextureBlendMode(obstSkinDwnSx, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyF(renderer, obstSkinDwnSx, nullptr,&(obstacle.rectDownSX.rect));

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_SetTextureBlendMode(obstSkinDwnDx, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyF(renderer, obstSkinDwnDx, nullptr,&(obstacle.rectDownDX.rect));

        sdl.drawInfoBar(renderer, font, currentScore);
        // applico al renderer
        SDL_RenderPresent(renderer);
    }
    printf("FINAL SCORE = %d\n", currentScore);
    //std::string currentScoreStr = std::to_string(currentScore);
    //const char* finalScore = "FINAL SCORE: " + currentScoreStr;
    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", finalScore, window);

    // Attendi la terminazione del thread
    positionThread.join();
    movingSnakeThread.join();

    sdl.destroyAll(window, renderer);
    return 0;
}

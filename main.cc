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
#include <SDL_ttf.h>
#include <SDL_image.h>

// VARIABILI DI GIOCO
    // corpo e posizioni di curva
    std::vector<Body> vectorBody; 
    std::string bodySkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/Corpo.png";
    std::vector<Position> vectorPosChanged;
    // elementi secondari
    Square food = Square(0, 0, L, L);
    std::string foodSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/Oliva_NoBack_NoBorder.png";
    Obstacle obstacle = Obstacle();
    std::string obstacleAltSxSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_AltSX.png";
    std::string obstacleAltDxSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_AltDX.png";
    std::string obstacleDownSxSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_DwnSX.png";
    std::string obstacleDownDxSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try_1_Rock_DwnDX.png";
    // pivot e skin
    Pivot pivot(0.0, 0.0, L, L, -1);
    std::string pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaGiu_Mono.png";
    // variabile dello score
    int currentScore = 0;
    // variabile atomica del thread
    std::atomic<bool> endThread(false);

// VARIABILI DI MENU
    // cursore e skin
    SDL_Rect cursorPosition = {370, 480, 40, 40};
    SDL_Surface* surfaceCursore = IMG_Load("/Users/marianna/Desktop/snakes/Skin/Puntatore.png");


void renderGame(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL sdl, SDL_Texture* pivotSkin, SDL_Texture* pivotBody, SDL_Texture* foodSkin,
    SDL_Texture* obstSkinAltSx, SDL_Texture* obstSkinAltDx, SDL_Texture* obstSkinDwnSx, SDL_Texture* obstSkinDwnDx) {

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

    /*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);*/

    // setto la skin del pivot
    pivotSkin = sdl.loadTexture(pivotSkinPath, renderer);
    SDL_SetTextureBlendMode(pivotSkin, SDL_BLENDMODE_BLEND);
    SDL_RenderCopyF(renderer, pivotSkin, nullptr, &(pivot.rect));

    // setto la skin del body
    for (int i = 0; i < vectorBody.size(); i++) {

        // chiamo la findInVectPos che mi ritorna la direzione verso cui sto girando se e solo se
        // quel pezzo di body è dentro il vettore delle posizioni cambiate
        std::string directionChanged = findInVectPos(vectorBody[i].rect);
        if (directionChanged == "NotFound") {
            // caso in cui non ho girato
            // il primo controllo setta la direzione, il secondo decide se l'elemento 
            // considerato è l'ultimo del corpo
            if ((vectorBody[i].getDirection() == "SDLK_UP") && (i+1 == vectorBody.size()))
                pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo_CodaSu.png", renderer);
            else if ((vectorBody[i].getDirection() == "SDLK_UP"))
                pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo.png", renderer);
            else if (vectorBody[i].getDirection() == "SDLK_RIGHT" && (i+1 == vectorBody.size()))
                pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo_CodaDx.png", renderer);
            else if (vectorBody[i].getDirection() == "SDLK_RIGHT")
                pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CorpoSxDx.png", renderer);
            else if (vectorBody[i].getDirection() == "SDLK_LEFT" && (i+1 == vectorBody.size()))
                pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo_CodaSx.png", renderer);
            else if (vectorBody[i].getDirection() == "SDLK_LEFT")
                pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CorpoSxDx.png", renderer);
            else if (vectorBody[i].getDirection() == "SDLK_DOWN" && (i+1 == vectorBody.size()))
                pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo_CodaGiu.png", renderer);
            else if (vectorBody[i].getDirection() == "SDLK_DOWN")
                pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo.png", renderer);
            
        } else {
            // caso curva
            // se non è l'ultimo elemento e se esiste un elemento precedente
            if ((i+1 != vectorBody.size()) && (i != vectorBody.size())) {
                
                // siccome non è l'ultimo elemento, il precedente potrebbe trovarsi su una curva
                // a sua volta. in quel caso bisogna considerare la nuova direzione che sta per prendere
                std::string directionPreviousRectChanged = findInVectPos(vectorBody[i+1].rect);
                std::string directionToCompare = directionPreviousRectChanged!="NotFound"? directionPreviousRectChanged:vectorBody[i+1].direction;

                // GESTIONE DELLE CURVE --- senso antiorario
                // da up a left
                if (directionChanged == "SDLK_LEFT" && directionToCompare == "SDLK_UP") {
                    //printf("%s da SOPRA A SINISTRA \n%s", RED, RESET);
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CURVADXGIU.png", renderer);
                }
                // da left a down
                else if (directionChanged == "SDLK_DOWN" && directionToCompare == "SDLK_LEFT") {
                    //printf("%s da SINISTRA A SOTTO \n%s", RED, RESET);
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CURVAGIUSX.png", renderer);
                }
                // da down a right
                else if (directionChanged == "SDLK_RIGHT" && directionToCompare == "SDLK_DOWN") {
                    //printf("%s da SOTTO A DESTRA \n%s", RED, RESET);
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CURVASUDX.png", renderer);
                }
                // da right a up
                else if (directionChanged == "SDLK_UP" && directionToCompare == "SDLK_RIGHT") {
                    //printf("%s da DESTRA A SOPRA \n%s", RED, RESET);
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CURVASXSU.png", renderer);
                }

                // GESTIONE DELLE CURVE --- senso orario
                // da up a right
                if (directionChanged == "SDLK_RIGHT" && directionToCompare == "SDLK_UP") {
                    //printf("%s da SOPRA A DESTRA \n%s", RED, RESET);
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CURVAGIUSX.png", renderer);
                } 
                // da right a down
                else if (directionChanged == "SDLK_DOWN" && directionToCompare == "SDLK_RIGHT") {
                    //printf("%s da DESTRA A SOTTO \n%s", RED, RESET);
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CURVADXGIU.png", renderer);
                }
                // da down a left
                else if (directionChanged == "SDLK_LEFT" && directionToCompare == "SDLK_DOWN") {
                    //printf("%s da SOTTO A SINISTRA \n%s", RED, RESET);
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CURVASXSU.png", renderer);
                }
                // da left a up
                else if (directionChanged == "SDLK_UP" && directionToCompare == "SDLK_LEFT") {
                    //printf("%s da SINISTRA A SOPRA \n%s", RED, RESET);
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/CURVASUDX.png", renderer);
                }
            }
            else if (i != vectorBody.size()) {
                if ((directionChanged == "SDLK_UP"))
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo_CodaSu.png", renderer);
                else if (directionChanged == "SDLK_RIGHT")
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo_CodaDx.png", renderer);
                else if (directionChanged == "SDLK_LEFT")
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo_CodaSx.png", renderer);
                else if (directionChanged == "SDLK_DOWN")
                    pivotBody = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Try3/Corpo_CodaGiu.png", renderer);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetTextureBlendMode(pivotBody, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyF(renderer, pivotBody, nullptr,&(vectorBody[i].rect));
    }

    // setto la skin del food
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_SetTextureBlendMode(foodSkin, SDL_BLENDMODE_BLEND);
    SDL_RenderCopyF(renderer, foodSkin, nullptr,&(food.rect));
    
    // setto la skin dell'ostacolo
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
}

void renderMenu(SDL_Renderer* renderer, SDL sdl) {

    //setto il back del menu
    SDL_Texture* textMenu = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Menu.png", renderer);
    SDL_Rect menu = {0, 0, 704, 640};
    SDL_SetTextureBlendMode(textMenu, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, textMenu, nullptr,&(menu));

    // setto la skin del cursor
    SDL_Texture* textCursor = sdl.loadTexture("/Users/marianna/Desktop/snakes/Skin/Puntatore.png", renderer);
    SDL_SetTextureBlendMode(textCursor, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, textCursor, nullptr,&(cursorPosition));

    // setto la barra sottostante
    SDL_Rect downBar = {0, 640, 704, 255};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &downBar);
}


void handleGame(SDL_Event &event, GameState &state, int getDir, SDL_Renderer* renderer, SDL sdl, SDL_Texture* pivotSkin) {
    // variabile del movimento
    int nextMove = -1;

    // mi muovo in basso
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
        
        // caso in cui snake si sta muovendo per la prima volta
        if (pivot.direction.load(std::memory_order_relaxed) == -1) {
            pivot.direction.store(3, std::memory_order_relaxed);
        }

        // la prima cosa che verifichiamo è se l'utente sta andando nella direzione 
        // opposta a quella precedente: in quel caso continuerà per la sua strada
        // in questo caso se vado verso l'alto (1) non posso andare verso giù (3)
        // !!!QUESTO FUNZIONA SOLO SE LA SIZE DEL BODY è DIVERSA DA 0!!!
        // SE INVECE ABBIAMO SOLO IL PIVOT NON C'è PROBLEMA
        if (getDir == 1 && vectorBody.size() != 0) {
            onButtonMove("SDLK_UP");
            removeUselessPos();
        } else {
            SDL_DestroyTexture(pivotSkin);
            vectorBody.size() != 0 ? pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaGiu.png" :  pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaGiu_Mono.png";
            
            // tramite la funzione nextMoveIsRect ricavo l'info su cosa succederà al prossimo movimento 
            // di snake: o mangia il food, o incontra l'ostacolo (o se stesso o il muro) o prosegue
            nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_DOWN");

            // caso in cui rect mangia food
            if (nextMove == 0) {
                // incremento lo score e riposiziono food
                currentScore++;
                food.updatePosForFood();

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

            // caso in cui rect incontra un ostacolo, il muro o se stesso
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

        if (pivot.direction.load(std::memory_order_relaxed) == -1) {
            pivot.direction.store(1, std::memory_order_relaxed);
        }

        if (getDir == 3 && vectorBody.size() != 0) {
            onButtonMove("SDLK_DOWN");
            removeUselessPos();
        } else {
            SDL_DestroyTexture(pivotSkin);
            vectorBody.size() != 0 ? pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaSu.png" :  pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaSu_Mono.png";            

            nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_UP");
            // caso in cui rect mangia food
            if (nextMove == 0) {
                // incremento lo score e riposiziono food
                currentScore++;
                food.updatePosForFood();

                // snake cresce
                if (vectorBody.size() == 0) {
                    Body newBody = {pivot.rect.x, pivot.rect.y+L, L, L, "SDLK_UP"};
                    vectorBody.push_back(newBody);
                }
                else
                    growing(vectorBody[vectorBody.size()-1]);
            }

            // caso in cui rect incontra un ostacolo, il muro o se stesso
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
        
        if (pivot.direction.load(std::memory_order_relaxed) == -1) {
            pivot.direction.store(4, std::memory_order_relaxed);
        }

        if (getDir == 2 && vectorBody.size() != 0) {
            onButtonMove("SDLK_RIGHT");
            removeUselessPos();
        } else {
            SDL_DestroyTexture(pivotSkin);
            vectorBody.size() != 0 ? pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaSx.png" :  pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaSx_Mono.png";            

            nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_LEFT");
            if (nextMove == 0) {
                // incremento lo score e riposiziono food
                currentScore++;
                food.updatePosForFood();

                // snake cresce
                if (vectorBody.size() == 0) {
                    Body newBody = {pivot.rect.x+L, pivot.rect.y, L, L, "SDLK_LEFT"};
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

        if (pivot.direction.load(std::memory_order_relaxed) == -1) {
            pivot.direction.store(2, std::memory_order_relaxed);
        }

        if (getDir == 4 && vectorBody.size() != 0) {
            onButtonMove("SDLK_LEFT");
            removeUselessPos();
        } else {
            SDL_DestroyTexture(pivotSkin);
            vectorBody.size() != 0 ? pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaDx.png" :  pivotSkinPath = "/Users/marianna/Desktop/snakes/Skin/Try3/TestaDx_Mono.png";            

            nextMove = nextMoveIsRect(pivot.rect, food, obstacle, "SDLK_RIGHT");
            // caso in cui rect mangia food
            if (nextMove == 0) {
                // incremento lo score e riposiziono food
                currentScore++;
                food.updatePosForFood();

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
}

void handleMenu(SDL_Event &event, GameState &state, SDL_Renderer* renderer) {
    // clicco su start
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
        SDL_Texture* textCursor = SDL_CreateTextureFromSurface(renderer, surfaceCursore);
        
        cursorPosition = {370, 410, 40, 40};
        SDL_SetTextureBlendMode(textCursor, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, textCursor, nullptr,&(cursorPosition));

   }

   // clicco su mappa
   else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
        SDL_Texture* textCursor = SDL_CreateTextureFromSurface(renderer, surfaceCursore);
        
        cursorPosition = {370, 480, 40, 40};
        SDL_SetTextureBlendMode(textCursor, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, textCursor, nullptr,&(cursorPosition));

   }

   // muovo il cursore
   else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
    if (cursorPosition.y == 480) {
        //printf("Game Start");
        state = GameState::GAME;
    } else {
        //printf("Game Skin");
        state = GameState::SKIN_SELECTION;
    }
   }
}

int main(void) {
    // il servizio di cui necessitiamo è uno schermo a video
    SDL sdl;
    sdl.init();
    // inizializzo la window
    SDL_Window* window = sdl.initWindow();
    // inizializzo il renderer
    SDL_Renderer* renderer = sdl.initRenderer(window);
    // inizializzo il font
    TTF_Font* font = sdl.initFont();
    // inizializzo la skin della testa di snake
    SDL_Texture* pivotSkin = sdl.loadTexture(pivotSkinPath, renderer);
    // inizializzo la skin del corpo di snake
    SDL_Texture* pivotBody = sdl.loadTexture(bodySkinPath, renderer);
    // inizializzo la skin del food
    SDL_Texture* foodSkin = sdl.loadTexture(foodSkinPath, renderer);
    // inizializzo la skin dell'ostacolo
    SDL_Texture* obstSkinAltSx = sdl.loadTexture(obstacleAltSxSkinPath, renderer);
    SDL_Texture* obstSkinAltDx = sdl.loadTexture(obstacleAltDxSkinPath, renderer);
    SDL_Texture* obstSkinDwnSx = sdl.loadTexture(obstacleDownSxSkinPath, renderer);
    SDL_Texture* obstSkinDwnDx = sdl.loadTexture(obstacleDownDxSkinPath, renderer);

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

    GameState state = GameState::MENU;

    // ciclo principale di gioco
    while (!endThread) {
        while(SDL_PollEvent(&event)) {
            // usciamo al click sulla x
            if (event.type == SDL_QUIT)
                endThread = true;

            // ricaviamo la direzione del pivot
            int getDir = pivot.direction.load(std::memory_order_relaxed);

            switch (state) {
                case GameState::MENU:
                    handleMenu(event, state, renderer);
                    break;
                case GameState::GAME:
                    //printf("GAME\n");
                    handleGame(event, state, getDir, renderer, sdl, pivotSkin);
                    break;
                default:
                    break;
            }
        }
        // Render
        switch (state) {
            case GameState::MENU:
                renderMenu(renderer, sdl);
                break;
            case GameState::GAME:
                renderGame(window, renderer, font, sdl, pivotSkin, pivotBody, foodSkin, obstSkinAltSx, obstSkinAltDx,
                    obstSkinDwnSx, obstSkinDwnDx);
                break;
            default:
                break;
        }
        // applico al renderer
        SDL_RenderPresent(renderer);
    }
    printf("FINAL SCORE = %d\n", currentScore);

    // Attendi la terminazione del thread
    positionThread.join();
    movingSnakeThread.join();

    sdl.destroyAll(window, renderer);
    return 0;
}

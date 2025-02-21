#ifndef SDL_INIT_H
#define SDL_INIT_H

#include <SDL.h>
#include <SDL_ttf.h>

#define DIM_H 600 //componente orizzontale della risoluzione
#define DIM_V 650 //componente verticale della risoluzione

#define DIM_H_TESTO 600 //componente orizzontale della risoluzione
#define DIM_V_TESTO 50 //componente verticale della risoluzione

// la classe SDL nasce per inizializzare le componenti principali:
// Window, surface e renderer
class SDL {
public:

    int init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("ERROR IN VIDEO INIT: %s\n", SDL_GetError());
            return 1;
        }
        return -1;
    }
    SDL_Window* initWindow() {
        SDL_Window* window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                          DIM_H, DIM_V, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("ERROR IN WINDOW INIT: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            return window;
        }
        return window;
    }

    SDL_Renderer* initRenderer(SDL_Window* window) {
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        if (renderer == NULL) {
            printf("ERROR IN SCREEN INIT: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            return renderer;
        }
        return renderer;
    }

    void destroyAll(SDL_Window* window, SDL_Renderer* renderer) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        printf("Exiting ...");
    }

    void drawInfoBar(SDL_Renderer* renderer, TTF_Font* font, int score) {
        // Disegno il rettangolo della barra inferiore
        // {0, 650-50, 600, 50}
        SDL_Rect infoBar = {0, DIM_V - DIM_V_TESTO, DIM_H_TESTO, DIM_V_TESTO};
        SDL_SetRenderDrawColor(renderer, 200, 75, 50, 255);  // Grigio chiaro
        SDL_RenderFillRect(renderer, &infoBar);
    
        // Scrivi del testo nella barra inferiore
        SDL_Color white = {255, 255, 255, 255};
        std::string currentScore = "Current Score:  " + std::to_string(score); 
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, currentScore.c_str(), white);
        if (textSurface == NULL) {
            printf("textSurface è NULL");
            return;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {20, DIM_V - DIM_V_TESTO + 15, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    
        // Pulisci memoria
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
};

#endif
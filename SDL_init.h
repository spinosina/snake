#ifndef SDL_INIT_H
#define SDL_INIT_H

#include <SDL.h>

#define DIM_H 600 //componente orizzontale della risoluzione
#define DIM_V 600 //componente verticale della risoluzione

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
};

#endif
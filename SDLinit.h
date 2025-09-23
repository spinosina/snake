#ifndef SDL_INIT_H
#define SDL_INIT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#define DIM_H_TESTO 640 //componente orizzontale della risoluzione
#define DIM_V_TESTO 64 //componente verticale della risoluzione

// la classe SDL nasce per inizializzare le componenti principali:
// Window, surface e renderer
class SDL {
public:

    int init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("ERROR IN VIDEO INIT: %s\n", SDL_GetError());
            return 1;
        }

        // inizializzazione dell'SDL_image
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "Errore nell'inizializzazione di SDL2_image: " << IMG_GetError() << std::endl;
            return 1;
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // Disabilita il filtro di interpolazione

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
        SDL_SetRenderDrawColor(renderer, 95, 158, 160, 255);  // Grigio chiaro
        SDL_RenderFillRect(renderer, &infoBar);
    
        // Scrivi del testo nella barra inferiore
        SDL_Color black = {0, 0, 0, 255};
        std::string currentScore = "Current Score:  " + std::to_string(score); 
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, currentScore.c_str(), black);
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

    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {

        // carico l'immagine considerandola una surface
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            printf("Errore nel caricamento dell'immagine: %s ", IMG_GetError());
            return nullptr;
        }
        // crea una texture dalla surface (l'immagine)
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            printf("Error in texture. Exit ...");
            return nullptr;
        }
        SDL_FreeSurface(surface);
        return texture;
    }

    TTF_Font* initFont() {
        if (TTF_Init() == -1) {
            printf("Errore inizializzazione SDL_ttf: %s\n", TTF_GetError());
            return nullptr;
        }
        TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 20);
        if (!font) {
            printf("Errore: il font non è stato caricato! %s\n", TTF_GetError());
            return font;
        }
    }
};

#endif
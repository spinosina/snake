#ifndef GROWTH_H
#define GROWTH_H

#include <iostream>
#include <SDL.h>

    struct body {
        SDL_FRect rect;
        std::string direction;
    };

    void growing(struct body body);

#endif
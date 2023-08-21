//
// Created by Owner on 3/5/2023.
//

#ifndef SDL_DRAWCOLOR_HPP
#define SDL_DRAWCOLOR_HPP

#include <stdint.h>
#include <random>
#include "time.h"

class RenderDrawColor {
public:
    uint8_t r,g,b,a;
    RenderDrawColor() : r(0), g(0), b(0), a(255) {}
    RenderDrawColor(int r, int g, int b, int a = 255) : r(r), g(g), b(b), a(a) {}
    static RenderDrawColor RandomColor() {
        srand(clock());
        return {
            rand() % 255,
            rand() % 255,
            rand() % 255,
            rand() % 255,
        };
    }
};


#endif //SDL_DRAWCOLOR_HPP

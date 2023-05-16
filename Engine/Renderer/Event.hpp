//
// Created by Owner on 2/27/2023.
//

#ifndef SDL_EVENT_HPP
#define SDL_EVENT_HPP

#include "SDL.h"

class Event {
private:
    SDL_Event event;
    const Uint8* keyboardState;
    int32_t mousePos[2];
    bool mouseButtonDown[3];
    friend class Renderer;
    friend int main();
public:
    void update();
    Event();
    const uint8_t* keyState();
    [[nodiscard]] const int32_t mx() const;
    [[nodiscard]] const int32_t my() const;
    [[nodiscard]] const bool mouseDownL() const;
    [[nodiscard]] const bool mouseDownR() const;
    [[nodiscard]] const bool mouseDownM() const;

    [[nodiscard]] const bool mouseScrollDown() const;
    [[nodiscard]] const bool mouseScrollUp() const;
};


#endif //SDL_EVENT_HPP

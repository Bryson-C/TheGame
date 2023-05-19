//
// Created by Owner on 2/27/2023.
//

#include "Event.hpp"

void            Event::update()                     {
    SDL_PollEvent(&event);
    keyboardState = SDL_GetKeyboardState(nullptr);
    SDL_GetMouseState(&mousePos[0], &mousePos[1]);

    if (event.button.type == SDL_MOUSEBUTTONDOWN) {
        mouseButtonDown[SDL_BUTTON_LEFT-1] = event.button.button == SDL_BUTTON_LMASK;
        mouseButtonDown[SDL_BUTTON_MIDDLE-1] = event.button.button == SDL_BUTTON_MMASK;
        mouseButtonDown[SDL_BUTTON_RIGHT-1] = event.button.button == SDL_BUTTON_RMASK;
    }
    if (event.button.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0) mouseScroll = -1;
        if (event.wheel.y < 0) mouseScroll = 1;
    }
}
                Event::Event()                      { update(); }
const uint8_t*  Event::keyState()                   { return keyboardState; }
const int32_t   Event::mx()                 const   { return mousePos[0]; }
const int32_t   Event::my()                 const   { return mousePos[1]; }
const bool      Event::mouseDownL()         const   {
    auto m = SDL_GetMouseState(nullptr, nullptr);
    return (mouseButtonDown[SDL_BUTTON_LEFT-1] && m == SDL_BUTTON_LMASK);
}
const bool      Event::mouseDownR()         const   {
    auto m = SDL_GetMouseState(nullptr, nullptr);
    return (mouseButtonDown[SDL_BUTTON_RIGHT-1] && m == SDL_BUTTON_RMASK);
}
const bool      Event::mouseDownM()         const   {
    auto m = SDL_GetMouseState(nullptr, nullptr);
    return (mouseButtonDown[SDL_BUTTON_MIDDLE-1] && m == SDL_BUTTON_MMASK);
}

const bool      Event::mouseScrollDown()    const {
    return mouseScroll == 1;
}
const bool      Event::mouseScrollUp()      const {
    return mouseScroll == -1;
}
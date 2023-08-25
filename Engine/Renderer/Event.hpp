//
// Created by Owner on 2/27/2023.
//

#ifndef SDL_EVENT_HPP
#define SDL_EVENT_HPP

#include <iostream>
#include <string>

#include "SDL.h"

#include "../Utility/Timer/Timer.hpp"

class Event {
private:
    SDL_Event event;
    const Uint8* keyboardState;
    int32_t mousePos[2];
    bool mouseButtonDown[3];
    int32_t mouseScroll;
    
    Timer keyboardTimer{};
    char lastKeyboardKey = ' ';
    
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

    std::string getKeyboardInput(std::string message) {
        if (event.key.type == SDL_KEYDOWN) {
            if (keyState()[SDL_SCANCODE_SPACE]) {
                if ((keyboardTimer.isComplete(10) || lastKeyboardKey != ' ')) {
                    lastKeyboardKey = ' ';
                    keyboardTimer.reset();
                    return message + " ";
                }
                return message;
            }
            if ((event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE || event.key.keysym.scancode == SDL_SCANCODE_DELETE)) {
                if (!message.empty()) message.erase(message.size() - 1);
                return message;
            } else if (keyState()[SDL_SCANCODE_RETURN]) {
                // TODO: Line Breaks
                lastKeyboardKey = ' ';
                return "";
            }
            // Pressing Shift Breaks The Code, This Is Here Temporarily To Prevent That
            else if (keyState()[SDL_SCANCODE_LSHIFT]) return message;
            else if (std::isalpha((event.key.keysym.scancode - 4) + 'a') && (keyboardTimer.isComplete(10) || (event.key.keysym.scancode - 4) + 'a' != lastKeyboardKey)) {
                if (int keycode = (event.key.keysym.scancode - 4) + 'a'; !(keycode >= 0 && keycode < UCHAR_MAX)) return message;
                char key = (event.key.keysym.scancode - 4) + 'a';
                    if (keyState()[SDL_SCANCODE_LSHIFT]) {
                        message += toupper(key);
                        lastKeyboardKey = toupper(key);
                    } else {
                        message += key;
                        lastKeyboardKey = key;
                    }
                    keyboardTimer.reset();
                    return message;
            }
        }
        return message;
    }
};


#endif //SDL_EVENT_HPP

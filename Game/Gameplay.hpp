//
// Created by Owner on 4/1/2023.
//

#ifndef SDL_GAMEPLAY_HPP
#define SDL_GAMEPLAY_HPP

#include <iostream>
#include <vector>

#include "SDL.h"
#undef main

#include "../Engine/Renderer/Renderer.hpp"
#include "../Engine/Renderer/Animation/Animate.hpp"
#include "../World/World.hpp"
#include "../Item/Item.hpp"

#include <random>
#include <map>

#define GAME_FUNCTION Game::RunGame
namespace Game {
    int RunGame();
    // TODO: Work On Resizable Windows
    int RunResizableWindowTest();
    // TODO: Work On Scriptable Gameplay
    int RunScriptingEngineTest();
}

#endif //SDL_GAMEPLAY_HPP

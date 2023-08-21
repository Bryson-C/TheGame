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
#include "../Engine/Renderer/Scene/World/World.hpp"
#include "../Item/Item.hpp"

#include <random>
#include <map>

#define GAME_FUNCTION Game::RunGame
//#define GAME_FUNCTION Game::WorldDemo

namespace Game {
    // TODO: Work On Resizable Windows
    // TODO: Work On Scriptable Gameplay

    int RunGame(Renderer& renderer);
    int WorldDemo(Renderer& renderer);
    int RunWorldChunkingDemo(Renderer& renderer);
}

#endif //SDL_GAMEPLAY_HPP

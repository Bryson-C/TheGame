
#undef main
// TODO: Textures
// TODO: Text

#include "Game/Gameplay.hpp"

int main() {

    #ifdef GAME_FUNCTION
        GAME_FUNCTION();
    #else
        auto Window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, 0);
        SDL_ShowSimpleMessageBox(0,"Error", "Invalid Application", Window);
    #endif
        return 0;


}

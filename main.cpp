
#undef main
// TODO: Text

#include "Engine/Renderer/Renderer.hpp"
#include "Game/Gameplay.hpp"

#include "Engine/UI/UIComponents.hpp"
auto main() -> int {

    Renderer renderer(800, 600);
/*
    Panel panel{};
    Button button{};
    button.showBorder().setSize(20,10).setPadding(50,50);
    panel.addComponent(button);
    panel.setSize(500,400);
    TextFeild text{};
    text.setPosition(100, 0).setSize(200, 200).showBorder();
    panel.addComponent(text);


    while (renderer.gameLoopIsValid()) {
        renderer.pollEvents();
        renderer.clear();

        panel.draw(renderer);

        renderer.present();
    }
*/

    #ifdef GAME_FUNCTION
        GAME_FUNCTION(renderer);
    #else
        auto Window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, 0);
        SDL_ShowSimpleMessageBox(0,"Error", "Invalid Application, MACRO: GAME_FUNCTION(Renderer& r) = Not Set", renderer.window);
    #endif


    return 0;
}

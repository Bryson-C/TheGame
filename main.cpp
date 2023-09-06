
#undef main
// TODO: Text

#include "Engine/Renderer/Renderer.hpp"
#include "Engine/UI/UI.hpp"
#include "Game/Gameplay.hpp"



auto main() -> int {

    Renderer renderer(800, 600);


    //auto book = renderer.loadTexture(renderer.getPathToSrc().string() + "\\Asset\\Spell_Tome.png");
/*
    while (renderer.gameLoopIsValid()) {
        /*auto event = renderer.pollEvents();
        ImGui_ImplSDL2_ProcessEvent(&event.event);
        if (event.event.type == SDL_QUIT)
            break;
        if (event.event.type == SDL_WINDOWEVENT &&
            event.event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.event.window.windowID == SDL_GetWindowID(renderer.window))
            break;

        renderer.clear();
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (showDemoWindow) {
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        ImGui::Render();
        renderer.present();*/

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
/*
    }
    return 0;
*/


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

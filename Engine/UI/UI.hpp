//
// Created by Owner on 9/5/2023.
//

#ifndef SDL_UI_HPP
#define SDL_UI_HPP

#include "../../lib/ImGui/imgui.h"
#include "../../lib/ImGui/backends/imgui_impl_sdl2.h"
#include "../../lib/ImGui/backends/imgui_impl_sdlrenderer2.h"

#include "../Renderer/Renderer.hpp"


class UI {
    ImVec4 clearColor = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);
    ImGuiIO io;
private:
public:
    // Initialization Functions
    UI(Renderer& renderer) {
        // Ripping This Code From The Example
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForSDLRenderer(renderer.window, renderer.renderer);
        ImGui_ImplSDLRenderer2_Init(renderer.renderer);
    }
    void pollEvents(Event& event, bool& doExit) {
        auto rawEvent = event.event;
        ImGui_ImplSDL2_ProcessEvent(&rawEvent);
        if (rawEvent.type == SDL_QUIT)
            doExit = true;
        /*if (rawEvent.type == SDL_WINDOWEVENT && rawEvent.window.event == SDL_WINDOWEVENT_CLOSE && rawEvent.window.windowID == SDL_GetWindowID(renderer.window))
            doExit = true;*/
    }
    void newFrame() {
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }
    void newWindow(std::string name = "Window", ImGuiWindowFlags flags = 0) { ImGui::Begin(name.c_str(), NULL, flags); }
    void endWindow() { ImGui::End(); }

    // Add Window Element Functions
    enum class ButtonDir {
        ArrowButtonDirLeft = ImGuiDir_Left,
        ArrowButtonDirRight = ImGuiDir_Right,
        ArrowButtonDirUp = ImGuiDir_Up,
        ArrowButtonDirDown = ImGuiDir_Down,
    };
    bool attachButton(std::string text, v2<int> size, std::optional<ButtonDir> buttonDir = {}) {
        ImVec2 fSize = { (float)size.x, (float)size.y };
        if (buttonDir.has_value())
            return ImGui::ArrowButton(text.c_str(), (ImGuiDir)buttonDir.value());
        else
            return ImGui::Button(text.c_str(), fSize);
    }
    void attachText(std::string text, std::optional<RenderDrawColor> color = {}) {
        if (color.has_value()) {
            ImVec4 col = {
                    (float)color->r/255.0f,
                    (float)color->g/255.0f,
                    (float)color->b/255.0f,
                    (float)color->a/255.0f,
            };
            ImGui::TextColored(col, "%s", text.c_str());
        }
        ImGui::Text("%s", text.c_str());
    }
    void attachSlider(std::string text, int* value, int min, int max) {
        ImGui::SliderInt(text.c_str(), value, min, max);
    }
    void attachSlider(std::string text, float* value, float min, float max) {
        ImGui::SliderFloat(text.c_str(), value, min, max);
    }
    v4<float> attachImage(Texture& texture, v2<int> size = {-1,-1}, bool drawFullTexture = false) {
        auto newTexture = (*texture.getGlobalTextures())[texture.textureIndex];

        ImVec2 fSize { (float)size.x, (float)size.y };
        if (size.x <= -1) { fSize.x = newTexture.rect.w; }
        if (size.y <= -1) { fSize.y = newTexture.rect.h; }

        v4<float> uv = {0,0,1,1};
        if (!drawFullTexture) {
            uv = {
                (float) texture.textureX / (float) (newTexture.rect.w),
                (float) texture.textureY / (float) (newTexture.rect.h),
                (float) texture.textureW / (float) newTexture.rect.w,
                (float) texture.textureH / (float) newTexture.rect.h,
            };
        }
        if (uv.x == INFINITY || isnan<float>(uv.x)) uv.x = 0.0f;
        if (uv.y == INFINITY || isnan<float>(uv.y)) uv.y = 0.0f;
        uv.print();
        ImGui::Image(newTexture.texture, fSize, {uv.x, uv.y}, {uv.z, uv.w});
        return uv;
    }

    /*
    void newWindow(Texture& texture) {
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (showDemoWindow)
        //ImGui::ShowDemoWindow(&showDemoWindow);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
            //ImGui::Checkbox("Another Window", &showAnotherWindow);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

            ImVec2 size { 100.0f, 100.0f };
            auto newTexture = (*texture.getGlobalTextures())[texture.textureIndex].texture;
            ImGui::Image(newTexture, size);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
    }
     */
    void draw(Renderer& renderer) {
        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer.renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer.renderer, (Uint8)(clearColor.x * 255), (Uint8)(clearColor.y * 255), (Uint8)(clearColor.z * 255), (Uint8)(clearColor.w * 255));
        //SDL_RenderClear(renderer.renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        //SDL_RenderPresent(renderer.renderer);
    }

};


#endif //SDL_UI_HPP

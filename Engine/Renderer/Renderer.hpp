//
// Created by Owner on 1/9/2023.
//

#ifndef SDL_RENDERER_HPP
#define SDL_RENDERER_HPP

#include <iostream>
#include <vector>
#include <optional>

#include "SDL.h"
#include "SDL_image.h"

#include "Event.hpp"
#include "DrawColor.hpp"
#include "Texture.hpp"
#include "../Utility/Rect/Rect.hpp"
#include "Scene/Scene.hpp"



class Renderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool gameLoopValid;
    bool gameFocused;

    clock_t time;
    // milliseconds / desired framerate
    uint32_t updateTime = 1000/60;
public:
    explicit Renderer(uint32_t width, uint32_t height);
    ~Renderer();

    explicit operator SDL_Renderer*() { return renderer; }

    Event pollEvents();

    inline bool updateGameLoop() {
        if (clock() - time >= updateTime) {
            time = clock();
            return true;
        }
        return false;
    }

    inline SDL_Rect getScreenAsRect() const {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        return {0,0,w,h};
    }

    bool gameLoopIsValid() const;

    struct RectDrawProperties { bool filled = true; };

    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void setColor(RenderDrawColor color);
    RenderDrawColor getColor();

    void clear();
    void present();

    Texture loadTexture(const char* path, int textureX = 0, int textureY = 0, int textureW = -1, int textureH = -1);
    void unloadTexture(Texture& texture);

    // Draw Functions
    template<typename RectType>
    void drawRect(RectType rect, std::optional<RectDrawProperties> props = {}) requires concept_ConvertableToSDLRect<RectType>;
    void drawRect(Rect& r, std::optional<RectDrawProperties> props = {});

    [[deprecated("unused")]] void drawRectArray(std::vector<SDL_Rect>& rects, std::optional<RectDrawProperties> props = {});


    struct TextureDrawProperties {
        float rotation = 0.0f;
        SDL_Point rotationOrigin = {.x = 0, .y = 0};
        SDL_RendererFlip flip = SDL_FLIP_NONE;
    };

    void drawTexture(SDL_Rect pos, Texture& texture, std::optional<TextureDrawProperties> props = {});

};


SDL_Rect getAABBFromSceneView(Rect& r);
SDL_Rect getAABBFromSceneView(SDL_Rect r);
void drawObjectFromSceneView(Renderer& renderer, Rect& r, std::optional<Renderer::RectDrawProperties> props = {});
void drawObjectFromSceneView(Renderer& renderer, SDL_Rect r, std::optional<Renderer::RectDrawProperties> props = {});
void drawObjectFromSceneView(Renderer& renderer, SDL_Rect r, Texture& texture, std::optional<Renderer::TextureDrawProperties> props = {});


#endif //SDL_RENDERER_HPP

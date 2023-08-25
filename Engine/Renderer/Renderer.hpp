//
// Created by Owner on 1/9/2023.
//

#ifndef SDL_RENDERER_HPP
#define SDL_RENDERER_HPP

#define PATH_TO_SRC "X:\\SDL"


#include <iostream>
#include <vector>
#include <optional>

#include "SDL.h"
#include "SDL_image.h"

#ifdef main
#undef main
#endif

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


    friend int main();
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
    void drawLine(v2<int> xy, v2<int> xy2, RenderDrawColor color = {255,0,0});
    void drawLine(v4<int> xy, RenderDrawColor color = {255,0,0});

    template<typename RectType>
    void drawRect(RectType rect, std::optional<RectDrawProperties> props = {}) requires concept_ConvertableToSDLRect<RectType>;
    void drawRect(Rect& r, std::optional<RectDrawProperties> props = {});
    void inline drawRect(int x, int y, int w, int h, std::optional<RectDrawProperties> props = {}) {
        SDL_Rect r {x,y,w,h};
        drawRect(r, props);
    }

    [[deprecated("unused")]] void drawRectArray(std::vector<SDL_Rect>& rects, std::optional<RectDrawProperties> props = {});


    struct TextureDrawProperties {
        float rotation = 0.0f;
        SDL_Point rotationOrigin = {.x = 0, .y = 0};
        SDL_RendererFlip flip = SDL_FLIP_NONE;
    };

    void drawTextureClipped(SDL_Rect pos, SDL_Rect clipArea, Texture& texture, std::optional<TextureDrawProperties> props = {});
    void drawTexture(SDL_Rect pos, Texture& texture, std::optional<TextureDrawProperties> props = {});
};

void drawPercentageBar(Renderer& renderer, SDL_Rect rect, int current, int max);

SDL_Rect getAABBFromSceneView(Rect& r);
SDL_Rect getAABBFromSceneView(SDL_Rect r);
void drawObjectFromSceneView(Renderer& renderer, Rect& r, std::optional<Renderer::RectDrawProperties> props = {});
void drawObjectFromSceneView(Renderer& renderer, SDL_Rect r, std::optional<Renderer::RectDrawProperties> props = {});
void drawObjectFromSceneView(Renderer& renderer, SDL_Rect r, Texture& texture, std::optional<Renderer::TextureDrawProperties> props = {});
void drawLineFromSceneView(Renderer& renderer, v4<int> line);
void drawLineFromSceneView(Renderer& renderer, v2<int> xy, v2<int> xy2);

/*
// TODO: This Works But I Need To Find A Better Way To Make Fonts Compatible
class FontRenderer {
private:
    Texture _FontTexture;
public:
    inline FontRenderer(Renderer& renderer) {
        _FontTexture = renderer.loadTexture("X:\\SDL\\Asset\\Font.png", 10, 3, 310, 19);
    }
    inline void draw(Renderer& renderer) {
        renderer.drawTexture({0,0,700,100}, _FontTexture);
    }
    inline void drawLetter(Renderer& renderer, uint32_t letter) {
        renderer.drawTextureClipped({0, 0, 50, 50}, {((int)letter * 13) + 10, 3, 12, 17}, _FontTexture);
    }
};
*/



class FontRenderer {
private:
    Texture _FontTexture;
    Texture _BBFont;

    inline void drawLetter(Renderer& renderer, uint32_t letter, int yOffset, v4<int> rect) {
        renderer.drawTextureClipped((SDL_Rect)rect, {((int)letter * 16), yOffset, 16, 16}, _FontTexture);
    }
    inline void drawCharacter(Renderer& renderer, uint32_t character, int yOffset, v4<int> rect) {
        renderer.drawTextureClipped((SDL_Rect)rect, {(int)character * 32, yOffset, 32, 32}, _BBFont);
    }
    std::string intToRoman(int num) {
        std::string ones[] = {"","I","II","III","IV","V","VI","VII","VIII","IX"};
        std::string tens[] = {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"};
        std::string hrns[] = {"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"};
        std::string ths[]={"","M","MM","MMM"};

        return ths[num/1000] + hrns[(num%1000)/100] + tens[(num%100)/10] + ones[num%10];
    }

public:
    //inline FontRenderer() = default;
    inline FontRenderer(Renderer& renderer) {
        _FontTexture = renderer.loadTexture(PATH_TO_SRC"\\Asset\\Font.png", 0, 0, 16 * 10, 16 * 4);
        _BBFont = renderer.loadTexture(PATH_TO_SRC"\\Asset\\BBFont.png", 0, 0, 32 * 26, 32 * 2);
    }

    struct FontProperties {
        int8_t kerning;
        int8_t characterSize;
        bool renderAsOutlined;
        FontProperties() : characterSize(16), renderAsOutlined(false) { kerning = -2 * (characterSize/8); };
    };

    inline void drawRomanNumeral(Renderer& renderer, int number, int x, int y, FontProperties props = {}) {
        const auto characterSize = props.characterSize;
        const auto kerning = props.kerning;
        const auto fontOffset = (props.renderAsOutlined) ? 16 : 0;

        for (size_t count = 0; const auto &chr: intToRoman(number)) {
            switch (chr) {
                case 'I': drawLetter(renderer, 0, fontOffset, v4<int>(x + ((count++) * (characterSize + kerning)), y, characterSize, characterSize)); break;
                case 'V': drawLetter(renderer, 1, fontOffset, v4<int>(x + ((count++) * (characterSize + kerning)), y, characterSize, characterSize)); break;
                case 'X': drawLetter(renderer, 2, fontOffset, v4<int>(x + ((count++) * (characterSize + kerning)), y, characterSize, characterSize)); break;
                case 'L': drawLetter(renderer, 3, fontOffset, v4<int>(x + ((count++) * (characterSize + kerning)), y, characterSize, characterSize)); break;
                case 'C': drawLetter(renderer, 4, fontOffset, v4<int>(x + ((count++) * (characterSize + kerning)), y, characterSize, characterSize)); break;
                default: break;
            }
        }
    }
    inline void drawNumber(Renderer& renderer, int number, int x, int y, FontProperties props = {}) {
        const auto characterSize = props.characterSize;
        const auto kerning = props.kerning;
        const auto fontOffset = 32 + ((props.renderAsOutlined) ? 16 : 0);

        for (size_t count = 0; const auto& chr : std::to_string(number)) {
            int digit = chr - '0';
            drawLetter(renderer, digit, fontOffset, v4<int>(x + ((count++) * (characterSize + kerning)), y, characterSize, characterSize));
        }
    }
    inline void drawText(Renderer& renderer, std::string text, int x, int y, FontProperties props = {}) {
        const auto characterSize = props.characterSize;
        const auto kerning = props.kerning;
        for (size_t count = 0; const auto& chr : text) {
            int32_t letterOffset = (chr - (isupper(chr) ? 'A' : 'a')) * 32;
            int32_t fontOffset = (isupper(chr) ? 0 : 32) + (props.renderAsOutlined ? 64 : 0);
            SDL_Rect pos {x + ((characterSize - kerning) * (int)count++), y, characterSize, characterSize};
            if (!isalpha(chr) && !isspace(chr))
                renderer.drawRect(pos);
            else
                renderer.drawTextureClipped(pos, {letterOffset, fontOffset, 32, 32}, _BBFont);
            count++;
        }
    }

};


#endif //SDL_RENDERER_HPP

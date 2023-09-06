//
// Created by Owner on 3/5/2023.
//

#ifndef SDL_TEXTURE_HPP
#define SDL_TEXTURE_HPP

#include <stdint.h>
#include <vector>
#include <string>

#include "SDL.h"

#include "../Utility/Types/Types.hpp"

struct TextureLookUp {
    std::string path;
    bool loaded;
    SDL_Texture* texture;
    SDL_Rect rect;
    uint32_t references;
    void print() const {
        printf("Texture \"%s\":\n\tLoaded: %s\n\tReferences: %u\n", path.c_str(), (loaded) ? "True" : "False", references);
    }
};


static std::vector<TextureLookUp> global_textures;

// A Class That Holds Multiple Textures (Texture Look Ups)
class Texture {
private:
    friend class Renderer;
public:
    size_t textureIndex;
    int32_t textureX, textureY, textureW, textureH;

    Texture() = default;
    inline explicit Texture(size_t index, int x, int y, int w, int h) :
        textureIndex(index), textureX(x),textureY(y), textureW(w), textureH(h) {}

    inline static std::vector<TextureLookUp>* getGlobalTextures() { return &global_textures; }
};

// A Class That Switches Between Textures Based On State
class ConditionalTexture {
private:
public:
    // State Variable Acts As The Index For The Texture Array
    int state;
    // Uses State Variable To Get Index Of Texture Array
    std::vector<size_t> textureArray;
    int32_t textureX, textureY, textureW, textureH;

    ConditionalTexture() = default;
    // TODO: Implement
    inline void addTextureState(int stateValue, size_t textureIndex, v4<int> textureSize) {

    }

    //inline static std::vector<TextureLookUp>* getGlobalTextures() { return &global_textures; }
};

#endif //SDL_TEXTURE_HPP

//
// Created by Owner on 3/5/2023.
//

#ifndef SDL_TEXTURE_HPP
#define SDL_TEXTURE_HPP

#include <stdint.h>
#include <vector>
#include <string>

#include "SDL.h"

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


class Texture {
private:
    friend class Renderer;
public:
    Texture() {}
    inline explicit Texture(size_t index, int x, int y, int w, int h) :
        gTextureIndex(index), textureX(x),textureY(y), textureW(w), textureH(h) {}

    inline static std::vector<TextureLookUp>* getGlobalTextures() { return &global_textures; }

    size_t gTextureIndex;
    int32_t textureX, textureY, textureW, textureH;
};


#endif //SDL_TEXTURE_HPP

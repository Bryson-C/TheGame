//
// Created by Owner on 3/5/2023.
//

#include "Rect.hpp"





Rect::Rect(SDL_Rect rect) : pos(rect), _textures({}), _textureIndex(0), _rot(0.0f), col(), _flip(SDL_FLIP_NONE), _rotOrigin({0,0}) {}
Rect::Rect(int x, int y, int w, int h) { SDL_Rect r {x,y,w,h}; *this = Rect(r); }

void Rect::addTexture(std::initializer_list<Texture> texture) {
    for (const auto& tex : texture)
        _textures.push_back(tex);
}
void Rect::setTexture(Texture texture) {
    if (_textures.empty())
        _textures.resize(1);
    _textures[_textureIndex] = texture;
}
std::vector<Texture> Rect::getTextureList() {
    return _textures;
}
void Rect::flip(SDL_RendererFlip flip) {
    _flip = flip;
}
void Rect::rotate(float rotation) {
    _rot = rotation;
}
void Rect::setRotateOrigin(int32_t x, int32_t y) {
    _rotOrigin = {x,y};
}
void Rect::setTextureIndex(size_t index) {
    _textureIndex = index;
}


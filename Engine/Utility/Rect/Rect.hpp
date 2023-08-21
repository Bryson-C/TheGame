//
// Created by Owner on 3/5/2023.
//

#ifndef SDL_RECT_HPP
#define SDL_RECT_HPP

#include <vector>

#include "SDL.h"
#include "../../Renderer/Texture.hpp"
#include "../../Renderer/DrawColor.hpp"
#include "../Types/Types.hpp"

class Rect {
private:
    std::vector<Texture>    _textures;
    size_t                  _textureIndex;
    float                   _rot;
    SDL_RendererFlip        _flip;
    SDL_Point               _rotOrigin;

    friend class Renderer;
    friend class Item;
    friend class PlayerController;
public:
    SDL_Rect pos;
    RenderDrawColor col;
    Rect() : pos({0, 0, 0, 0}) {}
    explicit Rect(SDL_Rect rect);
    explicit Rect(int x, int y, int w, int h);
    inline void operator=(SDL_Rect rect) { Rect(rect.x, rect.y, rect.w, rect.h); }
    inline explicit operator SDL_Rect() const { return pos; }

    void addTexture(std::initializer_list<Texture> textures);
    void setTexture(Texture texture);
    std::vector<Texture> getTextureList();

    void flip(SDL_RendererFlip flip);
    void rotate(float rotation);
    void setRotateOrigin(int32_t x, int32_t y);
    void setTextureIndex(size_t index);
    inline size_t getTextureIndex() const { return _textureIndex; };

    bool collides(SDL_Rect b) { return SDL_HasIntersection(&pos, &b); }

    void centerOnRect(SDL_Rect b) { pos.x = (b.x + (b.w/2)) - pos.w/2; pos.y = (b.y + (b.h/2)) - pos.h/2; }
    inline Rect getCenterOfRect() const { Rect r {pos.x + (pos.w/2), pos.y + (pos.h/2), 1, 1}; return r; }


};
inline SDL_Rect getCenterOfRect(SDL_Rect pos) { SDL_Rect r {pos.x + (pos.w/2), pos.y + (pos.h/2), 1, 1}; return r; }
inline bool rectsCollide(SDL_Rect a, SDL_Rect b) { return SDL_HasIntersection(&a, &b); }
#endif //SDL_RECT_HPP

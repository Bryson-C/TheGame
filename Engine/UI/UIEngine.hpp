//
// Created by Owner on 5/10/2023.
//

#ifndef SDL_UIENGINE_HPP
#define SDL_UIENGINE_HPP

#include <string>
#include <vector>

#include "../../lib/yaml/Yaml.hpp"
#include "../../Engine/Utility/Rect/Rect.hpp"
#include "../../Engine/Renderer/Renderer.hpp"

class UIEngine {
private:
    std::vector<Rect> _Rects{};
public:
    UIEngine() {}
    inline void draw(Renderer& renderer) {
        for (auto& r : _Rects) {
            renderer.drawRect(r);
        }
    }
    inline void addRect(SDL_Rect rect, RenderDrawColor color) {
        Rect r{rect};
        r.col = color;
        _Rects.push_back(r);
    }
};


#endif //SDL_UIENGINE_HPP

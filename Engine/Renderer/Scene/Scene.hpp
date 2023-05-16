//
// Created by Owner on 5/15/2023.
//

#ifndef SDL_SCENE_HPP
#define SDL_SCENE_HPP

#include "../../Utility/Types/Types.hpp"
#include "../../Utility/Rect/Rect.hpp"


class Scene {
private:
    v2<int32_t> _Offset{0,0};
public:
    static v2<int32_t> getActiveSceneRenderOffset();

    void setActiveScene();
    void moveWorld(int x, int y);
    void moveWorld(v2<int> vec);
    void moveWorldTo(int x, int y);
    void moveWorldTo(v2<int> vec);
};


#endif //SDL_SCENE_HPP

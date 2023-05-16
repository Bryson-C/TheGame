//
// Created by Owner on 5/4/2023.
//

#ifndef SDL_ENEMY_HPP
#define SDL_ENEMY_HPP

#include "../../Engine/Renderer/Renderer.hpp"
#include "../../Engine/Utility/Types/Types.hpp"
#include "../../Engine/Utility/Timer/Timer.hpp"
#include "../Momentum.hpp"

class Enemy {
private:
    Momentum _MomentumCompX{10}, _MomentumCompY{10};

    Rect _Pos;

    Timer _HitTimer{};
    int32_t _IFrames = 100;
    int32_t _MaxHealth = 100;
    int32_t _Health = 100;
public:
    Enemy(int spawnX, int spawnY) : _Pos(spawnX, spawnY - 96, 32, 96) {
            _Pos.col = {255,0,255};
    }
    inline Rect& rect() { return _Pos; }
    inline void update() {
        _MomentumCompX.applyForce(_Pos.pos.x);
        _MomentumCompY.applyForce(_Pos.pos.y);
    }
    inline void draw(Renderer& renderer) {
        auto col = renderer.getColor();
        renderer.setColor(40,40,40);
        drawObjectFromSceneView(renderer, {_Pos.pos.x, _Pos.pos.y - 20, _Pos.pos.w, 10});
        renderer.setColor(255,0,0);
        int32_t healthBarWidth = (_Health > 0) ? ((_Health * _Pos.pos.w) / _MaxHealth) : 0;
        drawObjectFromSceneView(renderer, {_Pos.pos.x, _Pos.pos.y - 20, healthBarWidth, 10});
        drawObjectFromSceneView(renderer, _Pos);
        renderer.setColor(col);
    }
    inline void damage(int32_t dmg) {
        if (_HitTimer.isCompleteReset(_IFrames))
            _Health -= dmg;
    }
    inline void heal(int32_t hp) {
        if (_Health + hp > _MaxHealth) _Health = _MaxHealth;
        else _Health += hp;
    }
    inline Momentum& momentumX() { return _MomentumCompX; }
    inline Momentum& momentumY() { return _MomentumCompY; }
};


#endif //SDL_ENEMY_HPP

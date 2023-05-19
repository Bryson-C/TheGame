//
// Created by Owner on 4/27/2023.
//

#ifndef SDL_PLAYER_HPP
#define SDL_PLAYER_HPP

#include "../../Engine/Renderer/Renderer.hpp"
#include "../../Engine/Utility/Rect/Rect.hpp"
#include "PlayerController.hpp"
#include "../Momentum.hpp"

class Player {
private:
    Rect _PlayerHead;
    Rect _PlayerBody;
    Rect _PlayerLegs;
    Rect _PlayerHitbox;

    bool _PlayerFacingRight = true;
    bool _PlayerCanJump = false;
    Momentum _MomentumX{20}, _MomentumY{20};

    PlayerController _Controller;

    void resetPlayerHitbox() {
        _PlayerHitbox = Rect(
                _PlayerHead.pos.x,
                _PlayerHead.pos.y,
                _PlayerBody.pos.w,
                _PlayerHead.pos.h + _PlayerBody.pos.h + _PlayerLegs.pos.h);
    }
public:
    Player(int spawnX, int spawnY) :
        _PlayerHead(spawnX,spawnY, 32, 32),
        _PlayerBody(spawnX, spawnY + 32, 32, 32),
        _PlayerLegs(spawnX, spawnY + 64, 32, 32),
        _PlayerHitbox({})
    {
        _PlayerHead.col = {0,0,255};
        _PlayerBody.col = {255,0,0};
        _PlayerLegs.col = {0,255,0};
        resetPlayerHitbox();
    }

    // Gets The Area Of The Body, Head, And Legs All Into 1 Rect
    // (Has The Same Rect That Is Used In `.drawOutline`)
    SDL_Rect pos();

    // Gets The Area Of The Body, Head, And Legs All Into 1 Rect
    // (Has The Same Rect That Is Used In `.drawOutline`)
    Rect asRect();

    inline void draw(Renderer& renderer) {
        renderer.drawRect(_PlayerHead);
        renderer.drawRect(_PlayerBody);
        renderer.drawRect(_PlayerLegs);
    }
    inline void drawOutline(Renderer& renderer) {
        Renderer::RectDrawProperties props{};
        Rect outline{asRect()};
        outline.col = {255,255,0};
        renderer.drawRect(outline, {{false}});
    }
    inline SDL_RendererFlip getPlayerFlip() { return (_PlayerFacingRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL; }

    inline void move(int worldOffset[2] ,int x, int y) { worldOffset[0] += x, worldOffset[1] += y; }
    inline void moveTo(int worldOffset[2] ,int x, int y) { worldOffset[0] = x, worldOffset[1] = y; }
    void movePlayerSprite(int x, int y);
    void centerPlayerSprite(Renderer& renderer);

    inline void pollPlayerController(Event& event, Item& heldItem, v2<int,int>& worldOffset) { _Controller.update(event, heldItem, getPlayerFlip(), worldOffset); }
    inline PlayerController& getController() { return _Controller; }

    enum class PlayerDirection {RIGHT, LEFT};
    inline void setPlayerDirect(PlayerDirection dir) { _PlayerFacingRight = (dir == PlayerDirection::RIGHT) ? true : false; }

    void update(Event& event, Scene& scene, std::vector<Rect>& possibleCollisions);
};


#endif //SDL_PLAYER_HPP

//
// Created by Owner on 4/27/2023.
//

#ifndef SDL_PLAYER_HPP
#define SDL_PLAYER_HPP

#include "../../Engine/Renderer/Renderer.hpp"
#include "../../Engine/Utility/Rect/Rect.hpp"
#include "PlayerController.hpp"
#include "../AnimationController.hpp"
#include "../Momentum.hpp"

enum class PlayerMovementState {
    Idle,
    Walking,
    Running,
    CrouchIdle,
    CrouchWalk,
    Jump,
    Land,
    AirFlip,
    Roll,
    WeaponSwing,
    Death,
    Hurt,
    Punch,
    WeaponStab,
};

class Player {
private:
    enum class PlayerDirection {RIGHT, LEFT};

    Rect _PlayerHead;
    Rect _PlayerBody;
    Rect _PlayerLegs;
    Rect _PlayerHitbox;

    bool _PlayerFacingRight = true;
    uint32_t _MaxJumpHeight = 150;
    struct PlayerJumpStatus {
        bool canJump = false;
        uint32_t jumpStage = 0;
        Timer airTime{};
        void reset() {
            canJump = false;
            airTime.reset();
            jumpStage = 0;
        }
    } _PlayerJumpStatus[2];
    struct PlayerRollStatus {
        bool isRolling = false;
        //bool inAir = false;
        uint32_t rollStage = 0;
    } _PlayerRollStatus;

    Momentum _MomentumX{20}, _MomentumY{300};

    PlayerController _Controller;
    AnimationController _AnimationController;

    void AnimatePlayerIdleState() {
        _AnimationController.setAnimationIndex(0);
        constexpr int PLAYER_IDLE_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_IDLE_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerWalkState() {
        _AnimationController.setAnimationIndex(1);
        constexpr int PLAYER_WALK_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_WALK_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerRunState() {
        _AnimationController.setAnimationIndex(2);
        constexpr int PLAYER_RUN_FRAME_TIME = 50;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_RUN_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerCrouchIdleState() {
        _AnimationController.setAnimationIndex(3);
        constexpr int PLAYER_CROUCH_IDLE_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_CROUCH_IDLE_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerCrouchWalkState() {
        _AnimationController.setAnimationIndex(4);
        constexpr int PLAYER_CROUCH_WALK_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_CROUCH_WALK_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerJumpState() {
        _AnimationController.setAnimationIndex(5);
        _AnimationController.setFrame(_PlayerJumpStatus[0].jumpStage);
        // Special State Info Needed
    }
    void AnimatePlayerLandState() {
        _AnimationController.setAnimationIndex(6);
        constexpr int PLAYER_LAND_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_LAND_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerAirFlipState() {
        _AnimationController.setAnimationIndex(7);
        //constexpr int PLAYER_AIR_FLIP_FRAME_TIME = 64;
        _AnimationController.setFrame(_PlayerJumpStatus[1].jumpStage);
    }
    void AnimatePlayerRollState() {
        _AnimationController.setAnimationIndex(8);
        constexpr int PLAYER_ROLL_FRAME_TIME = 80;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_ROLL_FRAME_TIME)) {
            _AnimationController.nextFrame();
            _PlayerRollStatus.rollStage = _AnimationController.getFrame();
        }
    }
    void AnimatePlayerWeaponSwingState() {
        _AnimationController.setAnimationIndex(9);
        constexpr int PLAYER_WEAPON_SWING_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_WEAPON_SWING_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerDieState() {
        _AnimationController.setAnimationIndex(10);
        constexpr int PLAYER_DIE_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_DIE_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerHurtState() {
        _AnimationController.setAnimationIndex(11);
        constexpr int PLAYER_HURT_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_HURT_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerPunchState() {
        _AnimationController.setAnimationIndex(12);
        constexpr int PLAYER_PUNCH_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_PUNCH_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }
    void AnimatePlayerWeaponStabState() {
        _AnimationController.setAnimationIndex(13);
        constexpr int PLAYER_WEAPON_STAB_FRAME_TIME = 64;
        if (_UpdateAnimationFrameTimer.isCompleteReset(PLAYER_WEAPON_STAB_FRAME_TIME)) {
            _AnimationController.nextFrame();
        }
    }



    Timer _UpdateAnimationFrameTimer{};

    void resetPlayerHitbox() {
        _PlayerHitbox = Rect(
                _PlayerHead.pos.x,
                _PlayerHead.pos.y,
                _PlayerBody.pos.w,
                _PlayerHead.pos.h + _PlayerBody.pos.h + _PlayerLegs.pos.h);
    }

    int32_t _PlayerMaxHealth = 100;
    int32_t _PlayerHealth = _PlayerMaxHealth;

    int32_t _playerHeldItemInteropFrame = 0;
    int32_t _playerHeldItemInteropSteps = 10;

    friend void DrawPlayersHeldItem(Renderer& renderer, Player& player, Item& heldItem);
public:
    Player(Renderer& renderer, int spawnX, int spawnY);

    //
    [[deprecated("Remove Or Solve In A Better Way")]] AnimationController getAnimator() const { return _AnimationController; }
    [[deprecated("Remove Or Solve In A Better Way")]] AnimationController& getAnimatorRef() { return _AnimationController; }


    // Gets The Area Of The Body, Head, And Legs All Into 1 Rect
    // (Has The Same Rect That Is Used In `.drawOutline`)
    SDL_Rect pos();

    // Gets The Area Of The Body, Head, And Legs All Into 1 Rect
    // (Has The Same Rect That Is Used In `.drawOutline`)
    Rect asRect();

    void draw(Renderer& renderer);
    inline void drawPlayerUI(Renderer& renderer) {
        auto screen = renderer.getScreenAsRect();
        drawPercentageBar(renderer, {screen.w - 210, 30, 200, 30}, _PlayerHealth, _PlayerMaxHealth);
    }

    inline void drawOutline(Renderer& renderer) {
        Renderer::RectDrawProperties props{};
        Rect outline{asRect()};
        outline.col = {255,255,0};
        renderer.drawRect(outline, {{false}});
    }
    inline SDL_RendererFlip getPlayerFlip() { return (_PlayerFacingRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL; }

    inline void move(v2<int> worldOffset ,int x, int y) { worldOffset.x += x, worldOffset.x += y; }
    inline void moveTo(v2<int> worldOffset ,int x, int y) { worldOffset.y = x, worldOffset.y = y; }
    void movePlayerSprite(int x, int y);
    void centerPlayerSprite(Renderer& renderer);

    inline void pollPlayerController(Event& event, Item& heldItem, v2<int,int>& worldOffset) { _Controller.update(event, heldItem, getPlayerFlip(), worldOffset); }
    inline PlayerController& getController() { return _Controller; }

    //inline void setPlayerDirect(PlayerDirection dir) { _PlayerFacingRight = (dir == PlayerDirection::RIGHT) ? true : false; }

    void update(Event& event, Scene& scene, std::vector<Rect>& possibleCollisions);

    uint32_t getMaxJumpHeight() const { return _MaxJumpHeight; }

#define PLAYER_DEBUG
#ifdef PLAYER_DEBUG
    std::vector<SDL_Rect> CollisionCheckers{};
#endif

};

void DrawPlayersHeldItem(Renderer& renderer, Player& player, Item& heldItem);


#endif //SDL_PLAYER_HPP

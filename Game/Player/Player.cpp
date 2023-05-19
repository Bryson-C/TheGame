//
// Created by Owner on 4/27/2023.
//

#include "Player.hpp"

SDL_Rect Player::pos() { return _PlayerHitbox.pos; }

Rect Player::asRect() { return _PlayerHitbox; }

void Player::movePlayerSprite(int x, int y) {
    _PlayerHead.pos = {x, y, 32, 32};
    _PlayerBody.pos = {x, y + 32, 32, 32};
    _PlayerLegs.pos = {x, y + 64, 32, 32};
    resetPlayerHitbox();
}
void Player::centerPlayerSprite(Renderer& renderer) {
    _PlayerBody.centerOnRect(renderer.getScreenAsRect());
    _PlayerHead.pos = {_PlayerBody.pos.x, _PlayerBody.pos.y - 32, 32, 32};
    _PlayerLegs.pos = {_PlayerBody.pos.x, _PlayerBody.pos.y + 32, 32, 32};
    resetPlayerHitbox();
}

void Player::update(Event& event, Scene& scene, std::vector<Rect>& possibleCollisions) {

    bool playerCanFastFall = true;
    // TODO: Change This To A Integer With Bit Operations To Save Memory
    // TODO: Possibly Add This To An Advanced Version Of The `Rect` Struct
    // Side Note, I Have No Idea Where This Last To-Do Came From, Probably The Chasms Of My Cranium
    bool playerCanMove[4] = {true, true, true, true};
    // If Player Is Colliding With Block

    constexpr int32_t playerCollisionOverlap = 5;

    for (const auto& col : possibleCollisions) {
        if (col.getCenterAsRect().pos.y > asRect().getCenterAsRect().pos.y) {
            if (col.pos.w >= playerCollisionOverlap) playerCanMove[2] = false;
        } else {
            if (col.pos.w >= playerCollisionOverlap) playerCanMove[0] = false;
        }
        if (col.pos.h > col.pos.w && col.pos.h >= playerCollisionOverlap) {
            if (col.getCenterAsRect().pos.x > asRect().getCenterAsRect().pos.x) {
                playerCanMove[1] = false;
            } else {
                playerCanMove[3] = false;
            }
        }
        playerCanFastFall = false;
        _PlayerCanJump = true;
    }

    if (playerCanMove[2]) _MomentumY.pushDontExceed(-3, 3);
        //scene.moveWorld(0, -3);

    auto shiftHeld = event.keyState()[SDL_SCANCODE_LSHIFT];
    if (event.keyState()[SDL_SCANCODE_A] && playerCanMove[3]) {
    //    scene.moveWorld((shiftHeld) ? 11 : 7, 0);
        _MomentumX.pushDontExceed((shiftHeld) ? 11 : 7, 11);
        setPlayerDirect(PlayerDirection::LEFT);
    }
    if (event.keyState()[SDL_SCANCODE_D] && playerCanMove[1]) {
    //    scene.moveWorld((shiftHeld) ? -11 : -7, 0);
        _MomentumX.pushDontExceed((shiftHeld) ? -11 : -7, 11);
        setPlayerDirect(PlayerDirection::RIGHT);
    }
    if (event.keyState()[SDL_SCANCODE_SPACE] && _PlayerCanJump && playerCanMove[0]) {
        _PlayerCanJump = false;
        scene.moveWorld(0, 100);
    }

    scene.moveWorld(_MomentumX.currentMomentum, _MomentumY.currentMomentum);
    _MomentumX.applyForce();
    _MomentumY.applyForce();

}

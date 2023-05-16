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

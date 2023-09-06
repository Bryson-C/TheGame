//
// Created by Owner on 4/27/2023.
//

#include "Player.hpp"

//#define pathToSrc +  "X:\\SDL"

Player::Player(Renderer& renderer, int spawnX, int spawnY) :
        _PlayerHead(spawnX,spawnY, 32, 32),
        _PlayerBody(spawnX, spawnY + 32, 32, 32),
        _PlayerLegs(spawnX, spawnY + 64, 32, 32),
        _PlayerHitbox()
{
    _PlayerHead.col = {0,0,255};
    _PlayerBody.col = {255,0,0};
    _PlayerLegs.col = {0,255,0};
    resetPlayerHitbox();

    const std::string pathToSrc = renderer.getPathToSrc().string();



    std::vector<std::vector<Texture>> playerAnimations {
            {
                // idle
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*0, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*1, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*2, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*3, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*4, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*5, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*6, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*7, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*8, 0, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48*9, 0, 48, 48),
            },
            {
                // walk
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 1, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 1, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 1, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 1, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 1, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 1, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 1, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 1, 48, 48),
            },
            {
                // run
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 2, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 2, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 2, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 2, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 2, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 2, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 2, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 2, 48, 48),
            },
            {
                // crouch idle
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 8, 48 * 3, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 9, 48 * 3, 48, 48),
            },
            {
                // crouch walk
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 8, 48 * 4, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 9, 48 * 4, 48, 48),
            },
            {
                // jump
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 5, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 5, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 5, 48, 48),

            },
            {
                // land
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 6, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 6, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 6, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 6, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 6, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 6, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 6, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 6, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 8, 48 * 6, 48, 48),
            },
            {
                // air flip
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 7, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 7, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 7, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 7, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 7, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 7, 48, 48),
            },
            {
                // roll
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 8, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 8, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 8, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 8, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 8, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 8, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 8, 48, 48),
            },
            {
                // weapon swing
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 9, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 9, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 9, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 9, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 9, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 9, 48, 48),
            },
            {
                // die
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 10, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 10, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 10, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 10, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 10, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 10, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 10, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 10, 48, 48),
            },
            {
                // hit
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 11, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 11, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 11, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 11, 48, 48),
            },
            {
                // punch
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 12, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 12, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 12, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 12, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 12, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 12, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 12, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 12, 48, 48),
            },
            {
                // weapon stab
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 13, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 13, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 13, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 13, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 13, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 13, 48, 48),
                    renderer.loadTexture(pathToSrc + "\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 13, 48, 48),
            }
    };

    _AnimationController.setAnimationArray(playerAnimations);
}



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

// TODO: Update Player Collision Code
// TODO: Add Checks For Collisions For Movement Other Than Walking/Running, i.e Rolling/Punching
void Player::update(Event& event, Scene& scene, World& world) {

    bool playerCanFastFall = true;
    _PlayerJumpStatus[0].canJump = false;
    // TODO: Change This To A Integer With Bit Operations To Save Memory
    // TODO: Possibly Add This To An Advanced Version Of The `Rect` Struct
    // Side Note, I Have No Idea Where This Last To-Do Came From, Probably The Chasms Of My Cranium
    bool playerCanMove[4] = {true, true, true, true};
    // If Player Is Colliding With Block

    int32_t maxJumpHeight = 150;

#ifdef PLAYER_DEBUG
    CollisionCheckers.clear();
#endif

    SDL_Rect wallCol = _PlayerBody.pos, wallResult;
    wallCol.x -= 2 + abs(_MomentumX.currentMomentum);
    wallCol.w += 4 + (abs(_MomentumX.currentMomentum) * 2);
    wallCol.h = (_PlayerBody.pos.h * 3) - 16;
    wallCol.y = _PlayerHead.pos.y + 8;

    SDL_Rect jumpCastResult;
    const int gravityLimit = 4;
    auto playerPos = pos();
    SDL_Rect jumpRayCast = {playerPos.x + 2, playerPos.y - maxJumpHeight, playerPos.w - 4, maxJumpHeight};
    const uint32_t blockRange = 32;
    // Checking All Collisions Besides Upwards And Jumping Collisions
    for (const auto& block : world.getBlocks()) {
        SDL_Rect collisionOverlaps;
        if (SDL_IntersectRect(&playerPos, &block.first.pos, &collisionOverlaps) && block.second != BlockType::None) {
            CollisionCheckers.push_back(block.first.pos);
            // Ground collisions
            if (playerPos.y + playerPos.h + gravityLimit > block.first.pos.y && getCenterOfRect(block.first.pos).y > getCenterOfRect(playerPos).y) {
                playerCanMove[2] = false;
                _PlayerJumpStatus[0].canJump = true;
                _MomentumY = 0;
                if (collisionOverlaps.h > 1)
                    _MomentumY.pushDontExceed(1, 1);
            }

            if (collisionOverlaps.h > 1) {
                // Right Collision
                if (playerPos.x + playerPos.w + _MomentumX.currentMomentum < block.first.pos.x + (block.first.pos.w/2)) {
                    playerCanMove[1] = false;
                    _MomentumX = 0;
                    if (collisionOverlaps.w > 1)
                        _MomentumX.pushDontExceed(1, 1);
                }
                // Left Collision
                if (playerPos.x - _MomentumX.currentMomentum > block.first.pos.x - (block.first.pos.w/2)) {
                    playerCanMove[3] = false;
                    _MomentumX = 0;
                    if (collisionOverlaps.w > 1)
                        _MomentumX.pushDontExceed(-1, 1);
                }
            }
        }
    }

    // Checking Jumping Collisions
    for (const auto& block : world.getBlocks()) {
        if (block.second != BlockType::None && rectsCollide(jumpRayCast, block.first.pos)) {
            if (playerPos.y + gravityLimit > block.first.pos.y + (block.first.pos.h - block.first.pos.h/2)) {
                maxJumpHeight = playerPos.y - (block.first.pos.y + block.first.pos.h);
                jumpRayCast = {playerPos.x, playerPos.y - maxJumpHeight, playerPos.w, maxJumpHeight};
                printf("JumpHeight: %i\n", maxJumpHeight);
                playerCanMove[0] = false;
            }
        }
    }

    CollisionCheckers.push_back(jumpRayCast);

/*
    for (const auto& col : possibleCollisions) {
        // Horizontal Collisions
        if (SDL_IntersectRect(&wallCol, &col.pos, &wallResult)) {
            // Check If Collision Is On The Right Side
            if ((wallResult.x) > getCenterOfRect(wallCol).x) { playerCanMove[1] = false; }
            // Check If Collision Is On The Left Side
            if ((wallResult.x + wallResult.w) < getCenterOfRect(wallCol).x) { playerCanMove[3] = false; }
            if (wallResult.w > 1) {
                _MomentumX = 0;
                // check if collision is on the right side
                _MomentumX.pushDontExceed(((wallResult.x) > getCenterOfRect(wallCol).x) ? 1 : -1, 1);
            }
        }

        // Vertical Collisions
        if (SDL_IntersectRect(&jumpRayCast, &col.pos, &jumpCastResult)) {
            maxJumpHeight = groundCol.y - (jumpCastResult.y + jumpCastResult.h);
//            printf("Max Jump Height: %i\n", maxJumpHeight);
        }
        if (SDL_IntersectRect(&groundCol, &col.pos, &resultCol)) {
            if (resultCol.y < getCenterOfRect(groundCol).y) {
                playerCanMove[0] = false;
                _PlayerJumpStatus[0].canJump = true;
                _MomentumY = 0;
            } else if (resultCol.y > getCenterOfRect(groundCol).y && wallResult.w > 3) {
                playerCanMove[2] = false;
                _MomentumY = 0;
                if (resultCol.w > 0) {
                    _PlayerJumpStatus[0].canJump = true;
                }
            }
        }

#ifdef PLAYER_DEBUG

        CollisionCheckers.push_back(wallCol);
        CollisionCheckers.push_back(groundCol);
        CollisionCheckers.push_back(jumpRayCast);
        CollisionCheckers.push_back(resultCol);
        //CollisionCheckers.push_back(col.pos);
#endif
    }
*/

    // Apply Gravity
    // Any Gravity Higher Than 5 Makes The Horizontal Collider Collide With The Ground
    if (playerCanMove[2]) _MomentumY.pushDontExceed(-2, gravityLimit);


    auto shiftHeld = event.keyState()[SDL_SCANCODE_LSHIFT];
    auto aHeld = event.keyState()[SDL_SCANCODE_A];
    auto dHeld = event.keyState()[SDL_SCANCODE_D];
    int32_t timeTillNextFrame = 64;

    if (aHeld && playerCanMove[3] && !_PlayerRollStatus.isActive) {
        int movement = (shiftHeld) ? 11 : 6;
        if (!_PlayerPunchStatus.isActive) //movement /= (shiftHeld) ? 2 :4;
            _MomentumX.pushDontExceed(movement, movement);
        _PlayerFacingRight = false;
    }
    else if (dHeld && playerCanMove[1] && !_PlayerRollStatus.isActive) {
        int movement = (shiftHeld) ? 11 : 6;
        if (!_PlayerPunchStatus.isActive) //movement /= (shiftHeld) ? 2 :4;
            _MomentumX.pushDontExceed(-movement, movement);
        _PlayerFacingRight = true;
    }
    if (event.keyState()[SDL_SCANCODE_LALT] && !_PlayerRollStatus.isActive && _PlayerJumpStatus[0].canJump) {
        _PlayerRollStatus.isActive = true;
        _PlayerRollStatus.stage = 0;
    }
    if (event.keyState()[SDL_SCANCODE_SPACE] && playerCanMove[0] && !_PlayerRollStatus.isActive) {
        _PlayerWeaponSwingStatus.isActive = false;
        _PlayerWeaponSwingStatus.stage = 0;
        if (_PlayerJumpStatus[0].canJump) {
            _PlayerJumpStatus[0].reset();
            _PlayerJumpStatus[1].canJump = true;
            printf("Max Jump Height: %i, (Originally 150)\n", -maxJumpHeight);
            _MomentumY.push(maxJumpHeight);
        } else if (_PlayerJumpStatus[1].canJump && _PlayerJumpStatus[0].jumpStage >= 2) {
            printf("Double Jump\n");
            _PlayerJumpStatus[1].reset();
            _MomentumY.push(maxJumpHeight);
        }
    }
    if (event.mouseDownL() && _PlayerJumpStatus[0].canJump) {
        if (_Controller.playerInCombatStance()) {
            if (event.keyState()[SDL_SCANCODE_S]) {
                _PlayerWeaponStabStatus.isActive = true;
            } else {
                _PlayerWeaponSwingStatus.isActive = true;
            }
        } else {
            _PlayerPunchStatus.isActive = true;
        }
    }



    if (_PlayerPunchStatus.isActive) {
        timeTillNextFrame = 80;
        AnimatePlayerPunchState();
        if (event.mouseDownL()) {
            if (!aHeld && !dHeld) {
                _PlayerPunchStatus.stage = 1;
                _AnimationController.setFrame(1);
            } else if (aHeld || dHeld) {
                _PlayerPunchStatus.stage = 3;
                _AnimationController.setFrame(3);
            }
        } else {
            if (_UpdateAnimationFrameTimer.isCompleteReset(timeTillNextFrame)) {
                _AnimationController.nextFrame();
                _PlayerPunchStatus.stage = _AnimationController.getFrame();
            }
        }
        if (_PlayerPunchStatus.stage == 0) {
            _PlayerPunchStatus.isActive = false;
            _PlayerPunchStatus.stage = 0;
        }
        else if (_PlayerPunchStatus.stage == 4) _MomentumX.pushDontExceed((_PlayerFacingRight) ? -5 : 5, (shiftHeld) ? 11 : 6);
    }
    if (_PlayerWeaponStabStatus.isActive) {
        timeTillNextFrame = 64;
        if (aHeld || dHeld) {
            uint32_t stage = (shiftHeld) ? 3 : 2;
            _PlayerWeaponSwingStatus.stage = stage;
            _AnimationController.setFrame(stage);
        }
        AnimatePlayerWeaponStabState();
        printf("Stab\n");
        if (_PlayerWeaponStabStatus.stage >= 6) {
            _PlayerWeaponStabStatus.isActive = false;
            _PlayerWeaponStabStatus.stage = 0;
        }
    }
    if (_PlayerWeaponSwingStatus.isActive) {
        timeTillNextFrame = 64;
        AnimatePlayerState(PlayerControllerState::WeaponSwing, timeTillNextFrame, &_PlayerWeaponSwingStatus);
        if (_PlayerWeaponSwingStatus.stage >= 5) {
            _PlayerWeaponSwingStatus.isActive = false;
            _PlayerWeaponSwingStatus.stage = 0;
        }
    }
    if (_PlayerRollStatus.isActive) {
        timeTillNextFrame = 80;
        AnimatePlayerState(PlayerControllerState::Roll, timeTillNextFrame, &_PlayerRollStatus);
        if (_PlayerRollStatus.stage < 6) {
            _MomentumX.pushDontExceed((_PlayerFacingRight) ? -11 : 11, 11);
        } else {
            _PlayerRollStatus.isActive = false;
            _PlayerRollStatus.stage = 0;
        }
    }
    else if (!_PlayerJumpStatus[0].canJump) {
        if (!_PlayerJumpStatus[1].canJump) {
            timeTillNextFrame = 80;
            AnimatePlayerStateDontRepeat(PlayerControllerState::AirFlip, timeTillNextFrame);
        } else {
            timeTillNextFrame = 120;
            PlayerActionStatus jumpStatus;
            AnimatePlayerStateDontRepeat(PlayerControllerState::Jump, timeTillNextFrame, &jumpStatus);
            _PlayerJumpStatus[0].jumpStage = jumpStatus.stage;
        }
    } else {
        if (!_PlayerPunchStatus.isActive && !_PlayerWeaponSwingStatus.isActive && !_PlayerWeaponStabStatus.isActive) {
            if ((aHeld || dHeld)) {
                // Running
                if (shiftHeld) {
                    timeTillNextFrame = 50;
                    AnimatePlayerState(PlayerControllerState::Running, timeTillNextFrame);
                } else {
                    // Walking
                    AnimatePlayerState(PlayerControllerState::Walking, timeTillNextFrame);
                }
            } else {
                // Idle
                if (!_PlayerPunchStatus.isActive) {
                    AnimatePlayerState(PlayerControllerState::Idle, timeTillNextFrame);
                }
            }
        }
    }

    scene.moveWorld(_MomentumX.currentMomentum, _MomentumY.currentMomentum);
    _MomentumX.applyForce();
    _MomentumY.applyForce();


    //_playerHeldItemInteropNeedsUpdate = _UpdateAnimationFrameTimer.getTime() > ((timeTillNextFrame * _playerHeldItemInteropFrame) / _playerHeldItemInteropSteps);
    _playerHeldItemInteropFrame = _UpdateAnimationFrameTimer.getTime() / (timeTillNextFrame / _playerHeldItemInteropSteps);
    if (_playerHeldItemInteropFrame > _playerHeldItemInteropSteps) _playerHeldItemInteropFrame = 0;
}


void Player::draw(Renderer &renderer) {
    // Old Way Of Doing It, I Still Need To Split The Test Animations So I Can Have More Control Over The Animations
    //renderer.drawRect(_PlayerHead);
    //renderer.drawRect(_PlayerBody);
    //renderer.drawRect(_PlayerLegs);
    Renderer::TextureDrawProperties props;
    props.flip = (_PlayerFacingRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    props.rotationOrigin = {0,0};
    props.rotation = 0.0f;
    SDL_Rect position = {
            pos().x - 48,
            pos().y - 48,
            _AnimationController.getFrameTexture().textureW * 3,
            _AnimationController.getFrameTexture().textureH * 3
    };
    _AnimationController.drawFrame(renderer, position, props);

#ifdef PLAYER_DEBUG
    auto col = renderer.getColor();
    renderer.setColor(255,0,0);
    for (const auto& check : CollisionCheckers) {
        renderer.drawRect(check, {{.filled = false}});
    }
    renderer.setColor(col);
#endif

}

int32_t updateInterop(int32_t from, int32_t to, int32_t frame, int32_t steps) {
    return  from + ((to - from) / steps) * frame;
}

void DrawPlayersHeldItem(Renderer& renderer, Player& player, Item& heldItem) {
        if (heldItem.type() != ItemSpawnList::ItemType::Tool &&
            heldItem.type() != ItemSpawnList::ItemType::Weapon &&
            heldItem.type() != ItemSpawnList::ItemType::MagicalItem) return;

        enum PlayerAnimationState {
            Idle,
            Walk,
            Run,
            CrouchIdle,
            CrouchWalk,
            Jump,
            Land,
            AirFlip,
            Roll,
            WeaponSwing,
            Die,
            Hit,
            Punch,
            WeaponStab
        };

        // v3 { x offset, y offset, rotation }
        std::vector<v3<int>> gripPoints {};
        PlayerAnimationState playerAnimationSet = (PlayerAnimationState)player.getAnimator().getAnimationSet();
        uint32_t animationFrame = player.getAnimator().getFrame();

        switch (playerAnimationSet) {
            case Idle:
                gripPoints = {
                        {-6, 0, 40},
                        {-5, 3, 40},
                        {-3, 6, 40},
                        {-3, 6, 40},
                        {-3, 6, 40},
                        {-4, 1, 40},
                        {-4, -1, 40},
                        {-3, -5, 40},
                        {-3, -5, 40},
                        {-3, -5, 40},
                };
                break;
            case Walk:
                gripPoints = {
                        {-6, 3, 40},
                        {21, 1, 40},
                        {27, 2, 25},
                        {27, 7, 15},
                        {15, 5, 25},
                        {-3, 0, 40},
                        {-20, -2, 55},
                        {-18, -3, 60},
                };
                break;
            case Run:
                gripPoints = {
                        {-12, -5, 40},
                        {-11, -28, 80},
                        {-14, -10, 55},
                        {7, 3, 15},
                        {35, 6, -15},
                        {52, -28, -100},
                        {49, -7, -55},
                        {19, 8, -25},
                };
                break;
            case CrouchIdle:
                gripPoints = {
                        {22, -1, 0},
                        {24, 5, 0},
                        {24, 7, 0},
                        {24, 7, 0},
                        {24, 7, 0},
                        {24, 7, 0},
                        {25, 4, 0},
                        {25, -1, 0},
                        {25, -1, 0},
                        {25, -1, 0},
                };
                break;
            case CrouchWalk:
                gripPoints = {
                        {22, -1, 0},
                        {22, -1, 0},
                        {23, 1, 0},
                        {23, 1, 0},
                        {23, 1, 0},
                        {22, 1, 0},
                        {22, 1, 0},
                        {28, 3, 0},
                        {27, 0, 0},
                        {23, 0, 0},
                };
                break;
            case Jump:
                gripPoints = {
                        {-9, -7, 25},
                        {-22, -24, 55},
                        {-9, -33, 90},
                };
                break;
            case Land:
                gripPoints = {
                        {15, -70, 0},
                        {38, -36, 0},
                        {14, -20, 0},
                        {-15, -30, 0},
                        {-22, -31, 0},
                        {-19, -40, 0},
                        {-41, -66, 0},
                };
                break;
            case AirFlip:
                gripPoints = {
                        {13, -45, -190},
                        {35, -11, -80},
                        {-6, -2, 10},
                        {-35, -22, 80},
                        {-35, -22, 100},
                        {-23, -28, 105},
                };
                break;
            case Roll:
                gripPoints = {
                        {-8, -2, 40},
                        {33, -7, 50},
                        {17, 21, 70},
                        {4, 20, 110},
                        {-28, 23, 185},
                        {-19, 3, 245},
                        {-18, 12, 360},
                };
                break;
            case WeaponSwing:
                gripPoints = {
                        {-9, -43, -180},
                        {23, -62, -155},
                        {59, -45, -125},
                        {32, 7, 65},
                        {27, 19, 55},
                        {12, -10, 95},
                }; break;
            case Die:
                gripPoints = {
                        {-4, 15, 45},
                        {-1, 19, 45},
                        {2, 29, 45},
                        {2, 29, 45},
                        {2, 29, 45},
                        {2, 29, 45},
                        {2, 29, 45},
                        {2, 29, 45},
                }; break;
            case Hit:
                gripPoints = {
                        {2, -4, 0},
                        {-16, -6, 25},
                        {-17, -12, 35},
                        {-13, -8, 45},
                }; break;
            case Punch:
                gripPoints = {
                        {-9, 2, -50},
                        {-8, 9, -50},
                        {-5, 7, -50},
                        {-6, 6, -50},
                        {60, -17, 40},
                        {41, -4, 70},
                        {33, -1, 60},
                        {21, 0, 15},
                }; break;
            case WeaponStab:
                gripPoints = {
                        {-22, -2, 45},
                        {-22, -2, 45},
                        {-12, 0, 45},
                        {-25, 4, 45},
                        {-25, 4, 45},
                        {68, -12, 45},
                        {63, -11, 45},
                        {31, -4, 45},
                }; break;
            default:
                break;
        }


        int32_t weaponSize = 32;
        int32_t weaponScale = 2;
        auto playerCenter = getCenterOfRect(player.pos());
        SDL_Rect weaponPos = {playerCenter.x + ((player.getPlayerFlip() == SDL_FLIP_HORIZONTAL) ? -30 : -10), player.pos().y, weaponSize * weaponScale, weaponSize * weaponScale};
        Renderer::TextureDrawProperties weaponProps;
        weaponProps.rotation = 0.0f;
        weaponProps.flip = player.getPlayerFlip();
        weaponProps.rotationOrigin = {(weaponProps.flip == SDL_FLIP_NONE) ? 0 : weaponSize * weaponScale, weaponSize * weaponScale};

        if (player.getController().playerInCombatStance()) {
            weaponPos.x += gripPoints[animationFrame].x * ((weaponProps.flip == SDL_FLIP_NONE) ? 1 : -1);

// Interpolation Code For The Weapons, Chnage `previosFrame` To `nextFrame`
/*
        size_t previousFrame = (animationFrame > 0) ? animationFrame - 1 : 0;

        int distanceX = gripPoints[animationFrame].x - gripPoints[previousFrame].x;
        int interpDistX = distanceX - ((distanceX / player._playerHeldItemInteropSteps) * player._playerHeldItemInteropFrame);

        int distanceY = gripPoints[animationFrame].y - gripPoints[previousFrame].y;
        int interpDistY = distanceY - ((distanceY / player._playerHeldItemInteropSteps) * player._playerHeldItemInteropFrame);

        int distanceZ = gripPoints[animationFrame].z - gripPoints[previousFrame].z;
        int interpDistZ = distanceZ - ((distanceZ / player._playerHeldItemInteropSteps) * player._playerHeldItemInteropFrame);
*/
        weaponPos.y += gripPoints[animationFrame].y;
        weaponProps.rotation = gripPoints[animationFrame].z * ((weaponProps.flip == SDL_FLIP_NONE) ? 1.0f : -1.0f);
/*
        weaponPos.x += interpDistX;
        weaponPos.y += interpDistY;
        weaponProps.rotation += interpDistZ;
*/
    } else {
        if (playerAnimationSet == Idle) {
            weaponPos.y += gripPoints[animationFrame].y;
            weaponProps.rotation += gripPoints[animationFrame].z;
        }
        else if (playerAnimationSet == Walk || playerAnimationSet == Run) {
            weaponProps.rotation += (float) sin(clock() / ((playerAnimationSet == Run) ? 50 : 100)) * ((playerAnimationSet == Run) ? 5.0f : 2.0f);
        }
        weaponProps.rotation = (weaponProps.flip == SDL_FLIP_HORIZONTAL) ? -150.0f : 150.0f;
        weaponPos.y -= weaponSize * weaponScale;
    }

    heldItem.draw(renderer, weaponPos, weaponProps);

}

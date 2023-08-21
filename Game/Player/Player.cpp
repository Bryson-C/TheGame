//
// Created by Owner on 4/27/2023.
//

#include "Player.hpp"

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


    std::vector<std::vector<Texture>> playerAnimations {
            {
                // idle
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*0, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*1, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*2, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*3, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*4, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*5, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*6, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*7, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*8, 0, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48*9, 0, 48, 48),
            },
            {
                // walk
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 1, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 1, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 1, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 1, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 1, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 1, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 1, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 1, 48, 48),
            },
            {
                // run
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 2, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 2, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 2, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 2, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 2, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 2, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 2, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 2, 48, 48),
            },
            {
                // crouch idle
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 8, 48 * 3, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 9, 48 * 3, 48, 48),
            },
            {
                // crouch walk
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 8, 48 * 4, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 9, 48 * 4, 48, 48),
            },
            {
                // jump
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 5, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 5, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 5, 48, 48),

            },
            {
                // land
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 6, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 6, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 6, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 6, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 6, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 6, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 6, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 6, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 8, 48 * 6, 48, 48),
            },
            {
                // air flip
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 7, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 7, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 7, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 7, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 7, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 7, 48, 48),
            },
            {
                // roll
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 8, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 8, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 8, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 8, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 8, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 8, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 8, 48, 48),
            },
            {
                // weapon swing
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 9, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 9, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 9, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 9, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 9, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 9, 48, 48),
            },
            {
                // die
            },
            {
                // hit
            },
            {
                // punch
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 12, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 12, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 12, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 12, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 12, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 12, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 12, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 7, 48 * 12, 48, 48),
            },
            {
                // weapon stab
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 0, 48 * 13, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 1, 48 * 13, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 2, 48 * 13, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 3, 48 * 13, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 4, 48 * 13, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 5, 48 * 13, 48, 48),
                    renderer.loadTexture("X:\\SDL\\Asset\\SpriteSheets\\all.png", 48 * 6, 48 * 13, 48, 48),
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
void Player::update(Event& event, Scene& scene, std::vector<Rect>& possibleCollisions) {

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

    SDL_Rect groundCol = _PlayerLegs.pos, resultCol;
    SDL_Rect jumpRayCast = {pos().x, pos().y - maxJumpHeight, pos().w, maxJumpHeight};
    SDL_Rect jumpCastResult;
    /*
    // TODO: Find Way To Make The Jump Raycast Better
    jumpRayCast.y -= maxJumpHeight - (maxJumpHeight - pos().h);
    jumpRayCast.w -= 6;
    jumpRayCast.x += 3;*/

    const int gravityLimit = 4;
    groundCol.h += _MomentumY.currentMomentum + gravityLimit;

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

    // Apply Gravity
    // Any Gravity Higher Than 5 Makes The Horizontal Collider Collide With The Ground
    if (playerCanMove[2]) _MomentumY.pushDontExceed(-2, 4);


    auto shiftHeld = event.keyState()[SDL_SCANCODE_LSHIFT];
    auto aHeld = event.keyState()[SDL_SCANCODE_A];
    auto dHeld = event.keyState()[SDL_SCANCODE_D];
    if (aHeld && playerCanMove[3] && !_PlayerRollStatus.isRolling) {
        _MomentumX.pushDontExceed((shiftHeld) ? 11 : 6, (shiftHeld) ? 11 : 6);
        _PlayerFacingRight = false;
    }
    else if (dHeld && playerCanMove[1] && !_PlayerRollStatus.isRolling) {
        _MomentumX.pushDontExceed((shiftHeld) ? -11 : -6, (shiftHeld) ? 11 : 6);
        _PlayerFacingRight = true;
    }
    if (event.keyState()[SDL_SCANCODE_LALT] && !_PlayerRollStatus.isRolling && _PlayerJumpStatus[0].canJump) {
        _PlayerRollStatus.isRolling = true;
    }
    if (event.keyState()[SDL_SCANCODE_SPACE] && playerCanMove[0] && !_PlayerRollStatus.isRolling) {
        if (_PlayerJumpStatus[0].canJump) {
            _PlayerJumpStatus[0].reset();
            _PlayerJumpStatus[1].canJump = true;
            printf("Max Jump Height: %i, (Originally 150)\n", -maxJumpHeight);
            /*if (maxJumpHeight < 150) {
                //scene.moveWorld(0, maxJumpHeight - 10);
                move(scene.getSceneOffset(), 0, -maxJumpHeight);
            } else*/
            _MomentumY.push(maxJumpHeight);
        } else if (_PlayerJumpStatus[1].canJump && _PlayerJumpStatus[0].jumpStage == 2) {
            printf("Double Jump\n");
            _PlayerJumpStatus[1].reset();
            _MomentumY.push(maxJumpHeight);
        }
    }


    scene.moveWorld(_MomentumX.currentMomentum, _MomentumY.currentMomentum);
    _MomentumX.applyForce();
    _MomentumY.applyForce();

    uint32_t timeTillNextFrame;

    if (_PlayerRollStatus.isRolling) {
        timeTillNextFrame = 50;
        AnimatePlayerRollState();
        if (_PlayerRollStatus.rollStage >= 6) {
            _PlayerRollStatus.isRolling = false;
            _PlayerRollStatus.rollStage = 0;
        } else {
            _MomentumX.pushDontExceed((_PlayerFacingRight) ? -11 : 11, 11);
        }
    }
    else if (!_PlayerJumpStatus[0].canJump) {
        if (!_PlayerJumpStatus[1].canJump) {
            timeTillNextFrame = 80;
            if (_UpdateAnimationFrameTimer.isCompleteReset(timeTillNextFrame) && _PlayerJumpStatus[1].jumpStage < 5) {
                _PlayerJumpStatus[1].jumpStage++;
            }
            AnimatePlayerAirFlipState();
        } else {

            timeTillNextFrame = 100;
            // Jumping
            if (_PlayerJumpStatus[0].airTime.getTime() > 200 * (maxJumpHeight / 100))
                _PlayerJumpStatus[0].jumpStage = 1;
            if (_PlayerJumpStatus[0].airTime.getTime() > 300 * (maxJumpHeight / 100))
                _PlayerJumpStatus[0].jumpStage = 2;

            AnimatePlayerJumpState();
        }
    } else {
        if ((aHeld || dHeld)) {
            // Running
            if (shiftHeld) {
                timeTillNextFrame = 50;
                AnimatePlayerRunState();
            } else {
                // Walking
                timeTillNextFrame = 64;
                AnimatePlayerWalkState();
            }
        } else {
            // Idle
            timeTillNextFrame = 64;
            AnimatePlayerIdleState();
        }
    }

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
                    {},
            };
            break;
        case Roll:
            gripPoints = {
                    {},
            };
            break;
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

    // TODO: Use Interpolation Between Frames For Grip Points And Rotation
    if (player.getController().playerInCombatStance()) {
        weaponPos.x += gripPoints[animationFrame].x * ((weaponProps.flip == SDL_FLIP_NONE) ? 1 : -1);

/*        weaponPos.x += updateInterop(gripPoints[animationFrame].x, gripPoints[nextIndex].x,
                                                   player._playerHeldItemInteropFrame,
                                                   player._playerHeldItemInteropSteps);
        weaponProps.rotation += updateInterop(gripPoints[animationFrame].z, gripPoints[nextIndex].z,
                                                   player._playerHeldItemInteropFrame,
                                                   player._playerHeldItemInteropSteps); */
        // TODO: I Have Reason To Believe This Peice Of Code Is Accessing Elements At Higher Indices Then What Should Be Possible,
        // Thereby Creating Out-Of-Bounds Errors
        //int32_t nextIndex = (animationFrame + 1 < gripPoints.size() - 1) ? (animationFrame + 1) : (gripPoints.size() - 1);

        weaponPos.y += gripPoints[animationFrame].y;
        weaponProps.rotation = gripPoints[animationFrame].z * ((weaponProps.flip == SDL_FLIP_NONE) ? 1.0f : -1.0f);
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



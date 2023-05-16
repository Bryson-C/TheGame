//
// Created by Owner on 4/1/2023.
//

#include "Gameplay.hpp"
#include "Player/Player.hpp"
#include "Enemy/Enemy.hpp"
#include "../Item/Inventory.hpp"
#include "../Engine/UI/UIEngine.hpp"

int Game::RunGame() {

    Renderer renderer(800, 600);

    bool playerCanJump = false, playerCanFastFall = false;
    Player player(0,0);
    player.centerPlayerSprite(renderer);

    Enemy enemy(0,0);

    // Load World Sprite Sheet
    Texture tileTextures[] = {
            renderer.loadTexture(R"(X:\SDL\Asset\Tileset64.png)", 0, 0, 64, 64),
            renderer.loadTexture(R"(X:\SDL\Asset\Tileset64.png)", 64, 0, 64, 64),
            renderer.loadTexture(R"(X:\SDL\Asset\Tileset64.png)", 128, 0, 64, 64),
    };

    // generate world

    Scene scene;
    scene.setActiveScene();

    World world(100, 10, 32, 32);
    v2<int32_t> worldOffset = {200, player.pos().y + player.pos().h};
    world.offsetWorld(worldOffset);
    world.addWorldTextureForBlockType(World::BlockType::None, tileTextures[0]);
    world.addWorldTextureForBlockType(World::BlockType::Grass, tileTextures[0]);
    world.addWorldTextureForBlockType(World::BlockType::Dirt, tileTextures[1]);
    world.addWorldTextureForBlockType(World::BlockType::Stone, tileTextures[2]);
    world.updateWorldTextures();

    size_t hotbarIndex = 0;
    size_t hotbarSize = 10;
    Timer hotbarTimer{};

    Inventory playerInventory(renderer, 10, 1);
    playerInventory.moveCursor(0,0);
    playerInventory.setSlot(0,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::TestSword, renderer));
    playerInventory.setSlot(1,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::GrassBlock, renderer));
    playerInventory.setSlot(2,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::DirtBlock, renderer));
    playerInventory.setSlot(3,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::StoneBlock, renderer));

    Timer enemyPushTimer{};


    while (renderer.gameLoopIsValid()) {
        // Get Inputs
        Event event = renderer.pollEvents();
        if (renderer.updateGameLoop()) {

            if (event.keyState()[SDL_SCANCODE_G] && enemyPushTimer.isComplete(100)) {
                enemy.momentumY().push(-10);
                enemyPushTimer.reset();
                enemy.damage(10);
            }
            if (getAABBFromSceneView(enemy.rect().pos).y + enemy.rect().pos.h < renderer.getScreenAsRect().h/2)
                enemy.momentumY().push(1);


            if (event.keyState()[SDL_SCANCODE_RIGHT]) {
                if (hotbarIndex < hotbarSize-1 && hotbarTimer.isCompleteReset(150)) {
                    hotbarIndex++;
                    playerInventory.moveCursor(hotbarIndex, 0);
                    player.getController().validateCombatStance(playerInventory.getSlot(hotbarIndex));
                }
            } else if (event.keyState()[SDL_SCANCODE_LEFT]) {
                if (hotbarIndex >= 1 && hotbarTimer.isCompleteReset(150)) {
                    hotbarIndex--;
                    playerInventory.moveCursor(hotbarIndex, 0);
                    player.getController().validateCombatStance(playerInventory.getSlot(hotbarIndex));
                }
            }


// Player Movement
            playerCanFastFall = true;
            // TODO: Change This To A Integer With Bit Operations To Save Memory
            // TODO: Possibly Add This To An Advanced Version Of The `Rect` Struct
            bool playerCanMove[4] = {true, true, true, true};
            // Gravity
            std::vector<SDL_Rect> collisions = {player.pos()};
            auto playerGravityCollisions = world.collisionCheck(collisions);
            // If Player Is Colliding With Block
            for (const auto& col : playerGravityCollisions) {
                if (col.getCenterAsRect().pos.y > player.asRect().getCenterAsRect().pos.y) {
                    if (col.pos.w >= 5) playerCanMove[2] = false;
                } else {
                    if (col.pos.w >= 5) playerCanMove[0] = false;
                }
                if (col.pos.h > col.pos.w && col.pos.h >= 5) {
                    if (col.getCenterAsRect().pos.x > player.asRect().getCenterAsRect().pos.x) {
                        playerCanMove[1] = false;
                    } else {
                        playerCanMove[3] = false;
                    }
                }
                playerCanFastFall = false;
                playerCanJump = true;
            }

            if (playerCanMove[2]) worldOffset.y -= 3;


            auto shiftHeldDown = event.keyState()[SDL_SCANCODE_LSHIFT];
            if (event.keyState()[SDL_SCANCODE_A]) {
                if (playerCanMove[3]) worldOffset.x += (shiftHeldDown) ? 11 : 7;
                player.setPlayerDirect(Player::PlayerDirection::LEFT);
            }
            if (event.keyState()[SDL_SCANCODE_D]) {
                if (playerCanMove[1]) worldOffset.x -= (shiftHeldDown) ? 11 : 7;
                player.setPlayerDirect(Player::PlayerDirection::RIGHT);
            }
#ifdef DEBUG_CONTROLS
            if (event.keyState()[SDL_SCANCODE_W]) {
                if (playerCanMove[0])
                    worldOffset.y += 7;
            }
            if (event.keyState()[SDL_SCANCODE_S] && canFastFall && playerCanMove[2]) worldOffset.y -= 7;
#endif
            if (event.keyState()[SDL_SCANCODE_SPACE] && playerCanJump && playerCanMove[0]) {
                playerCanJump = false;
                worldOffset.y += 100;
            }
// End Player Movement

            if (event.mouseDownL() ) {
                if (playerInventory.getSlot(hotbarIndex).type() == ItemSpawnList::ItemType::Block) {
                    auto block = world.getBlockAtCursor();
                    if (block != nullptr) {
                        (*block).second = (World::BlockType)playerInventory.getSlot(hotbarIndex).id();
                        (*block).first.setTexture(playerInventory.getSlot(hotbarIndex).getTexture());
                    }
                }
                else if (playerInventory.getSlot(hotbarIndex).type() == ItemSpawnList::ItemType::Tool) {
                    if (playerInventory.getSlot(hotbarIndex).id() == ItemSpawnList::ItemID::TestPickaxe) {
                        auto block = world.getBlockAtCursor();
                        if (block != nullptr) {
                            (*block).second = World::BlockType::None;
                        }
                    }
                }
            }

            if (event.keyState()[SDL_SCANCODE_DOWN]) {
                SelectedItem::SelectItem(playerInventory.getSlot(hotbarIndex), renderer);
            } else if (event.keyState()[SDL_SCANCODE_UP]) {
                SelectedItem::DropSelectedItem(playerInventory.getSlot(hotbarIndex), renderer);
            }
            world.offsetWorld(worldOffset);
            scene.moveWorldTo(worldOffset);
        }
        player.pollPlayerController(event, playerInventory.getSlot(hotbarIndex), worldOffset);

        enemy.update();

        renderer.clear();


        world.draw(renderer);

        enemy.draw(renderer);
        player.draw(renderer);
        player.drawOutline(renderer);

        switch (auto currentItem = playerInventory.getSlot(hotbarIndex); currentItem.type()) {
            case ItemSpawnList::ItemType::Weapon:
            case ItemSpawnList::ItemType::Tool:
                // Tool/Weapon Needs To Be Out
                player.getController().drawWeapon(renderer,
                                                  player.pos(),
                                                  player.getPlayerFlip(),
                                                  playerInventory[hotbarIndex]);
                break;
            default:
                break;
        }

        playerInventory.draw(renderer);
        playerInventory.drawCursor(renderer);

        SelectedItem::DrawSelectedItem(renderer, {{(int)(hotbarIndex * 32) + 16, 16, 32, 32}});
        //PlayAnimations();


        renderer.present();
    }
    return 0;
}

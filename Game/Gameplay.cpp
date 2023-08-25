//
// Created by Owner on 4/1/2023.
//

#include <thread>

#include "Gameplay.hpp"
#include "Player/Player.hpp"
#include "Enemy/Enemy.hpp"
#include "../Item/Inventory.hpp"
#include "../Engine/UI/UIComponents.hpp"

#ifdef WORLD_DEMO_TEST_ENABLED
auto Game::WorldDemo(Renderer &renderer) -> int {

    FontRenderer FontRenderer(renderer);

    Scene scene{};
    scene.setActiveScene();
    scene.moveWorldTo(-100, -100);

    World world {};

    Timer worldSplitTimer{};



    while (renderer.gameLoopIsValid()) {
        auto event = renderer.pollEvents();

        if (renderer.updateGameLoop()) {
/*
            if (event.mouseDownL() && worldSplitTimer.isComplete(100)) {
                auto grid = world.getGrid().grid->getGridParentAtMouse(event, *world.getStorage(), world.getGrid().grid, nullptr, 100, 100);
                if (grid.has_value()) {
                    grid->accessor.print();
                    grid->accessor.loadNew(*world.getStorage(), 1, grid->accessor.section);
                    worldSplitTimer.reset();
                }
            } else if (event.mouseDownR() && worldSplitTimer.isComplete(100)) {
                // TODO: This Does Not Work
                auto grid = world.getGrid().grid->getGridParentAtMouse(event, *world.getStorage(), world.getGrid().grid, nullptr, 100, 100);
                auto mod = &grid->parent[grid->accessor.section];
                mod->collapseFromParent(*world.getStorage(), mod);

                worldSplitTimer.reset();
            }
*/
            auto grid = world.getGrid().grid->getGridParentAtMouse(event, *world.getStorage(), world.getGrid().grid, nullptr, 100, 100);
            if (event.mouseDownL() && worldSplitTimer.isComplete(200)) {
                if (grid.has_value()) {
                    auto g = grid->accessor;
                    g.grid->loadNew(*world.getStorage(), 1, g.section);
                }
                worldSplitTimer.reset();
            } else if (event.mouseDownR() && worldSplitTimer.isComplete(200)) {
                if (grid.has_value()) {
                    auto g = grid;
                    auto sec = g->accessor.section;
                    if (g->parent != nullptr) {
                        //auto &child = g->accessor.grid;
                        auto &child = g->accessor.grid;
                        if (child != nullptr) {
                            for (int i = 0; i < 4; i++) {
                                if (child->grids[i] != nullptr) {
                                    delete (child->grids[i] = nullptr);
                                }
                            }
                            child = nullptr;
                            delete child;
                        }
                    }
                }
                worldSplitTimer.reset();
            } else if (event.mouseDownM() && worldSplitTimer.isComplete(200)) {
                if (grid.has_value()) {
                    auto g = grid->accessor;
                    printf("Depth %i, Section %i\n", g.grid->depth, g.section);
                    g.grid->printGridStatus();
                }
                worldSplitTimer.reset();
            }

        }



        renderer.clear();

        world.draw(renderer, FontRenderer, event);

        renderer.present();
    }

    return 0;
}
#endif

#define PATH_TO_SRC "C:\\Users\\Bryson\\Documents\\Code\\Game"


struct Chunk {
private:
    int _x = 0, _y = 0;
    friend struct ChunkHandler;
public:
    static constexpr size_t s_blocksX = 5;
    static constexpr size_t s_blocksY = 5;
    Rect blocks[s_blocksX * s_blocksY];
    BlockType types[s_blocksX * s_blocksY];
    Chunk(int _x, int _y) : _x(_x), _y(_y) {
        int y = 0;
        int x = 0;
        for (int i = 0; i < s_blocksX * s_blocksY; i++) {
            if (i % s_blocksX == 0) { x = 0; y++; }
            blocks[i].pos = {(x * 32) + _x, (y * 32) + _y, 32, 32};
            blocks[i].col = {255, 50, 255};
            types[i] = BlockType::None;
            x++;
        }
    }
    SDL_Rect getRect(v2<int> offset = {0,0}) {
        return {
            _x + offset.x,
            _y + offset.y,
            blocks[0].pos.w * (int)s_blocksX,
            blocks[0].pos.h * (int)s_blocksY
        };
    }
    void draw(Renderer& renderer, v2<int> offset = {0,0}) {
        for (int i = 0; i < s_blocksX * s_blocksY; i++) {
            Rect adjusted = blocks[i];
            adjusted.pos.x += offset.x;
            adjusted.pos.y += offset.y;
            renderer.drawRect(adjusted, {{.filled = false}});
        }
    }
};
static size_t s_globalChunkIndex = 0;
struct ChunkHandler {
private:
    bool& getLeftLoad() { return leftChunk->baseLoaded; }
    bool& getRightLoad() { return rightChunk->baseLoaded; }
public:
    ChunkHandler(int _x, int _y) : baseChunk(_x, _y) {
        baseLoaded = true;
    }
    Chunk baseChunk;
    bool baseLoaded;

    ChunkHandler* leftChunk = nullptr;
    //bool leftLoaded = false;
    ChunkHandler* rightChunk = nullptr;
    //bool rightLoaded = false;

    void loadBase() { baseLoaded = true; }
    void unloadBase() { baseLoaded = false; }
    bool leftIsLoaded() { return (leftChunk != nullptr && leftChunk->baseLoaded); }
    bool rightIsLoaded() { return (rightChunk != nullptr && rightChunk->baseLoaded); }

    void loadLeft() {
        if (leftChunk == nullptr) {
            printf("New Chunk GID: %zi\nLoaded: %i %i %i\n", s_globalChunkIndex++, leftIsLoaded(), baseLoaded, rightIsLoaded());
            leftChunk = new ChunkHandler{baseChunk._x - (32 * 5), baseChunk._y};
            getLeftLoad() = true;
        }
        (*leftChunk).rightChunk = this;
        (*leftChunk).getRightLoad() = true;
    }
    void unloadLeft() { if (leftChunk != nullptr) getLeftLoad() = false; }

    void loadRight() {
        if (rightChunk == nullptr) {
            printf("New Chunk GID: %zi\nLoaded: %i %i %i\n", s_globalChunkIndex++, leftIsLoaded(), baseLoaded, rightIsLoaded());
            rightChunk = new ChunkHandler{baseChunk._x + (32 * 5), baseChunk._y};
            getRightLoad() = true;
        }
        (*rightChunk).leftChunk = this;
        (*rightChunk).getLeftLoad() = true;
    }
    void unloadRight() { if (rightChunk != nullptr) getRightLoad() = false; }

    void draw(Renderer& renderer, v2<int> offset = {0,0}) {
        if (leftIsLoaded() && leftChunk != nullptr) leftChunk->baseChunk.draw(renderer, offset);
        if (baseLoaded) baseChunk.draw(renderer, offset);
        if (rightIsLoaded() && rightChunk != nullptr) rightChunk->baseChunk.draw(renderer, offset);
    }
};


#define MAIN_GAME
#ifdef MAIN_GAME

#include <fstream>

auto CheckAnimations(Renderer& renderer) -> int {
    Player player(renderer, 100,100);
    FontRenderer fontRenderer(renderer);


    std::ofstream writeFile(PATH_TO_SRC"\\Asset\\SpriteSheets\\offsets.txt", std::ios::trunc);
    SDL_Rect saveButton = {0,0,100,30},
             rotateLeftButton = {120, 0, 50, 30},
             rotateRightButton = {180, 0, 50, 30};

    Timer frameChangeTimer{};
    Timer recordTimer{};
    Timer rotateTimer{};
    int32_t lastRecordedFrame = -1;
    int32_t objSize = 32;
    int32_t objScale = 2;
    SDL_Rect heldObj = {player.pos().x, player.pos().y, objSize * objScale, objSize * objScale};

    Renderer::TextureDrawProperties objProps;
    objProps.rotation = 0.0f;
    objProps.rotationOrigin = {0, objSize * objScale};
    objProps.flip = SDL_FLIP_NONE;
    auto objTexture = renderer.loadTexture(PATH_TO_SRC"\\Asset\\sword.png", 0, 0);

    while (renderer.gameLoopIsValid()) {
        auto events = renderer.pollEvents();

        renderer.setColor(55, 55, 55);
        renderer.clear();

        auto& playerAnimator = player.getAnimatorRef();
        if (playerAnimator.getAnimationSetFrameCount() > 0)
            player.draw(renderer);

        if (events.keyState()[SDL_SCANCODE_RIGHT] && frameChangeTimer.isComplete(200)) {
            playerAnimator.nextFrame();
            frameChangeTimer.reset();
        } else if (events.keyState()[SDL_SCANCODE_LEFT] && frameChangeTimer.isComplete(200)) {
            playerAnimator.previousFrame();
            frameChangeTimer.reset();
        } else if (events.keyState()[SDL_SCANCODE_0] && frameChangeTimer.isComplete(200)) {
            playerAnimator.setFrame(0);
            frameChangeTimer.reset();
        } else if (events.keyState()[SDL_SCANCODE_DOWN] && frameChangeTimer.isComplete(200)) {
            auto animSet = playerAnimator.getAnimationSet();
            printf("Animation Sets: %i\n", playerAnimator.getAnimationSetCount());
            if (animSet < playerAnimator.getAnimationSetCount() - 1) {
                playerAnimator.setFrame(0);
                playerAnimator.setAnimationIndex(animSet + 1);
            }
            frameChangeTimer.reset();
        } else if (events.keyState()[SDL_SCANCODE_UP] && frameChangeTimer.isComplete(200)) {
            auto animSet = playerAnimator.getAnimationSet();
            if (animSet >= 1) {
                playerAnimator.setFrame(0);
                playerAnimator.setAnimationIndex(animSet - 1);
            }
            frameChangeTimer.reset();
        }



        renderer.drawLine({player.pos().x, player.pos().y}, {heldObj.x, heldObj.y});
        v2<int> heldObjOffset = {heldObj.x - player.pos().x, heldObj.y - player.pos().y};

        //renderer.drawRect(heldObj);
        renderer.drawTexture(heldObj, objTexture, objProps);
        auto col = renderer.getColor();
        renderer.setColor(40,40,40);
        renderer.drawRect(saveButton);
        fontRenderer.drawText(renderer, "Save", saveButton.x, saveButton.y);
        renderer.drawRect(rotateLeftButton);
        fontRenderer.drawText(renderer, "Rotate Left", saveButton.x, saveButton.y);
        renderer.drawRect(rotateRightButton);
        fontRenderer.drawText(renderer, "Rotate Right", saveButton.x, saveButton.y);
        renderer.setColor(col);

        fontRenderer.drawNumber(renderer, heldObjOffset.x, heldObj.x, heldObj.y - 30);
        fontRenderer.drawNumber(renderer, heldObjOffset.y, heldObj.x + 50, heldObj.y - 30);


        if (events.mouseDownL()) {
            if (rectsCollide({events.mx(), events.my(), 3, 3}, saveButton)) {
                if (recordTimer.isComplete(200)) {
                    writeFile << playerAnimator.getAnimationSet()
                              << ":" << playerAnimator.getFrame()
                              << "{" << heldObjOffset.x
                              << ", " << heldObjOffset.y
                              << ", " << objProps.rotation
                              << "},\n";
                    lastRecordedFrame = playerAnimator.getFrame();
                    recordTimer.reset();
                    printf("Recorded %i:%i\n", playerAnimator.getAnimationSet(), playerAnimator.getFrame());
                }
            } else if (rectsCollide({events.mx(), events.my(), 3, 3}, rotateLeftButton)) {
                if (rotateTimer.isComplete(200)) {
                    objProps.rotation -= (events.keyState()[SDL_SCANCODE_RSHIFT]) ? 1.0f : 5.0f;
                    rotateTimer.reset();
                }
            } else if (rectsCollide({events.mx(), events.my(), 3, 3}, rotateRightButton)) {
                if (rotateTimer.isComplete(200)) {
                    objProps.rotation += (events.keyState()[SDL_SCANCODE_RSHIFT]) ? 1.0f : 5.0f;
                    rotateTimer.reset();
                }
            } else {
                heldObj.x = events.mx();
                heldObj.y = events.my() - (objSize * objScale);
            }
        }




        fontRenderer.drawNumber(renderer, playerAnimator.getAnimationSet(), 170, 30);
        fontRenderer.drawNumber(renderer, playerAnimator.getFrame(), 200, 30);




        renderer.present();
    }
    writeFile.close();
    return 0;
}

auto Game::RunGame(Renderer& renderer) -> int {
    //return CheckAnimations(renderer);

    FontRenderer fontRenderer(renderer);

    Player player(renderer, 0,0);
    player.centerPlayerSprite(renderer);

    Texture backgroundTextures[] = {
            renderer.loadTexture(PATH_TO_SRC"\\Asset\\BG.png"),
            renderer.loadTexture(PATH_TO_SRC"\\Asset\\FG.png")
    };


    // Load World Sprite Sheet
    Texture tileTextures[] = {
            renderer.loadTexture(PATH_TO_SRC"\\Asset\\Tileset64.png", 0, 0, 64, 64),
            renderer.loadTexture(PATH_TO_SRC"\\Asset\\Tileset64.png", 64, 0, 64, 64),
            renderer.loadTexture(PATH_TO_SRC"\\Asset\\Tileset64.png", 128, 0, 64, 64),
    };

    // generate world

    Scene scene;
    scene.setActiveScene();
    v2<int32_t> worldOffset = {200, player.pos().y + player.pos().h + 32};
    scene.moveWorldTo(worldOffset);

    World world{};//(100, 100, 32, 32);
    auto generateBlock = [](int x, int y) {
        if (y < 5) {
            return BlockType::None;
        } else
            return (y == 5) ? BlockType::Grass : BlockType::Dirt;
    };

    world.generate(100, 100 ,32, 32, generateBlock);
    world.offsetWorld(scene.getSceneOffset());
    world.addWorldTextureForBlockType(BlockType::None, tileTextures[0]);
    world.addWorldTextureForBlockType(BlockType::Grass, tileTextures[0]);
    world.addWorldTextureForBlockType(BlockType::Dirt, tileTextures[1]);
    world.addWorldTextureForBlockType(BlockType::Stone, tileTextures[2]);
    world.updateWorldTextures();


    ChunkHandler chunkHandler{0,0};
    ChunkHandler* parentChunk = &chunkHandler;
    std::vector<Chunk> loadedChunks{};



/*
   auto CollapseWorldTiles = [&](){
        auto blocks = world.getBlocks();
        auto block = world.getBlockAccessorAtIndex(0);
        const size_t maxIterations = world.getBlocks().size();

        for (int i = 0; i < maxIterations; i++) {
            if (block.block->second == block.right()->block->second && block.block->first.pos.w < 96) {
                (*block.block).first.pos.w += block.right()->block->first.pos.w;
                world.getBlocks()[i+1].second = BlockType::None;//.erase(world.getBlocks().begin() + i);
                //world.getBlocks().erase(world.getBlocks().begin() + i);
                //i++;
                continue;
            }
        }
    };
   CollapseWorldTiles();
*/

    size_t hotbarIndex = 0, inventoryIndex = 0;
    size_t hotbarSize = 10, inventorySize = 10 * 5;
    Timer inventoryOpenTimer{};
    Timer inventoryCursorTimer{};
    bool inventoryIsOpen = false;

    Inventory playerInventory(renderer, 10, 5);
    playerInventory.moveCursor(0,0);
    playerInventory.setSlot(1,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::TestTome, renderer));
    (*playerInventory.getSlot(1).stackSize) = 10;
    playerInventory.setSlot(2,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::TestSword, renderer));
    (*playerInventory.getSlot(2).stackSize) = 10;
    playerInventory.setSlot(3,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::TestPickaxe, renderer));
    //playerInventory.setSlot(3,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::GrassBlock, renderer));
    //playerInventory.setSlot(4,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::DirtBlock, renderer));
    //playerInventory.setSlot(5,0,ItemSpawnList::Spawn(ItemSpawnList::ItemID::StoneBlock, renderer));



    std::vector<Item> droppedItems;
    std::vector<uint32_t> droppedItemsStackSize;

    int blockBreakTick = 0;
    BlockType blockBreakType = BlockType::None;

    Timer lastStackCombine{};
    v4<int> stackCombinePoints{};

    while (renderer.gameLoopIsValid()) {
        // Get Inputs
        Event event = renderer.pollEvents();

        if (renderer.updateGameLoop()) {


#define DEBUG_RESET_PLAYER_Y
#ifdef DEBUG_RESET_PLAYER_Y
            if (event.keyState()[SDL_SCANCODE_R]) {
                v2<int32_t> worldOriginalOffset = {200, player.pos().y + player.pos().h + 32};
                scene.moveWorldTo(worldOriginalOffset);
            }
#endif


            if (inventoryCursorTimer.isComplete(100)) {
                if (event.keyState()[SDL_SCANCODE_RIGHT]) {
                    if (!inventoryIsOpen) {
                        if (hotbarIndex < hotbarSize - 1) {
                            hotbarIndex++;
                            playerInventory.moveCursor(hotbarIndex, 0);
                            player.getController().validateCombatStance((*playerInventory.getSlot(hotbarIndex).item));
                        }
                    } else {
                        if (inventoryIndex < inventorySize - 1) {
                            if (inventoryIndex < hotbarSize - 1) hotbarIndex++;
                            inventoryIndex++;
                            playerInventory.moveCursor(inventoryIndex % 10, inventoryIndex / 10);
                        }
                    }
                }
                if (event.keyState()[SDL_SCANCODE_LEFT]) {
                    if (!inventoryIsOpen) {
                        if (hotbarIndex >= 1) {
                            hotbarIndex--;
                            playerInventory.moveCursor(hotbarIndex, 0);
                            player.getController().validateCombatStance((*playerInventory.getSlot(hotbarIndex).item));
                        }
                    } else {
                        if (inventoryIndex >= 1) {
                            if (inventoryIndex < hotbarSize - 1) hotbarIndex--;
                            inventoryIndex--;
                            playerInventory.moveCursor(inventoryIndex % 10, inventoryIndex / 10);
                        }
                    }
                }
                if (event.keyState()[SDL_SCANCODE_UP]) {
                    if (inventoryIsOpen && inventoryIndex >= 10) {
                        inventoryIndex -= 10;
                        playerInventory.moveCursor(inventoryIndex % 10, inventoryIndex / 10);
                    } else {

                    }
                }
                if (event.keyState()[SDL_SCANCODE_DOWN]) {
                    if (inventoryIsOpen && inventoryIndex < inventorySize - 11) {
                        inventoryIndex += 10;
                        playerInventory.moveCursor(inventoryIndex % 10, inventoryIndex / 10);
                    } else {

                    }
                }

                if (event.keyState()[SDL_SCANCODE_V]) {
                    printf("Holding Item: %i\n", SelectedItem::isHoldingItem);
                    if (!SelectedItem::isHoldingItem) {
                        SelectedItem::SelectItem((*playerInventory.getSlot(hotbarIndex).item), renderer);
                        printf("Picking Up Item\n");
                    } else {
                        SelectedItem::DropSelectedItem((*playerInventory.getSlot(hotbarIndex).item), renderer);
                        printf("Dropping Item\n");
                    }
                }
                inventoryCursorTimer.reset();
            }


            if (event.keyState()[SDL_SCANCODE_E] && inventoryOpenTimer.isCompleteReset(100)) {
                if (!inventoryIsOpen) {
                    inventoryIndex = hotbarIndex;
                    playerInventory.moveCursor(inventoryIndex%10, inventoryIndex/10);
                }
                inventoryIsOpen = !inventoryIsOpen;
            }
            if (event.keyState()[SDL_SCANCODE_R] && inventoryIsOpen) {
                playerInventory.debugPrint();
            }
            if (event.keyState()[SDL_SCANCODE_H] && inventoryIsOpen) {
                (*playerInventory[0].stackSize)++;
            }


// Player Movement

            std::vector<SDL_Rect> collisions = {
                    // Player Collision
                    player.pos(),
                    // Broad Over Head Collisions
                    {player.pos().x-16, player.pos().y-(int)player.getMaxJumpHeight(), player.pos().w+32, player.pos().h+100},
            };
            std::vector<Rect> possiblePlayerWorldCollisions = world.collisionCheck(collisions);
            player.update(event, scene, possiblePlayerWorldCollisions);
/*
            if (parentChunk->baseLoaded)
                possiblePlayerWorldCollisions.emplace_back(parentChunk->baseChunk.getRect(scene.getSceneOffset().add(0,32)));
            if (parentChunk->rightIsLoaded())
                possiblePlayerWorldCollisions.emplace_back(parentChunk->rightChunk->baseChunk.getRect(scene.getSceneOffset().add(32 * 5,32)));
            if (parentChunk->leftIsLoaded())
                possiblePlayerWorldCollisions.emplace_back(parentChunk->leftChunk->baseChunk.getRect(scene.getSceneOffset().add(-32 * 5,32)));



            auto CheckLoadedChunks = [&](){
                for (int i = 0; i < loadedChunks.size(); i++) {
                    auto center = loadedChunks[i].getRect(scene.getSceneOffset());
                    if (abs(distance(player.pos().x, player.pos().y, center.x, center.y)) > 100) {
                        loadedChunks.erase(loadedChunks.begin() + i);
                        i--;
                        continue;
                    }
                }
            };

            auto baseChunk = parentChunk->baseChunk.getRect(scene.getSceneOffset());
            auto playerCenterX = player.pos().x + (player.pos().w/2);
            if (playerCenterX > baseChunk.x + baseChunk.w + 32) {
                parentChunk->unloadLeft();
                parentChunk->loadRight();
                parentChunk = parentChunk->rightChunk;
                CheckLoadedChunks();
                loadedChunks.push_back(parentChunk->baseChunk);
                if (parentChunk->rightIsLoaded())
                    loadedChunks.push_back(parentChunk->rightChunk->baseChunk);
            }
            else if (player.pos().x < baseChunk.x) {
                parentChunk->unloadRight();
                parentChunk->loadLeft();
                parentChunk = parentChunk->leftChunk;
                CheckLoadedChunks();
                loadedChunks.push_back(parentChunk->baseChunk);
                if (parentChunk->leftIsLoaded())
                    loadedChunks.push_back(parentChunk->leftChunk->baseChunk);
            }

*/

            if (!event.mouseDownL()) { blockBreakTick = 0; }
            if (event.mouseDownL()) {
                if ((*playerInventory.getSlot(hotbarIndex).item).type() == ItemSpawnList::ItemType::Block) {
                    auto block = world.getBlockAtCursor();
                    if (block != nullptr) {
                        (*block).second = (BlockType)(*playerInventory.getSlot(hotbarIndex).item).id();
                        (*block).first.setTexture((*playerInventory.getSlot(hotbarIndex).item).getTexture());
                    }
                }
                else if ((*playerInventory.getSlot(hotbarIndex).item).type() == ItemSpawnList::ItemType::Tool) {
                    if ((*playerInventory.getSlot(hotbarIndex).item).id() == ItemSpawnList::ItemID::TestPickaxe) {
                        auto playerTool = playerInventory.getSlot(hotbarIndex).item;
                        auto block = world.getBlockAtCursor();
                        if (block != nullptr && block->second != BlockType::None) {
                            if (blockBreakType != block->second) { blockBreakTick = 0; blockBreakType = block->second; }
                            blockBreakTick++;
                            if (blockBreakTick > (100 - playerTool->getToolStats().toolStrength)) {
                                blockBreakTick = 0;
                                SDL_Rect blockOgPosition = world.getBlockOriginalPositionAtCursor();

                                bool found = false;
                                for (int i = 0; i < droppedItems.size(); i++) {
                                    if ((BlockType) droppedItems[i].id() != block->second) continue;
                                    v4<int> points = {
                                            droppedItems[i].getRect().pos.x,
                                            droppedItems[i].getRect().pos.y,

                                            blockOgPosition.x,
                                            blockOgPosition.y
                                    };
                                    if (distance(points.x, points.y, points.z, points.w) < 200.0f) {
                                        lastStackCombine.reset();
                                        stackCombinePoints = points;
                                        droppedItemsStackSize[i]++;
                                        (*block).second = BlockType::None;
                                        found = true;
                                        break;
                                    }
                                }
                                if (!found) {
                                    blockOgPosition.x += 5;
                                    blockOgPosition.w -= 10;
                                    blockOgPosition.y += 5;
                                    blockOgPosition.h -= 10;
                                    Item item{
                                            ItemSpawnList::ItemType::Block,
                                            (ItemSpawnList::ItemID) block->second,
                                            block->first.getTextureList()[0],
                                            blockOgPosition,
                                    };
                                    droppedItems.push_back(item);
                                    droppedItemsStackSize.push_back(1);
                                    (*block).second = BlockType::None;
                                }
                            }
                        }
                    }
                }
            }


            world.offsetWorld(scene.getSceneOffset());
        }
        player.pollPlayerController(event, (*playerInventory.getSlot(hotbarIndex).item), worldOffset);



        renderer.clear();
        renderer.drawTexture(renderer.getScreenAsRect(), backgroundTextures[0]);
        renderer.drawTexture(renderer.getScreenAsRect(), backgroundTextures[1]);


        world.draw(renderer);
        FontRenderer::FontProperties textProperties{};
        textProperties.characterSize = 32;
        textProperties.kerning = textProperties.characterSize/2;
        textProperties.renderAsOutlined = true;
        fontRenderer.drawText(renderer, "Hello", 100, 100, textProperties);
        fontRenderer.drawText(renderer, "Rendering A String", 100, 200, textProperties);



        player.draw(renderer);
        player.drawOutline(renderer);
        player.drawPlayerUI(renderer);
/*
        if (playerInventory[hotbarIndex].item->type() == ItemSpawnList::ItemType::Tool ||
            playerInventory[hotbarIndex].item->type() == ItemSpawnList::ItemType::Weapon ||
            playerInventory[hotbarIndex].item->type() == ItemSpawnList::ItemType::MagicalItem) {
            std::vector<v2<int>> gripPoints[] = {
                    {
                            {-6, 0},
                            {6, 0},
                            {10, 0},
                            {8, 0},
                            {6, 0},
                            {-4, 0},
                            {-7, 0},
                            {-9, 0},
                            {0, 0},
                    },
                    {
                            {0,  0},
                            {0, 2},
                            {0,  4},
                            {0, 5},
                            {0, 6},
                            {0,  5},
                            {0,  4},
                            {0,  2},
                            {0, 0},
                            {0, 0},
                    },
            };
            uint32_t animationIndex = player.getAnimator().getFrame();
            uint32_t animationSet = player.getAnimator().getAnimationSet();

            SDL_Rect weaponPos = {0, 0, 0, 0};

            Renderer::TextureDrawProperties weaponProps;
            int32_t weaponSize = 32, weaponScale = 2;
            weaponProps.flip = player.getPlayerFlip();
            weaponProps.rotationOrigin = {(weaponProps.flip == SDL_FLIP_NONE) ? 0 : weaponSize * weaponScale,
                                          weaponSize * weaponScale};

            if (player.getController().playerInCombatStance()) {
                weaponProps.rotation = (weaponProps.flip == SDL_FLIP_NONE) ? 40.0f : -40.0f;
            } else {
                weaponProps.rotation = (weaponProps.flip == SDL_FLIP_NONE) ? 150.0f : -150.0f;
            }

            float weaponSwayRot = 0.0f;
            if (animationSet == 1) {
                weaponPos.x = player.pos().x;
                weaponPos.y = player.pos().y;
                if (player.getController().playerInCombatStance()) {
                    int32_t weaponOffsetX = gripPoints[0][animationIndex].x;
                    if (player.getPlayerFlip() == SDL_FLIP_HORIZONTAL)
                        weaponPos.x -= (weaponOffsetX - 3);
                    else
                        weaponPos.x += weaponOffsetX;
                    weaponPos.y = player.pos().y + gripPoints[0][animationIndex].y;
                }
            } else if (animationSet == 0) {
                weaponPos.x = player.pos().x;
                weaponPos.y = player.pos().y + gripPoints[1][animationIndex].y;
            } else {
                weaponPos.x = player.pos().x;
                weaponPos.y = player.pos().y;
            }

            if ((animationSet == 1 || animationSet == 2) && !player.getController().playerInCombatStance())
                weaponProps.rotation += (float) sin(animationIndex) * ((animationSet == 2) ? 5.0f : 2.0f);

            weaponPos.w = 64;
            weaponPos.h = 64;

            if (!player.getController().playerInCombatStance())
                weaponPos.y -= weaponSize * weaponScale;

            (*playerInventory[hotbarIndex].item).draw(renderer, weaponPos, {weaponProps});
            //player.getController().drawWeapon(renderer, player.pos(), player.getPlayerFlip(), (*playerInventory[hotbarIndex].item));
        }*/
        DrawPlayersHeldItem(renderer, player, *playerInventory[hotbarIndex].item);

        static Timer lastItemPickedUp{};
        v4<int> itemPickUpPoints{};


        if (lastItemPickedUp.getTime() < 100) {
            renderer.drawLine(itemPickUpPoints);
        }
        if (lastStackCombine.getTime() < 100) {
            drawLineFromSceneView(renderer, stackCombinePoints);
            //renderer.drawLine(stackCombinePoints);
        }

        for (size_t i = 0; i < droppedItems.size();) {
            auto aabb = getAABBFromSceneView(droppedItems[i].getRect().pos);
            v2<int> xy = { aabb.x, aabb.y, };
            if (distance(xy.x, xy.y, player.pos().x, player.pos().y) < 150.0f) {
                Inventory::ItemSlotAccessor inventoryAccess = playerInventory.getFirstSlotOfID(droppedItems[i].id());
                if (inventoryAccess.error == Inventory::InventoryAccessResult::IDFound) {
                    lastItemPickedUp.reset();
                    itemPickUpPoints.x = xy.x;
                    itemPickUpPoints.y = xy.y;
                    auto playerCenter = getCenterOfRect(player.pos());
                    itemPickUpPoints.z = playerCenter.x;
                    itemPickUpPoints.w = playerCenter.y;

                    (*inventoryAccess.stackSize) += droppedItemsStackSize[i];
                    printf("New Item: ID: %i, Stack Size: %i\n", (*inventoryAccess.item).id(), (*inventoryAccess.stackSize));
                    droppedItems.erase(droppedItems.begin()+i);
                    droppedItemsStackSize.erase(droppedItemsStackSize.begin()+i);
                    continue;
                }
                if (inventoryAccess.error == Inventory::InventoryAccessResult::IDNotFound) inventoryAccess = playerInventory.getFirstEmptySlot();
                if (inventoryAccess.error == Inventory::InventoryAccessResult::EmptySlotFound) {
                    lastItemPickedUp.reset();
                    itemPickUpPoints.x = xy.x;
                    itemPickUpPoints.y = xy.y;
                    auto playerCenter = getCenterOfRect(player.pos());
                    itemPickUpPoints.z = playerCenter.x;
                    itemPickUpPoints.w = playerCenter.y;

                    printf("Stack Size: %i\n", droppedItemsStackSize[i]);

                    (*inventoryAccess.item) = droppedItems[i];
                    (*inventoryAccess.stackSize) = droppedItemsStackSize[i];
                    printf("New Item: ID: %i, Stack Size: %i\n", (*inventoryAccess.item).id(), (*inventoryAccess.stackSize));
                    droppedItems.erase(droppedItems.begin()+i);
                    droppedItemsStackSize.erase(droppedItemsStackSize.begin()+i);
                    continue;
                }
                else
                    printf("Inventory Full Error! (Error: %i)\n", inventoryAccess.error);
            }
            drawObjectFromSceneView(renderer, droppedItems[i].getRect().pos, droppedItems[i].getRect().getTextureList()[0]);
            i++;
        }

        if (blockBreakTick > 0) {
            drawPercentageBar(renderer, {event.mx() - 25, event.my() - 25, 50, 20}, blockBreakTick, (100 - playerInventory.getSlot(hotbarIndex).item->getToolStats().toolStrength));
        }

        if (inventoryIsOpen) {
            playerInventory.draw(renderer, fontRenderer);
            playerInventory.drawCursor(renderer);
        }
        else {
            Inventory::ItemSlotRangeAccessor hotbar(playerInventory, 0, 10);
            hotbar.moveCursor(hotbarIndex, 0);
            hotbar.draw(renderer);
            hotbar.drawCursor(renderer);


            //playerInventory.draw(renderer);
            //playerInventory.drawCursor(renderer);
        }

        SelectedItem::DrawSelectedItem(renderer, {{(int)(hotbarIndex * 32) + 16, 16, 32, 32}});

        //parentChunk->draw(renderer, scene.getSceneOffset());
        /*for (auto& i : loadedChunks) {
            i.draw(renderer, scene.getSceneOffset());
        }*/



        renderer.present();
    }

    return 0;
}

#endif


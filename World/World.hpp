//
// Created by Owner on 3/19/2023.
//

#ifndef SDL_WORLD_HPP
#define SDL_WORLD_HPP

#include <map>

#include "../Engine/Renderer/Renderer.hpp"
#include "../Engine/Utility/Rect/Rect.hpp"
#include "../Item/Item.hpp"

// TODO: Make World Dynamic, Add Chunks If Block Are Placed Outside Of The Current Chunk
class World {
public:
    enum class BlockType {
        None = (int)ItemSpawnList::ItemID::None,
        Grass = (int)ItemSpawnList::ItemID::GrassBlock,
        Dirt = (int)ItemSpawnList::ItemID::DirtBlock,
        Stone = (int)ItemSpawnList::ItemID::StoneBlock,
    };
private:
    int offsetX;
    int offsetY;
    std::vector<SDL_Rect> ogPositions;
    std::vector<std::pair<Rect, BlockType>> blocks;
    std::map<BlockType, Texture> textures;

    friend int main();
    friend class PlayerController;
public:
    explicit World() : offsetX(0), offsetY(0) {}
    // Colors Based On
    explicit World(int cellsX, int cellsY, int cellsW, int cellsH);
    void offsetWorld(int x, int y);
    void offsetWorld(v2<int,int> offset);
    void draw(Renderer& renderer);
    std::vector<Rect> collisionCheck(std::vector<SDL_Rect>& positions);
    std::pair<Rect, BlockType>* getBlockAtIndex(size_t index);
    void addWorldTextureForBlockType(BlockType type, Texture texture);
    void updateWorldTextures();
    std::pair<Rect, BlockType>* getBlockAtCursor();
    v2<int, int> getWorldOffset();
};


#endif //SDL_WORLD_HPP

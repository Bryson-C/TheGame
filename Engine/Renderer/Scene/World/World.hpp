//
// Created by Owner on 3/19/2023.
//

#ifndef SDL_WORLD_HPP
#define SDL_WORLD_HPP

#include <map>
#include <array>

#include "../../../Utility/Rect/Rect.hpp"
#include "../../Renderer.hpp"
#include "../../../Item/Item.hpp"
#include "GridAccessor.hpp"


// TODO: Make World Dynamic, Add Chunks If Block Are Placed Outside Of The Current Chunk

// TODO: Found An Interesting Solution, Doubly Linked Lists For Chunks Pointing To Each Other
// That Way I Can Have A Vector With The Chunks In Arbitrary Positions
// See: https://stackoverflow.com/questions/1391381/what-data-structures-can-efficiently-store-2-d-grid-data


enum class BlockType {
    None = (int)ItemSpawnList::ItemID::None,
    Grass = (int)ItemSpawnList::ItemID::GrassBlock,
    Dirt = (int)ItemSpawnList::ItemID::DirtBlock,
    Stone = (int)ItemSpawnList::ItemID::StoneBlock,
};

#define DEBUG_COLORS true

struct SparseGrid {
    static constexpr int s_GridSize = 4;

    int depth = 0;
    v2<int>* offset = nullptr;
    v2<int> blockSize{ 320/2, 320/2 };
    BlockType block { BlockType::None };
    SparseGrid* grids[s_GridSize] = { nullptr };

    SparseGrid() = default;

#ifdef DEBUG_COLORS
    RenderDrawColor color = RenderDrawColor::RandomColor();
#endif

    void printGridStatus() {
        printf("Valid Grid:\n\t| %s | %s |\n\t| %s | %s |\n",
               (grids[0] != nullptr) ? "True" : "False",
               (grids[1] != nullptr) ? "True" : "False",
               (grids[2] != nullptr) ? "True" : "False",
               (grids[3] != nullptr) ? "True" : "False");
    }

private:


    // Loads 1 Quadrant From The Parent
    GridAccessor loadNewFromParent(std::vector<SparseGrid*>& gridStorage, SparseGrid* parent, int depth, int section);
    // Loads All Quardrants (4) From The Parent
    std::array<GridAccessor, 4> loadAllFromParent(std::vector<SparseGrid*>& gridStorage, SparseGrid* parent);

    // Turns The Grid Structure Into An Array Of Blocks, All Of Equal Size
    inline void claimSpace() {}

    friend class World;
public:
    // Collapses/Un-Loads The Grid If All Are Empty Or The Same Type
    void collapseFromParent(std::vector<SparseGrid*>& gridStorage, SparseGrid* parent);
    GridAccessor loadNew(std::vector<SparseGrid*>& gridStorage, int _depth, int _section) {
        return loadNewFromParent(gridStorage, this, _depth, _section);
    }
    std::array<GridAccessor, 4> loadAll(std::vector<SparseGrid*>& gridStorage) {
        return loadAllFromParent(gridStorage, this);
    }
    std::optional<GridAccessor> getGridAtMouse(Event& event, std::vector<SparseGrid*>& gridStorage, SparseGrid* _parent, int gridX, int gridY);
    struct GridAccessorParent {
        GridAccessor accessor;
        SparseGrid* parent;
    };
    std::optional<GridAccessorParent> getGridParentAtMouse(Event& event, std::vector<SparseGrid*>& gridStorage, SparseGrid* _parent, SparseGrid* _prevParent, int gridX, int gridY);
    void draw(Renderer& renderer, FontRenderer& FontRenderer, Event& event, int x, int y) {
        v2<int> off = {0,0};
        if (offset != nullptr) off = *offset;

        for (int i = 0; i < s_GridSize; i++) {
            v2<int> size { blockSize.x, blockSize.y };
            v4<int> position {
                x + ((i % 2 == 1) ? size.x : 0) + off.x,
                y + ((i > 1) ? size.y : 0) + off.y,
                size.x,
                size.y,
            };

            auto getCol = renderer.getColor();

#ifdef DEBUG_COLORS
            renderer.setColor(color);
#endif
            if (grids[i] == nullptr) {
                renderer.drawRect((SDL_Rect)position, {{.filled = false}});
                FontRenderer::FontProperties props;
                props.characterSize = 16;
                props.kerning = -2;
            } else {
                grids[i]->draw(renderer, FontRenderer, event, position.x, position.y);
            }
            renderer.setColor(getCol);
        }
    }
};


#ifdef SECOND_WORLD_IMPL
class World {
private:
    v2<int> _offset;
    std::vector<SparseGrid*> _subGridStorage;
    SparseGrid _grid;
    GridAccessor loadNewFromGrid(SparseGrid* parent, int depth, int section);
    friend class GridAccessor;
public:
    inline World() : _offset(Scene::getActiveSceneRenderOffset()) {}
    inline ~World() {
        for (const auto& i : _subGridStorage) {
            delete i;
        }
        _subGridStorage.clear();
    }

    GridAccessor loadNew(int depth, int section);
    // TODO: Delete All The Children If All Children Are Empty. Basically Just Unload The Sub-Grids,
    // Only If They Are All Empty, Alternatively All Tiles/Grid Spaces Can Be Of The Same Type
    void collapseGrid(SparseGrid* grid);
    std::array<GridAccessor, 4> loadAll();

    void move(int x, int y);
    void draw(Renderer& renderer, FontRenderer& FontRenderer, Event& event);

    GridAccessor getGrid() { return {.grid = &_grid, .depth = 0, .section = -1}; }
    std::vector<SparseGrid*>* getStorage() { return &_subGridStorage; }
};
#endif

#define FIRST_WORLD_IMPL
#ifdef FIRST_WORLD_IMPL
class World {
public:
    using Block = std::pair<Rect, BlockType>;
private:
    int offsetX;
    int offsetY;
    int blocksX;
    int blocksY;
    std::vector<SDL_Rect> ogPositions;
    std::vector<std::pair<Rect, BlockType>> blocks;
    std::map<BlockType, Texture> textures;


    friend int main();
    friend class PlayerController;
    friend struct BlockAccessor;
    friend struct Chunk;
public:

    void generate(int cellsX, int cellsY, int cellsW, int cellsH, BlockType (generateFunc)(int, int));

    struct BlockAccessor {
    private:
        World* pWorld;
        size_t index;
    public:
        BlockAccessor(World* world, size_t _index) {
            pWorld = world;
            index = _index;
            block = &pWorld->blocks[index];
        }
        std::pair<Rect, BlockType>* block;
        std::optional<BlockAccessor> left() {
            if (index - 1 >= 0)
                return {{pWorld, index - 1}};
            return {};
        }
        std::optional<BlockAccessor> right() {
            if (index + 1 < pWorld->blocks.size() - 1)
                return {{pWorld, index + 1}};
            return {};
        }
        std::optional<BlockAccessor> above() {
            if (index - pWorld->blocksX >= 0)
                return {{pWorld, index - pWorld->blocksX}};
            return {};

        }
        std::optional<BlockAccessor> below() {
            if (index + pWorld->blocksX < pWorld->blocks.size() - 1)
                return {{pWorld, index + pWorld->blocksX}};
            return {};
        }
    };

    explicit World() : offsetX(0), offsetY(0) {}
    // Colors Based On
    explicit World(int cellsX, int cellsY, int cellsW, int cellsH);

    void offsetWorld(int x, int y);
    void offsetWorld(v2<int,int> offset);
    void draw(Renderer& renderer);
    std::vector<Rect> collisionCheck(std::vector<SDL_Rect>& positions);
    [[deprecated("Use `getBlockAccessorAtIndex`")]] std::pair<Rect, BlockType>* getBlockAtIndex(size_t index);
    World::BlockAccessor getBlockAccessorAtIndex(size_t index) {
        return {this, index};
    }
    void addWorldTextureForBlockType(BlockType type, Texture texture);
    void updateWorldTextures();
    std::pair<Rect, BlockType>* getBlockAtCursor();
    SDL_Rect getBlockOriginalPositionAtCursor() const;
    v2<int, int> getWorldOffset();
    std::vector<std::pair<Rect, BlockType>>& getBlocks() { return blocks; }


    std::vector<size_t> claim(int _x, int _y, int blocksX, int blocksY) {}
};
#endif


#endif //SDL_WORLD_HPP
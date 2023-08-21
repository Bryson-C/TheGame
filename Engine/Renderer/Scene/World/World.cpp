//
// Created by Owner on 3/19/2023.
//

#include "World.hpp"

#ifdef FIRST_WORLD_IMPL

void World::generate(int cellsX, int cellsY, int cellsW, int cellsH, BlockType (*generateFunc)(int, int)) {
    for (int x = 0, y = 0; y < cellsY; x++) {
        if (x == cellsX) {
            x = 0;
            y++;
            if (y == cellsY) break;
        }
        Rect r {x * cellsW, y * cellsH, cellsW, cellsH};
        blocks.emplace_back(r, generateFunc(x,y));
        ogPositions.push_back(r.pos);
    }
}

World::World(int cellsX, int cellsY, int cellsW, int cellsH) : offsetX(0), offsetY(0), blocksX(cellsX), blocksY(cellsY) {
    for (int x = 0, y = 0; y < cellsY; x++) {
        if (x == cellsX) {
            x = 0;
            y++;
            if (y == cellsY) break;
        }
        Rect r {x * cellsW, y * cellsH, cellsW, cellsH};
        if (y < 5) {
            blocks.emplace_back(r, BlockType::None);
        } else
            blocks.emplace_back(r, (y == 5) ? BlockType::Grass : BlockType::Dirt);
        ogPositions.push_back(r.pos);
    }
}
void World::offsetWorld(int x, int y) {
    offsetX = x;
    offsetX = y;
    for (size_t index = 0; auto& block : blocks) {
        block.first.pos.x = ogPositions[index].x + x;
        block.first.pos.y = ogPositions[index].y + y;
        index++;
    }
}

void World::offsetWorld(v2<int, int> offset) { offsetWorld(offset.x, offset.y); }
void World::draw(Renderer& renderer) {
    // Draw Color Should Automatically Be Set And Reset Before And After The Loop
    for (auto& i : blocks) {
        // TODO: Make Air/None Blocks Render, But Transparent Or Something
        if (i.second == BlockType::None) continue;
        if (!i.first.collides(renderer.getScreenAsRect())) continue;
        renderer.drawRect(i.first);
#define DRAW_DEBUG_AABBs
#ifdef DRAW_DEBUG_AABBs
        renderer.drawRect(i.first.pos, {{.filled = false}});
#endif
    }
}
std::vector<Rect> World::collisionCheck(std::vector<SDL_Rect>& positions) {
    std::vector<Rect> collisions;
    for (auto& block : blocks) {
        for (auto& pos : positions) {
            if (block.first.collides(pos) && block.second != BlockType::None) {
                SDL_Rect res;
                SDL_IntersectRect(&(block.first.pos), &pos, &res);
                collisions.emplace_back(res);
            }
        }
    }
    return collisions;
}
std::pair<Rect, BlockType>* World::getBlockAtIndex(size_t index) {
    return &blocks[index];
}
void World::addWorldTextureForBlockType(BlockType type, Texture texture) {
    textures.insert({type, texture});
}
void World::updateWorldTextures() {
    for (auto& i : blocks) {
        if (textures.contains(i.second)) {
            i.first.setTexture(textures[i.second]);
        }
    }
}
std::pair<Rect, BlockType>* World::getBlockAtCursor() {
    int32_t mx, my;
    SDL_GetMouseState(&mx, &my);
    const int MOUSE_PADDING = 3;
    for (auto& i : blocks) {
        SDL_Rect mouse {mx, my, MOUSE_PADDING, MOUSE_PADDING};
        if (SDL_HasIntersection(&i.first.pos, &mouse))
            return &i;
    }
    return nullptr;
}
SDL_Rect World::getBlockOriginalPositionAtCursor() const {
    int32_t mx, my;
    SDL_GetMouseState(&mx, &my);
    const int MOUSE_PADDING = 3;
    for (size_t index = 0; auto& i : blocks) {
        SDL_Rect mouse {mx, my, MOUSE_PADDING, MOUSE_PADDING};
        if (SDL_HasIntersection(&i.first.pos, &mouse))
            return ogPositions[index];
        index++;
    }
    return {0,0,0,0};
}
v2<int, int> World::getWorldOffset() { return {offsetX, offsetY}; }
#endif


#ifdef SECOND_WORLD_IMPL
std::optional<GridAccessor> SparseGrid::getGridAtMouse(Event& event, std::vector<SparseGrid*>& gridStorage, SparseGrid* _parent, int gridX, int gridY) {
    if (_parent == nullptr) { printf("Parent Grid Is Null, Returning Empty\n"); return {}; }
    SparseGrid* parent = _parent;
    int section = 0;
/*
    for (int i = 0; i < s_GridSize; i++) {
        v2<int> size { parent->blockSize.x, parent->blockSize.y };
        v4<int> position {
                gridX + ((i % 2 == 1) ? size.x : 0) + gridOffset.x,
                gridY + ((i > 1) ? size.y : 0) + gridOffset.y,
                parent->blockSize.x, parent->blockSize.y,
        };
        if (rectsCollide((SDL_Rect)position, {mouse.x, mouse.y, 1, 1})) {
            quad = i, depth = parent->depth;
            if (parent->grids[i] != nullptr) {
                getGridAtMouse(event, gridStorage, parent->grids[i], position.x, position.y, depth + 1);
            }
        }
    }
    printf("Mouse Colliding With Section: %i, depth: %i\n", quad, depth);
*/
    SDL_Rect rect {gridX, gridY, parent->blockSize.x * 2, parent->blockSize.y * 2};
    if (!rectsCollide(rect, {event.mx(), event.my(), 1, 1})) return {};

    v2<int> childPos {gridX, gridY};
    auto center = getCenterOfRect(rect);

    if (event.mx() > center.x) { section += 1; childPos.x += parent->blockSize.x; }
    if (event.my() > center.y) { section += 2; childPos.y += parent->blockSize.y; }


    if (parent->grids[section] != nullptr) {
        return getGridAtMouse(event, gridStorage, parent->grids[section], childPos.x, childPos.y);
    }

    GridAccessor accessor {
        .grid = parent,
        .depth = parent->depth,
        .section = section
    };
    printf("Got Grid, Section: %i, Depth: %i (Parent Depth %i)\n", accessor.section, accessor.depth, parent->depth);
    return accessor;
}
std::optional<SparseGrid::GridAccessorParent> SparseGrid::getGridParentAtMouse(Event& event, std::vector<SparseGrid*>& gridStorage, SparseGrid* _parent, SparseGrid* _prevParent, int gridX, int gridY) {
    if (_parent == nullptr) { printf("Parent Grid Is Null, Returning Empty\n"); return {}; }
    SparseGrid* parent = _parent;
    SparseGrid* prevParent = _prevParent;
    int section = 0;
/*
    for (int i = 0; i < s_GridSize; i++) {
        v2<int> size { parent->blockSize.x, parent->blockSize.y };
        v4<int> position {
                gridX + ((i % 2 == 1) ? size.x : 0) + gridOffset.x,
                gridY + ((i > 1) ? size.y : 0) + gridOffset.y,
                parent->blockSize.x, parent->blockSize.y,
        };
        if (rectsCollide((SDL_Rect)position, {mouse.x, mouse.y, 1, 1})) {
            quad = i, depth = parent->depth;
            if (parent->grids[i] != nullptr) {
                getGridAtMouse(event, gridStorage, parent->grids[i], position.x, position.y, depth + 1);
            }
        }
    }
    printf("Mouse Colliding With Section: %i, depth: %i\n", quad, depth);
*/
    SDL_Rect rect {gridX, gridY, parent->blockSize.x * 2, parent->blockSize.y * 2};
    if (!rectsCollide(rect, {event.mx(), event.my(), 1, 1})) return {};

    v2<int> childPos {gridX, gridY};
    auto center = getCenterOfRect(rect);

    if (event.mx() > center.x) { section += 1; childPos.x += parent->blockSize.x; }
    if (event.my() > center.y) { section += 2; childPos.y += parent->blockSize.y; }


    if (parent->grids[section] != nullptr) {
        return getGridParentAtMouse(event, gridStorage, parent->grids[section], parent, childPos.x, childPos.y);
    }


    SparseGrid::GridAccessorParent accessor {};
    accessor.accessor.grid = parent;
    accessor.accessor.depth = parent->depth;
    accessor.accessor.section = section;
    accessor.parent = prevParent;
    return accessor;
}



GridAccessor SparseGrid::loadNewFromParent(std::vector<SparseGrid*>& gridStorage, SparseGrid *parent, int _depth, int section) {
//    printf("Grid Creation Check: INIT\n");
    SparseGrid* parentGrid = parent;
    int i = 0;
    while (i < _depth) {
        if (parentGrid->grids[section] == nullptr) {
//            printf("Grid Creation Check: Child Grid NULL\n");
            //gridStorage.push_back(new SparseGrid());
//            printf("Grid Creation Check: Creating Child Grid\n");
            //SparseGrid* grid = gridStorage.back();
            auto grid = new SparseGrid{};
            (*grid).blockSize.x = parentGrid->blockSize.x / 2;
            (*grid).blockSize.y = parentGrid->blockSize.y / 2;
            (*grid).depth = (parentGrid->depth) + 1;
//            printf("Grid Creation Check: Child Grid Parameters Set\n");
            (*parentGrid).grids[section] = grid;
//            printf("Grid Creation Check: Child Grid Appended\n");
        }
        parentGrid = parentGrid->grids[section];
        i++;
    }

    GridAccessor accessor {
            .grid = parentGrid,
            .depth = depth,
            .section = section
    };

    return accessor;

}

std::array<GridAccessor, 4> SparseGrid::loadAllFromParent(std::vector<SparseGrid*>& gridStorage, SparseGrid *parent) {
    std::array<GridAccessor, 4> grids;
    for (int i = 0; i < 4; i++) {
        grids[i] = loadNewFromParent(gridStorage, parent, 1, i);
    }
    return grids;
}

void SparseGrid::collapseFromParent(std::vector<SparseGrid*>& gridStorage, SparseGrid* parent) {
    if (parent == nullptr) { fprintf(stderr, "Cannot Collapse An Invalid Grid\n"); return; }
    for (auto& grid : parent->grids) {
        if (grid == nullptr) continue;
        grid = nullptr;
        delete grid;
    }
    //parent = nullptr;
    //delete parent;
    //static_assert("Do Not Call This Function");
}



GridAccessor World::loadNewFromGrid(SparseGrid* parent, int depth, int section) {
    SparseGrid* parentGrid = parent;
    const int desiredDepth = depth;
    int i = 0;
    while (i < desiredDepth) {
        if (parentGrid->grids[section] == nullptr) {
            //_subGridStorage.push_back(new SparseGrid{});
            //auto grid = _subGridStorage.back();
            auto grid = new SparseGrid{};
            (*grid).blockSize.x = parentGrid->blockSize.x / 2;
            (*grid).blockSize.y = parentGrid->blockSize.y / 2;
            (*grid).depth = (parentGrid->depth) + 1;
            parentGrid = ((*parentGrid).grids[section] = grid);
        }
        if (parentGrid->grids[section] != nullptr)
            parentGrid = (*parentGrid).grids[section];
        i++;
    }

    GridAccessor accessor {
            .grid = parentGrid,
            .depth = depth,
            .section = section
    };

    return accessor;
}

GridAccessor World::loadNew(int depth, int section) {
    if (depth < 1) return {&_grid, 0};
    return loadNewFromGrid(&_grid, depth, section);
}
std::array<GridAccessor, 4> World::loadAll() {
    std::array<GridAccessor, 4> accessors;
    for (int i = 0; i < 4; i++) {
        accessors[i] = loadNew(1, i);
    }
    return accessors;
}
void World::collapseGrid(SparseGrid *grid) {
    // Check If All Are Empty
    // Or Check If All Blocks Are The Same Type
}

// This Function Seems To Not Increment The Depth Of The Returned Values
GridAccessor GridAccessor::loadNew(std::vector<SparseGrid*>& gridStorage, int _depth, int section) {
    return this->grid->loadNew(gridStorage, _depth, section);
}
std::array<GridAccessor, 4> GridAccessor::loadAll(std::vector<SparseGrid*>& gridStorage) {
    std::array<GridAccessor, 4> accessors;
    for (int i = 0; i < 4; i++) {
        accessors[i] = loadNew(gridStorage, 1, i);
    }
    return accessors;
}

void World::move(int x, int y) {

}
void World::draw(Renderer &renderer, FontRenderer& FontRenderer, Event& event) {
    _grid.draw(renderer, FontRenderer, event, 100, 100);
}
#endif
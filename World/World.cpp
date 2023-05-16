//
// Created by Owner on 3/19/2023.
//

#include "World.hpp"

World::World(int cellsX, int cellsY, int cellsW, int cellsH) : offsetX(0), offsetY(0) {
    for (int x = 0, y = 0; y < cellsY; x++) {
        if (x == cellsX) {
            x = 0;
            y++;
            if (y == cellsY) break;
        }
        Rect r {x * cellsW, y * cellsH, cellsW, cellsH};
        blocks.emplace_back(r, (y == 0) ? BlockType::Grass : BlockType::Dirt);
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
        if (i.second == World::BlockType::None) continue;
        if (!i.first.collides(renderer.getScreenAsRect())) continue;
        renderer.drawRect(i.first);
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
std::pair<Rect, World::BlockType>* World::getBlockAtIndex(size_t index) {
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
std::pair<Rect, World::BlockType>* World::getBlockAtCursor() {
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

v2<int, int> World::getWorldOffset() { return {offsetX, offsetY}; }
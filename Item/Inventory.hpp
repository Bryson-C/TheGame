//
// Created by Owner on 4/3/2023.
//

#ifndef SDL_INVENTORY_HPP
#define SDL_INVENTORY_HPP

#include <optional>
#include <vector>

#include "../Engine/Renderer/Renderer.hpp"
#include "../Item/Item.hpp"

class Inventory {
private:
    size_t _slotsX, _slotsY;
    std::vector<Item> _items;

    size_t _cursorSlot;
    SDL_Rect _cursorPos;
public:
    Inventory(Renderer& renderer, size_t sizeX, size_t sizeY);
    void draw(Renderer& renderer);
    void setSlot(size_t slotX, size_t slotY, Item item);
    void setSlot(size_t slotIndex, Item item);
    Item& getSlot(size_t slotX, size_t slotY);
    Item& getSlot(size_t slotIndex);
    Item& operator[](size_t index);
    void moveCursor(size_t x, size_t y);
    void drawCursor(Renderer& renderer);
};


#endif //SDL_INVENTORY_HPP

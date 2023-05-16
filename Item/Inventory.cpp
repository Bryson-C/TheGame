//
// Created by Owner on 4/3/2023.
//

#include "Inventory.hpp"

static std::optional<Texture> s_InventorySlotTexture{};
static constexpr int32_t s_SlotSizeX = 32, s_SlotSizeY = 32;

Inventory::Inventory(Renderer& renderer, size_t sizeX, size_t sizeY) : _slotsX(sizeX), _slotsY(sizeY) {
    if (!s_InventorySlotTexture.has_value())
        s_InventorySlotTexture = renderer.loadTexture(R"(X:\\SDL\\Asset\\Itemholder.png)");

    _items.resize(_slotsX * _slotsY);
}

void Inventory::draw(Renderer &renderer) {
    size_t loop = 0, xPos = 0;
    for (auto& slot : _items) {
        SDL_Rect slotPos = {
                (int)(xPos * s_SlotSizeX),
                (int)((loop / _slotsX) * s_SlotSizeY),
                s_SlotSizeX,
                s_SlotSizeY
        };
        if (!slot.validItem()) {
            renderer.drawTexture(slotPos, s_InventorySlotTexture.value());
        } else {
            slot.draw(renderer, slotPos);
        }
        loop++;
        xPos++;
        if (xPos == _slotsX) xPos = 0;
    }
}

void Inventory::setSlot(size_t slotX, size_t slotY, Item item) { _items[(slotY * _slotsX) + slotX] = item; }
void Inventory::setSlot(size_t slotIndex, Item item) { _items[slotIndex] = item; }
Item& Inventory::getSlot(size_t slotX, size_t slotY) { return _items[(slotY * _slotsX) + slotX]; }
Item& Inventory::getSlot(size_t slotIndex) { return _items[slotIndex]; }
Item &Inventory::operator[](size_t index) { return _items[index]; }

void Inventory::moveCursor(size_t x, size_t y) {
    _cursorSlot = (y * _slotsX) + x;
    _cursorPos = {(int)x * 32, (int)y * 32,32,32};
}

void Inventory::drawCursor(Renderer &renderer) {
    auto col = renderer.getColor();
    renderer.setColor(255,255,0);
    renderer.drawRect(_cursorPos, {{false}});
    renderer.setColor(col);
}

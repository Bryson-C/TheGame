//
// Created by Owner on 4/3/2023.
//

#include "Inventory.hpp"

static std::optional<Texture> s_InventorySlotTexture{};
static constexpr int32_t s_SlotSizeX = 32, s_SlotSizeY = 32;

Inventory::Inventory(Renderer& renderer, size_t sizeX, size_t sizeY) : _slotsX(sizeX), _slotsY(sizeY), _items(_slotsX * _slotsY), _itemStackSize(_slotsX * _slotsY), _cursorPos({0,0,0,0}),  _renderOffset(0,0) {
    if (!s_InventorySlotTexture.has_value())
        s_InventorySlotTexture = renderer.loadTexture(renderer.getPathToSrc().string() + "\\Asset\\Itemholder.png");
}


void Inventory::setSlot(size_t slotX, size_t slotY, Item item, uint32_t itemCount) {
    auto access = ItemSlotAccessor(*this, slotX, slotY);
    (*access.item) = item;
    (*access.stackSize) = itemCount;
}
void Inventory::setSlot(size_t index, Item item, uint32_t itemCount) {
    auto access = ItemSlotAccessor(*this, index);
    (*access.item) = item;
}
Inventory::ItemSlotAccessor Inventory::getSlot(size_t slotX, size_t slotY) {
    return {*this, slotX, slotY};
}
Inventory::ItemSlotAccessor Inventory::getSlot(size_t index) {
    return {*this, index};
}
Inventory::ItemSlotAccessor Inventory::operator[](size_t index) {
    return {*this, index};
}
Inventory::ItemSlotAccessor Inventory::getFirstEmptySlot() {
    for (size_t index = 0; auto& i : _items) {
        if (!i.validItem()) return {*this, index, InventoryAccessResult::EmptySlotFound};
        index++;
    }
    return {*this, InventoryAccessResult::InventoryFull};
}
Inventory::ItemSlotAccessor Inventory::getFirstSlotOfType(ItemSpawnList::ItemType type) {
    for (size_t index = 0; auto& i : _items) {
        if (i.type() == type) return {*this, index, InventoryAccessResult::TypeFound};
        index++;
    }
    return {*this, InventoryAccessResult::TypeNotFound};
}
Inventory::ItemSlotAccessor Inventory::getFirstSlotOfID(ItemSpawnList::ItemID ID) {
    for (size_t index = 0; auto& i : _items) {
        if (i.id() == ID) return {*this, index, InventoryAccessResult::IDFound};
        index++;
    }
    return {*this, InventoryAccessResult::IDNotFound};
}


void Inventory::moveCursor(size_t x, size_t y) {
    _cursorPos = {(int)x * 32, (int)y * 32,32,32};
}

void Inventory::moveInventoryTo(int x, int y) {
    _renderOffset = {x, y};
}

void Inventory::draw(Renderer &renderer, FontRenderer& numRenderer) {
    size_t loop = 0, xPos = 0;
    for (auto& slot : _items) {
        SDL_Rect slotPos = {
                (int)(xPos * s_SlotSizeX) + _renderOffset.x,
                (int)((loop / _slotsX) * s_SlotSizeY) + _renderOffset.y,
                s_SlotSizeX,
                s_SlotSizeY
        };
        renderer.drawTexture(slotPos, s_InventorySlotTexture.value());
        if (slot.validItem()) {
            slot.draw(renderer, slotPos);

        }
        loop++;
        xPos++;
        if (xPos == _slotsX) xPos = 0;
    }
    loop = 0, xPos = 0;
    for (const auto& count : _itemStackSize) {
        if (!_items[loop].validItem()) {
            loop++;
            xPos++;
            continue;
        }
        SDL_Rect slotPos = {
                (int)(xPos * s_SlotSizeX) + _renderOffset.x,
                (int)((loop / _slotsX) * s_SlotSizeY) + _renderOffset.y,
                s_SlotSizeX,
                s_SlotSizeY
        };
        auto slotCenter = getCenterOfRect(slotPos);
        FontRenderer::FontProperties props;
        props.renderAsOutlined = true;
        props.characterSize = 16;
        props.kerning = -2 * (props.characterSize/8);
        numRenderer.drawNumber(renderer, count, slotPos.x, slotCenter.y, props);
        loop++;
        xPos++;
        if (xPos == _slotsX) xPos = 0;
    }
}

void Inventory::drawCursor(Renderer &renderer) {
    auto col = renderer.getColor();
    renderer.setColor(255,255,0);
    SDL_Rect pos = _cursorPos;
    pos.x += _renderOffset.x;
    pos.y += _renderOffset.y;
    renderer.drawRect(pos, {{false}});
    renderer.setColor(col);
}

void Inventory::ItemSlotRangeAccessor::draw(Renderer& renderer) {
    size_t loop = 0, xPos = 0;
    size_t _slotsX = slots.size(), _slotsY = 1;
    for (auto& slot : slots) {
        SDL_Rect slotPos = {
                (int)(xPos * s_SlotSizeX) + renderOffset.x,
                (int)((loop / _slotsX) * s_SlotSizeY) + renderOffset.y,
                s_SlotSizeX,
                s_SlotSizeY
        };
        if (!slot.item->validItem()) {
            renderer.drawTexture(slotPos, s_InventorySlotTexture.value());
        } else {
            slot.item->draw(renderer, slotPos);
        }
        loop++;
        xPos++;
        if (xPos == _slotsX) xPos = 0;
    }
}

void Inventory::ItemSlotRangeAccessor::drawCursor(Renderer &renderer) {
    auto col = renderer.getColor();
    renderer.setColor(255,255,0);
    SDL_Rect pos = cursorPos;
    pos.x += renderOffset.x;
    pos.y += renderOffset.y;
    renderer.drawRect(pos, {{false}});
    renderer.setColor(col);
}

void Inventory::ItemSlotRangeAccessor::moveCursor(int x, int y) {
    cursorPos = {(int)x * 32, (int)y * 32,32,32};
}


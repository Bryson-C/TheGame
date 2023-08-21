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
    std::vector<uint32_t> _itemStackSize;

    SDL_Rect _cursorPos;
    v2<int> _renderOffset;

public:
    Inventory(Renderer& renderer, size_t sizeX, size_t sizeY);
    enum class InventoryAccessResult {
        None,
        EmptySlotFound,
        InventoryFull,
        TypeFound,
        TypeNotFound,
        IDFound,
        IDNotFound,
        FullOrTypeNotFound
    };

    struct ItemSlotAccessor {
        Item* item;
        uint32_t* stackSize;
        InventoryAccessResult error;

        ItemSlotAccessor(Inventory& inventory, size_t index, InventoryAccessResult result = InventoryAccessResult::None)
        {
            item = &(inventory._items[index]);
            stackSize = &(inventory._itemStackSize[index]);
            error = result;
        }

        ItemSlotAccessor(Inventory& inventory, size_t slotX, size_t slotY, InventoryAccessResult result = InventoryAccessResult::None)
        {
            item = &(inventory._items[(slotY * inventory._slotsX) + slotX]);
            stackSize = &(inventory._itemStackSize[(slotY * inventory._slotsX) + slotX]);
            error = result;
        }

        ItemSlotAccessor(Inventory& inventory, InventoryAccessResult error) : item(nullptr), stackSize(nullptr) {
            (*this).error = error;
            if (error != InventoryAccessResult::None)
                printf("Accessing Inventory: Assigning As Error!\n");
        }
    };

    // Like `ItemSlotAccessor`, But It Selects Multiple Slots And Returns That Portion Of The Inventory,
    // Basically A Slice Of The Inventory
    struct ItemSlotRangeAccessor {

        std::vector<ItemSlotAccessor> slots;
        v2<int> renderOffset;
        SDL_Rect cursorPos;

        ItemSlotRangeAccessor(Inventory& inventory, size_t index, size_t count)
            : slots(count - index, {inventory, InventoryAccessResult::None}),
              renderOffset(0,0)
        {
            for (int i = 0; i < count - index; i++) {
                slots[i] = inventory[i + index];
            }
        }
        void draw(Renderer& renderer);
        void drawCursor(Renderer& renderer);
        void moveCursor(int x, int y);
    };

    void setSlot(size_t slotX, size_t slotY, Item item, uint32_t itemCount = 1);
    void setSlot(size_t slotIndex, Item item, uint32_t itemCount = 1);
    ItemSlotAccessor getSlot(size_t slotX, size_t slotY);
    ItemSlotAccessor getSlot(size_t slotIndex);
    ItemSlotAccessor operator[](size_t index);
    ItemSlotAccessor getFirstEmptySlot();
    ItemSlotAccessor getFirstSlotOfType(ItemSpawnList::ItemType type);
    ItemSlotAccessor getFirstSlotOfID(ItemSpawnList::ItemID ID);


    void moveCursor(size_t x, size_t y);
    void moveInventoryTo(int x, int y);

    void draw(Renderer& renderer, FontRenderer& FontRenderer);
    void drawCursor(Renderer& renderer);

    inline void debugPrint() {
        for (int loop = 0; const auto& item : _items) {
            if (item.id() == ItemSpawnList::ItemID::None) { loop++; continue; }
            printf(" %i|ID:%i,#:%i|", loop, item.id(), _itemStackSize[loop]);
            if (loop%_slotsX==0) printf("\n");
            loop++;
        }
    }
};


#endif //SDL_INVENTORY_HPP

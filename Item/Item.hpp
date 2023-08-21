//
// Created by Owner on 3/19/2023.
//

#ifndef SDL_ITEM_HPP
#define SDL_ITEM_HPP

#include "../Engine/Utility/Rect/Rect.hpp"
#include "../Engine/Renderer/Renderer.hpp"
#include "../Engine/Utility/Types/Types.hpp"
#include "../Engine/Utility/Timer/Timer.hpp"
#include "../Engine/Renderer/Animation/Animate.hpp"

namespace ItemSpawnList {
    enum class ItemType {
        None,
        Block,
        Weapon,
        Tool,
        MagicalItem
    };
    enum class ItemID {
        None,
        GrassBlock,
        DirtBlock,
        StoneBlock,
        TestPickaxe,
        TestSword,
        TestTome,
    };
    void Despawn(Item& item, Renderer& renderer);
}


class Item {
private:
    ItemSpawnList::ItemType _type;
    ItemSpawnList::ItemID _id;
    Rect _rect;
    Timer useTimer{};
    time_t _swingTimer;

    struct ToolStats {
        int32_t toolStrength = 40;
    } toolStats;



    friend void ItemSpawnList::Despawn(Item& item, Renderer& renderer);
    friend class PlayerController;
public:
    explicit Item(
            ItemSpawnList::ItemType type = ItemSpawnList::ItemType::None,
            ItemSpawnList::ItemID id = ItemSpawnList::ItemID::None,
            Texture texture = {}, SDL_Rect rect = {0,0,0,0});


    explicit operator Rect() const;
    inline Rect getRect() { return _rect; }
    ItemSpawnList::ItemID id() const;
    ItemSpawnList::ItemType type() const;
    bool validItem();

    inline Texture getTexture(size_t index = 0) { return _rect._textures[index]; }
    inline void setTexture(Texture texture) { _rect.addTexture({texture}); }
    void draw(Renderer& renderer, SDL_Rect dst = {0,0,0,0}, std::optional<Renderer::TextureDrawProperties> props = {});
    inline float getRotation() { return _rect._rot; }

    inline ToolStats getToolStats() const { return toolStats; }
};

namespace ItemSpawnList {
    Item Spawn(ItemID id, Renderer& renderer);
}
namespace SelectedItem {
    static bool isHoldingItem;
    void SelectItem(Item& item, Renderer& renderer);
    void DropSelectedItem(Item& item, Renderer& renderer);
    void DrawSelectedItem(Renderer& renderer, std::optional<SDL_Rect> drawPos = {});
}


#endif //SDL_ITEM_HPP

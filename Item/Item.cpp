//
// Created by Owner on 3/19/2023.
//

#include "Item.hpp"

Item::Item(ItemSpawnList::ItemType type, ItemSpawnList::ItemID id, Texture texture, SDL_Rect rect) : _type(type), _id(id), _rect(rect), _swingTimer(clock()) {
    _rect.addTexture({texture});
    if (_type == ItemSpawnList::ItemType::Tool || _type == ItemSpawnList::ItemType::Weapon)
        _rect._rotOrigin = {_rect.pos.x, _rect.pos.h};
}
Item::operator Rect() const {
    return _rect;
}
void Item::draw(Renderer &renderer, SDL_Rect dst, std::optional<Renderer::TextureDrawProperties> props) {
    using namespace ItemSpawnList;
    if (_type == ItemType::None || _id == ItemID::None) {
            renderer.drawRect(_rect);
        return;
    }
    if (dst.w > 0 && dst.h > 0)
        renderer.drawTexture(dst, _rect._textures[_rect._textureIndex], props);
    else
        renderer.drawRect(_rect);
}
ItemSpawnList::ItemID Item::id() const { return _id; }
ItemSpawnList::ItemType Item::type() const { return _type; }
bool Item::validItem() {
    return (_id != ItemSpawnList::ItemID::None && _type != ItemSpawnList::ItemType::None);
}


Item ItemSpawnList::Spawn(ItemSpawnList::ItemID id, Renderer &renderer) {
    switch (id) {
        case ItemID::GrassBlock:
            return Item(ItemType::Block, ItemID::GrassBlock, renderer.loadTexture(R"(X:\SDL\Asset\Tileset64.png)", 0, 0, 64, 64), {0,0,64,64});
        case ItemID::DirtBlock:
            return Item(ItemType::Block, ItemID::DirtBlock, renderer.loadTexture(R"(X:\SDL\Asset\Tileset64.png)", 64, 0, 64, 64), {0,0,64,64});
        case ItemID::StoneBlock:
            return Item(ItemType::Block, ItemID::StoneBlock, renderer.loadTexture(R"(X:\SDL\Asset\Tileset64.png)", 128, 0, 64, 64), {0,0,64,64});
        case ItemID::TestPickaxe:
            return Item(ItemType::Tool, ItemID::TestPickaxe, renderer.loadTexture(R"(X:\SDL\Asset\axe.jpg)", 0, 0, 200, 200), {0,0,64,64});
        case ItemID::TestSword:
            return Item(ItemType::Weapon, ItemID::TestSword, renderer.loadTexture(R"(X:\SDL\Asset\sword.png)", 0, 0, 32, 32), {0,0,64,64});
        default:
            std::cerr << "Cannot Find Item Of Id: " << (int)id << "\n";
    }
    return Item(ItemType::None, ItemID::None);
}
void ItemSpawnList::Despawn(Item &item, Renderer& renderer) {
    for (auto texture : item._rect.getTextureList()) {
        renderer.unloadTexture(texture);
    }
    item._type = ItemType::None;
    item._id = ItemID::None;
}

static Item selectedItem(ItemSpawnList::ItemType::None, ItemSpawnList::ItemID::None);
static constexpr int pickupDelay = 150;
static Timer pickupTimer{};
static bool isHoldingItem = false;

void SelectedItem::SelectItem(Item& item, Renderer& renderer) {
    if (!item.validItem()) return;
    if (pickupTimer.isCompleteReset(pickupDelay)) {
        auto replace = selectedItem;
        selectedItem = item;
        item = replace;
        isHoldingItem = true;
    }
}
void SelectedItem::DropSelectedItem(Item& item, Renderer& renderer) {
    if (!selectedItem.validItem()) return;
    if (pickupTimer.isCompleteReset(pickupDelay)) {
        if (item.validItem()) {
            SelectItem(item, renderer);
            return;
        }
        auto replace = item;
        item = selectedItem;
        selectedItem = replace;
        isHoldingItem = false;
    }
}
void SelectedItem::DrawSelectedItem(Renderer& renderer, std::optional<SDL_Rect> drawPos) {
    if (!selectedItem.validItem()) return;

    if (drawPos.has_value())
        selectedItem.draw(renderer, drawPos.value());
    else
        selectedItem.draw(renderer);
}


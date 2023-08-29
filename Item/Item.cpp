//
// Created by Owner on 3/19/2023.
//

#include "Item.hpp"


Item::Item(ItemSpawnList::ItemType type, int id, Texture texture, SDL_Rect rect) : _type(type), _id(id), _rect(rect), _swingTimer(clock()) {
    _rect.addTexture({texture});
    if (_type == ItemSpawnList::ItemType::Tool || _type == ItemSpawnList::ItemType::Weapon)
        _rect._rotOrigin = {_rect.pos.x, _rect.pos.h};
}
Item::operator Rect() const {
    return _rect;
}
void Item::draw(Renderer &renderer, SDL_Rect dst, std::optional<Renderer::TextureDrawProperties> props) {
    using namespace ItemSpawnList;
    if (_type == ItemType::None || _id == 0) {
            renderer.drawRect(_rect);
        return;
    }
    if (dst.w > 0 && dst.h > 0)
        renderer.drawTexture(dst, _rect._textures[_rect._textureIndex], props);
    else
        renderer.drawRect(_rect);
}
size_t Item::id() const { return _id; }
ItemSpawnList::ItemType Item::type() const { return _type; }
bool Item::validItem() {
    return (_id != 0 && _type != ItemSpawnList::ItemType::None);
}


Item ItemSpawnList::Spawn(ItemSpawnList::ItemID id, Renderer &renderer) {
    const std::string pathToSrc = renderer.getPathToSrc().string();
    switch (id) {
        case ItemID::GrassBlock:
            return Item(ItemType::Block, (int)ItemID::GrassBlock, renderer.loadTexture(pathToSrc + "\\Asset\\Tileset64.png", 0, 0, 64, 64), {0,0,64,64});
        case ItemID::DirtBlock:
            return Item(ItemType::Block, (int)ItemID::DirtBlock, renderer.loadTexture(pathToSrc + "\\Asset\\Tileset64.png", 64, 0, 64, 64), {0,0,64,64});
        case ItemID::StoneBlock:
            return Item(ItemType::Block, (int)ItemID::StoneBlock, renderer.loadTexture(pathToSrc + "\\Asset\\Tileset64.png", 128, 0, 64, 64), {0,0,64,64});
        case ItemID::TestPickaxe:
            return Item(ItemType::Tool, (int)ItemID::TestPickaxe, renderer.loadTexture(pathToSrc + "\\Asset\\pickaxe.png", 0, 0, 64, 64), {0,0,64,64});
        case ItemID::TestSword:
            return Item(ItemType::Weapon, (int)ItemID::TestSword, renderer.loadTexture(pathToSrc + "\\Asset\\sword.png", 0, 0, 64, 64), {0,0,64,64});
        case ItemID::TestTome:
            return Item(ItemType::MagicalItem, (int)ItemID::TestTome, renderer.loadTexture(pathToSrc + "\\Asset\\Spell_Tome.png", 0, 0, 32, 32), {0,0,64,64});
        default:
            std::cerr << "Cannot Find Item Of Id: " << (int)id << "\n";
    }
    return Item(ItemType::None, (int)ItemID::None);
}

ItemRegistry s_ItemRegistry = {};

Item ItemSpawnList::SpawnFromRegistry(std::string key, Renderer &renderer) {
    if (s_ItemRegistry.contains(key)) {
        auto itemData = s_ItemRegistry[key];
        auto texturePath = renderer.getPathToSrc().string() + itemData.textureSrcPath;
        auto texture = renderer.loadTexture(texturePath, itemData.pos.x, itemData.pos.y, itemData.pos.z, itemData.pos.w);
        printf("Spawning '%s'(ID: %i) As Type: %i\n", itemData.name.c_str(), itemData.itemID, (int)itemData.itemType);
        return Item(itemData.itemType, (int) itemData.itemID, texture, (SDL_Rect) itemData.pos);
    }
    printf("Couldn't find key '%s'\n", key.c_str());
    return Item(ItemType::None, (int)ItemID::None);
}

void ItemSpawnList::Despawn(Item &item, Renderer& renderer) {
    for (auto texture : item._rect.getTextureList()) {
        renderer.unloadTexture(texture);
    }
    item._type = ItemType::None;
    item._id = (int)ItemType::None;
}

static Item selectedItem(ItemSpawnList::ItemType::None, (int)ItemSpawnList::ItemID::None);
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


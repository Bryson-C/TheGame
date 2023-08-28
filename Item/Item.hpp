//
// Created by Owner on 3/19/2023.
//

#ifndef SDL_ITEM_HPP
#define SDL_ITEM_HPP

#include <iostream>
#include <fstream>

#include "../Engine/Utility/Rect/Rect.hpp"
#include "../Engine/Renderer/Renderer.hpp"
#include "../Engine/Utility/Types/Types.hpp"
#include "../Engine/Utility/Timer/Timer.hpp"
#include "../Engine/Renderer/Animation/Animate.hpp"

#include "../Engine/Utility/json.hpp"

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


class RuntimeEnum {
    std::map<std::string, int> enumValue;
public:
    int getEnum(std::string str) { return enumValue[str]; }
    int operator[](std::string key) { return enumValue[key]; }
    void addEnum(std::string key) {
        enumValue.insert({key, enumValue.size() - 1});
    }
};

static RuntimeEnum s_ItemRegistry{};
struct ItemRegistryData {
    std::string name;
    std::string desc;
    std::string itemID;
    std::string itemType;
    v4<int> pos;
    std::string textureSrcPath;
    v4<int> texturePos;
};
static std::vector<ItemRegistryData> s_ItemRegistryData {};

inline void AddItemToItemRegistry(std::string key, void* data = nullptr) {
    s_ItemRegistry.addEnum(key);
}

static std::string fileToString(const std::filesystem::path& path) {
    std::ifstream file(path);
    std::string buffer, result;
    while (std::getline(file, buffer)) {
        if (!buffer.empty())
            result += buffer;
    }
    return result;
}

static std::string ItemDataPath;
inline void LoadItemRegistryFromJson() {
    const auto itemLoadPath = std::filesystem::current_path().string() + "\\ItemData\\Items.json";
    std::cout << "Loading Items From: " << itemLoadPath << "\n";

    std::string itemDataAsString = fileToString(itemLoadPath);
    std::cout << itemDataAsString << "\n";

    auto jsonData = nlohmann::json::parse(itemDataAsString);
    printf("Json Data Size %zi\n", jsonData.size());
    size_t itemLoop = 0;
    for (const auto& data : jsonData) {
        ItemRegistryData itemData;
        itemData.name = "BAD_ITEM_" + std::to_string(itemLoop);
        itemData.desc = "BAD_DESC_" + std::to_string(itemLoop);
        itemData.itemID = "None";
        itemData.itemType = "None";
        itemData.pos = {0, 0, 64, 64};
        itemData.textureSrcPath = "\\Asset\\Tileset64.png";
        itemData.texturePos = {64 * 3, 0, 64, 64};

        if (data.contains("name")) {
            itemData.name = data["name"].get<std::string>();
        }
        if (data.contains("id")) {
            itemData.itemID = data["id"].get<std::string>();
        }
        if (data.contains("type")) {
            itemData.itemType = data["id"].get<std::string>();
        }
        if (data.contains("itemPos")) {
            itemData.pos = {
                    data["itemPos"][0],
                    data["itemPos"][1],
                    data["itemPos"][2],
                    data["itemPos"][3],
             };
        }
        if (data.contains("desc")) {
            itemData.desc = data["desc"].get<std::string>();
        }
        if (data.contains("textureSrc")) {
            itemData.desc = data["textureSrc"].get<std::string>();
        }
        if (data.contains("texturePos")) {
            itemData.pos = {
                    data["texturePos"][0],
                    data["texturePos"][1],
                    data["texturePos"][2],
                    data["texturePos"][3],
            };
        }
        s_ItemRegistry.addEnum(itemData.name);
        s_ItemRegistryData.push_back(itemData);
        printf("Item \"%s\" Added As: %s \n", itemData.name.c_str(), itemData.itemID.c_str());
        itemLoop++;
    }


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
    Item SpawnFromRegistry(std::string key, Renderer& renderer);
}
namespace SelectedItem {
    static bool isHoldingItem;
    void SelectItem(Item& item, Renderer& renderer);
    void DropSelectedItem(Item& item, Renderer& renderer);
    void DrawSelectedItem(Renderer& renderer, std::optional<SDL_Rect> drawPos = {});
}

#endif //SDL_ITEM_HPP

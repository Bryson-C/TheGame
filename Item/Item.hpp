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

struct ItemRegistryData {
    std::string name;
    std::string desc;
    size_t itemID;
    ItemSpawnList::ItemType itemType;
    static ItemSpawnList::ItemType parseStringToEnum(std::string typeString) {
        if (typeString == "Block") { return ItemSpawnList::ItemType::Block; }
        else if (typeString == "Weapon") { return ItemSpawnList::ItemType::Weapon; }
        else if (typeString == "Tool") { return ItemSpawnList::ItemType::Tool; }
        else if (typeString == "MagicalItem") { return ItemSpawnList::ItemType::MagicalItem; }
        return ItemSpawnList::ItemType::None;
    }
    v4<int> pos;
    std::string textureSrcPath;
    v4<int> texturePos;
};

class ItemRegistry {
    std::map<std::string, ItemRegistryData> enumValue;
public:
    ItemRegistryData getEnum(std::string str) { return enumValue[str]; }
    ItemRegistryData operator[](std::string key) { return enumValue[key]; }
    void addEntry(std::string key, ItemRegistryData data) { enumValue.insert({key, data}); }
    bool contains(std::string key) { return enumValue.contains(key); }
    void debugPrint() {
        for (const auto& i : enumValue) {
            std::cout << i.first << " | " << i.second.itemID << "\n";
        }
    }
};




static std::string fileToString(const std::filesystem::path& path) {
    std::ifstream file(path);
    std::string buffer, result;
    while (std::getline(file, buffer)) {
        if (!buffer.empty())
            result += buffer;
    }
    return result;
}



namespace {
    namespace JsonHandler {
        enum class JsonHandlerError {
            Success,
            Failure,
            FieldNotPresent,
        };
        template<typename Type>
        struct JsonResult {
            Type data;
            JsonHandlerError err;
            Type defaultOnError(Type _default) {
                if (err != JsonHandlerError::Success) {
                    return _default;
                }
                return data;
            }
        };

        // Cannot Handle Arrays Quite Yet
        template<typename Type>
        JsonResult<Type> getField(std::string key, nlohmann::json jsonData) {
            if (jsonData.contains(key)) {
                return {jsonData[key], JsonHandlerError::Success};
            }
            return {Type(), JsonHandlerError::FieldNotPresent};
        }

    }
}

extern ItemRegistry s_ItemRegistry;

static std::string ItemDataPath;
inline void LoadItemRegistryFromJson() {
    const auto itemLoadPath = std::filesystem::current_path().string() + "\\ItemData\\Items.json";
    std::cout << "Loading Items From: " << itemLoadPath << "\n";

    std::string itemDataAsString = fileToString(itemLoadPath);
    std::cout << itemDataAsString << "\n";

    auto jsonData = nlohmann::json::parse(itemDataAsString);
    size_t itemLoop = 0;
    for (const auto& data : jsonData) {
        ItemRegistryData itemData;
        itemData.pos = {0, 0, 64, 64};
        itemData.texturePos = {64 * 3, 0, 64, 64};

        itemData.name = JsonHandler::getField<std::string>("name", data).defaultOnError("BAD_ITEM_" + std::to_string(itemLoop));
        itemData.desc = JsonHandler::getField<std::string>("desc", data).defaultOnError("BAD_DESC_" + std::to_string(itemLoop));
        itemData.itemID = JsonHandler::getField<int>("id", data).defaultOnError(0);
        itemData.itemType = ItemRegistryData::parseStringToEnum(JsonHandler::getField<std::string>("type", data).defaultOnError("None"));
        // JsonHandler::getFeild Cannot Handle Arrays Yet
        if (data.contains("itemPos")) {
            itemData.pos = { data["itemPos"][0], data["itemPos"][1], data["itemPos"][2], data["itemPos"][3] };
        }
        itemData.textureSrcPath = JsonHandler::getField<std::string>("textureSrc", data).defaultOnError("\\Asset\\Tileset64.png");
        // JsonHandler::getFeild Cannot Handle Arrays Yet
        if (data.contains("texturePos")) {
            itemData.pos = { data["texturePos"][0], data["texturePos"][1], data["texturePos"][2], data["texturePos"][3] };
        }
        s_ItemRegistry.addEntry(itemData.name, itemData);
        printf("Item \"%s\" Added As: %i \n", itemData.name.c_str(), (int)itemData.itemID);
        itemLoop++;
    }


}





class Item {
private:
    ItemSpawnList::ItemType _type;
//    ItemSpawnList::ItemID _id;
    size_t _id;
    Rect _rect;
    Timer useTimer{};
    time_t _swingTimer;

    struct ToolStats {
        int32_t toolStrength = 40;
    } toolStats;



    friend void ItemSpawnList::Despawn(Item& item, Renderer& renderer);
    friend class PlayerController;
public:
    Item(
            ItemSpawnList::ItemType type = ItemSpawnList::ItemType::None,
            //ItemSpawnList::ItemID id = ItemSpawnList::ItemID::None,
            int id = 0,
            Texture texture = {}, SDL_Rect rect = {0,0,0,0});


    explicit operator Rect() const;
    inline Rect getRect() { return _rect; }
    size_t id() const;
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

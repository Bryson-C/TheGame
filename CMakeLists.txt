cmake_minimum_required(VERSION 3.23)
project(SDL)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Runtime)

# SDL
include_directories(${PROJECT_SOURCE_DIR}/lib/SDL/include)
link_directories(${PROJECT_SOURCE_DIR}/lib/SDL/lib/x64)

# SDL Image
include_directories(${PROJECT_SOURCE_DIR}/lib/SDLImage/include)
link_directories(${PROJECT_SOURCE_DIR}/lib/SDLImage/lib/x64)

# Duktape (JS Engine)
#include_directories(${PROJECT_SOURCE_DIR}/lib/Duktape)

# ImGui
include_directories(${PROJECT_SOURCE_DIR}/lib/ImGui)
file(GLOB IMGUI_FILES_H ${PROJECT_SOURCE_DIR}/lib/ImGui/*.h)
file(GLOB IMGUI_FILES_CPP ${PROJECT_SOURCE_DIR}/lib/ImGui/*.cpp)


add_executable(SDL main.cpp

			Engine/Utility/json.hpp
# I Dont Need A JS Engine Yet
#            lib/Duktape/duk_config.h
#            lib/Duktape/duktape.h
#            lib/Duktape/duktape.c

# ImGui


            ${IMGUI_FILES_H}
            ${IMGUI_FILES_CPP}
            lib/ImGui/backends/imgui_impl_sdl2.h
            lib/ImGui/backends/imgui_impl_sdl2.cpp
            lib/ImGui/backends/imgui_impl_sdlrenderer2.h
            lib/ImGui/backends/imgui_impl_sdlrenderer2.cpp
# end ImGui
            Engine/Renderer/Renderer.hpp
            Engine/Renderer/Renderer.cpp
            Engine/Renderer/Event.hpp
            Engine/Renderer/Event.cpp
            Engine/Renderer/Texture.hpp
            Engine/Renderer/Texture.cpp
            Engine/Renderer/DrawColor.hpp
            Engine/Renderer/DrawColor.cpp
            Engine/Renderer/Scene/Scene.hpp
            Engine/Renderer/Scene/Scene.cpp
            Engine/Renderer/Scene/World/World.hpp
            Engine/Renderer/Scene/World/World.cpp

            Engine/Utility/Timer/Timer.hpp
            Engine/Utility/Timer/Timer.cpp

            Engine/Utility/Rect/Rect.hpp
            Engine/Utility/Rect/Rect.cpp

            Engine/UI/UI.hpp
            Engine/UI/UI.cpp

            Engine/Renderer/Animation/Animate.hpp
            Engine/Renderer/Animation/Animate.cpp



            Item/Item.hpp
            Item/Item.cpp
            Item/Inventory.hpp
            Item/Inventory.cpp

            Engine/Utility/Types/Types.hpp
            Engine/Utility/Types/Types.cpp

            #Engine/Scripting/ScriptEngine.hpp
            #Engine/Scripting/ScriptEngine.cpp
            #Engine/Scripting/SaveLoad.hpp
            #Engine/Scripting/SaveLoad.cpp

            Game/Gameplay.hpp
            Game/Gameplay.cpp

            Game/Player/Player.hpp
            Game/Player/Player.cpp
            Game/Player/PlayerController.hpp
            Game/Player/PlayerController.cpp
            Game/Enemy/Enemy.hpp
            Game/Enemy/Enemy.cpp
            Game/AnimationController.hpp
            Game/AnimationController.cpp
        )

target_link_libraries(SDL
            PUBLIC SDL2.lib
            PUBLIC SDL2_image.lib
        )

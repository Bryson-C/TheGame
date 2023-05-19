//
// Created by Owner on 5/15/2023.
//

#include "Scene.hpp"

static v2<int32_t>* sg_SceneRenderOffset = nullptr;

namespace {
    v2<int32_t> activeScene() { return Scene::getActiveSceneRenderOffset(); }
    v2<int32_t>* pActiveScene() { return sg_SceneRenderOffset; }
}

v2<int32_t> Scene::getActiveSceneRenderOffset() { return *sg_SceneRenderOffset; }

SDL_Rect getAABBFromSceneView(Rect& r) {
    v2<int> offset = {0,0};
    if (pActiveScene() != nullptr) { offset = activeScene(); }
    return {r.pos.x + offset.x, r.pos.y + offset.y, r.pos.w, r.pos.h};
}
SDL_Rect getAABBFromSceneView(SDL_Rect r) {
    v2<int> offset = {0,0};
    if (pActiveScene() != nullptr) { offset = (activeScene()); }
    return {r.x + offset.x, r.y + offset.y, r.w, r.h};
}

v2<int32_t> &Scene::getSceneOffset() { return _Offset; }

void Scene::setActiveScene() { sg_SceneRenderOffset = &_Offset; }
void Scene::moveWorld(int x, int y) { _Offset.x += x, _Offset.y += y; }
void Scene::moveWorld(v2<int> vec) { _Offset.x += vec.x, _Offset.y += vec.y; }
void Scene::moveWorldTo(int x, int y) { _Offset.x = x, _Offset.y = y; }
void Scene::moveWorldTo(v2<int> vec) { _Offset.x = vec.x, _Offset.y = vec.y; }
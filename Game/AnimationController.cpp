//
// Created by Owner on 5/24/2023.
//

#include "AnimationController.hpp"

void AnimationController::addNewAnimationSet(std::vector<Texture>& textureArray) {
    _Animations.push_back(textureArray);
}

void AnimationController::setAnimationArray(std::vector<std::vector<Texture>> &textureArray) {
    _Animations = textureArray;
}

void AnimationController::setAnimationIndex(size_t index) {
    _SelectedAnimations = index;
    if (_AnimationIndex > _Animations[_SelectedAnimations].size() - 1) {
        _AnimationIndex = 0;
    }
}

void AnimationController::drawFrame(Renderer& renderer, SDL_Rect dstRect, std::optional<Renderer::TextureDrawProperties> props) {
    //renderer.drawTexture(dstRect, _Animations[_SelectedAnimations][_AnimationIndex], props);
    renderer.drawTexture(dstRect, getFrameTexture(), props);
}

void AnimationController::nextFrame() {
    _AnimationIndex++;
    if (_AnimationIndex >= _Animations[_SelectedAnimations].size()) _AnimationIndex = 0;
    if (_AnimationIndex < 0) _AnimationIndex = _Animations[_SelectedAnimations].size()-1;
}

void AnimationController::previousFrame() {
    _AnimationIndex--;
    if (_AnimationIndex >= _Animations[_SelectedAnimations].size()) _AnimationIndex = 0;
    if (_AnimationIndex < 0) _AnimationIndex = _Animations[_SelectedAnimations].size()-1;
}

void AnimationController::setFrame(size_t index) {
    _AnimationIndex = index;
}
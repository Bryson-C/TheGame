//
// Created by Owner on 5/24/2023.
//

#ifndef SDL_ANIMATIONCONTROLLER_HPP
#define SDL_ANIMATIONCONTROLLER_HPP

#include "../Engine/Renderer/Texture.hpp"
#include "../Engine/Renderer/Renderer.hpp"

class AnimationController {
private:
    // 2d Resizeable Array Of Textures
    std::vector<std::vector<Texture>> _Animations{};
    size_t _SelectedAnimations = 0, _AnimationIndex = 0;
public:
    // This Adds The `textureArray` Element To The `_Animations` Array
    // After It Is Added It Can Be Selected With `setFrameIndex(...)`
    void addNewAnimationSet(std::vector<Texture>& textureArray);
    // This Sets `_Animations` To The `textureArray` Parameter
    void setAnimationArray(std::vector<std::vector<Texture>>& textureArray);
    // This Sets Which Array Of Animations You Want To Use
    // If I initialized The `_Animations` Array At Index 1 With The Walking Animation,
    // I Would Set The `index` Parameter To 1 To Use That Set Of Animations
    // This Will Not Check The Bounds Of The Array Nor Will It Wrap Around To The Min Or Max Of The Animations Array
    void setAnimationIndex(size_t index);
    // This Draws The Current Frame In The Selected Animation Array
    void drawFrame(Renderer& renderer, SDL_Rect dstRect, std::optional<Renderer::TextureDrawProperties> props = {});
    // This Advances To The Next Frame In The Animation Array
    // It Will Wrap Around To The Front Or Back Of The Array If The Index Is Exceeded Or Is Less Than 0
    void nextFrame();
    // This Goes Back One Frame
    // Like `drawFrame(...)` It Will Wrap So That No Out-Of-Bounds Exceptions Occur`
    void previousFrame();
    // This Sets The Frame
    void setFrame(size_t index);
    // Get The `_AnimationIndex` Variable
    inline size_t getFrame() { return _AnimationIndex; }
    inline size_t getAnimationSetFrameCount() { return _Animations[_SelectedAnimations].size(); }
    inline size_t getAnimationSet() { return _SelectedAnimations; }
    inline size_t getAnimationSetCount() { return _Animations.size(); }

    inline Texture& getFrameTexture() { return _Animations[_SelectedAnimations][_AnimationIndex]; }
};


#endif //SDL_ANIMATIONCONTROLLER_HPP

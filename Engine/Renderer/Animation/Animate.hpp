//
// Created by Owner on 3/5/2023.
//

#ifndef SDL_ANIMATE_HPP
#define SDL_ANIMATE_HPP


#include "../../Utility/Timer/Timer.hpp"
/*
class Animate {
private:
    Timer _time;
    size_t _speed;
    int32_t* _object;
    int32_t _condition;
    size_t _gIndex;
    bool _complete;

    bool paused = false;
    void step(size_t& stepIter);
    friend void PlayAnimations();
public:
    Animate(int32_t* obj, size_t speed_ms, int32_t condition);
    void pause();
    void resume();
    bool isComplete();
};

void PlayAnimations();
*/

#include <iostream>
#include <optional>
#include <variant>
#include <typeinfo>
#include <vector>


// TODO: Move This To Animation.cpp + .hpp In Engine Folder
// TODO: Merge AnimateI And Animate Functions Into 1 Function


namespace Animation {
    enum class ValType {
        INT,
        FLT
    };
}

template<typename T>
struct AnimationComponent {
private:
    size_t currentAnimationFrame = 0;
    size_t currentKeyFrame = 0;

    friend void updateAnimator();
    //friend void updateAnimatorI();
public:
    T* value;
    int32_t* animationIsCompleteCounter;
    struct Keyframe {
        inline Keyframe(int32_t _frameTime, float _increment, std::optional<float> _set = {}) :
            frameTime(_frameTime), increment(_increment), set(_set) {}
        // How Many Frames Will Be Spent On This Keyframe
        int32_t frameTime;
        // How Much The `value` Variable Will Be Increased (Can Be Negative)
        T increment;
        // If Set The `value` Will Be Set To This Instead Of Incremented
        std::optional<float> set;

        void print() {
            printf("KeyframeData:\n\tFrames In Animation: %i\n\t%s: %i\n",
                   frameTime,
                   (set.has_value() ? "Set" : "Increment"),
                   (set.has_value() ? set.value() : increment)
           );
        }
    };

    std::vector<Keyframe> keyframes{};
    inline Keyframe& getCurrentKeyframe() {
        //printf("Getting Keyframe: %i / %i\n",currentKeyFrame, keyframes.size());
        //keyframes[currentKeyFrame].print();
        return keyframes[currentKeyFrame];
    }
};

void updateAnimator();
//void updateAnimatorI();

void uploadAnimation(AnimationComponent<int32_t> comp);
void uploadAnimation(AnimationComponent<float> comp);


#endif //SDL_ANIMATE_HPP

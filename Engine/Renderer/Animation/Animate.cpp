//
// Created by Owner on 3/5/2023.
//

#include "Animate.hpp"
/*
static uint64_t globalIdIndex = 0;
static std::vector<Animate> animators;

Animate::Animate(int32_t* obj, size_t speed_ms, int32_t cond) : _time(), _object(obj), _speed(speed_ms), _gIndex(globalIdIndex), _condition(cond), _complete(false) {
    animators.push_back(*this);
    globalIdIndex++;
}
void Animate::step(size_t& stepIter) {
    if (_time.isCompleteReset(_speed) && !paused) {
        if (*_object == _condition) {
            _complete = true;
            animators.erase(animators.begin()+stepIter);
            return;
        }
        (*_object)++;
        stepIter++;
    }
}
void Animate::pause() { paused = true; }
void Animate::resume() { paused = false; }
bool Animate::isComplete() { return _complete; }

void PlayAnimations() {
    for (size_t loop = 0; auto& anim : animators) {
        anim.step(loop);
    }
}*/

//static std::vector<AnimationComponent<float>> s_Animations{};
static size_t s_Frame{0};
static Timer s_FrameAnimationTimer{};

using AnimationVectorWrapper = std::vector<std::pair<std::variant<AnimationComponent<float>, AnimationComponent<int32_t>>,Animation::ValType>>;
static AnimationVectorWrapper s_Animations{};

/*
void updateAnimator() {
    if (s_FrameAnimationTimer.isCompleteReset(16)) {
        s_Frame++;
        for (uint32_t loop =  0; auto& anim : s_Animations) {
            if (anim.currentAnimationFrame == 0 && anim.currentKeyFrame == 0) (*anim.animationIsCompleteCounter)++;
            if (anim.currentAnimationFrame >= anim.getCurrentKeyframe().frameTime) {
                if (anim.getCurrentKeyframe().set.has_value())
                    (*anim.value) = anim.getCurrentKeyframe().set.value();
                else
                    (*anim.value) += anim.getCurrentKeyframe().increment;

                anim.currentKeyFrame++;
                anim.currentAnimationFrame = 0;
            }
            anim.currentAnimationFrame++;

            if (anim.currentKeyFrame > anim.keyframes.size() - 1) {
                if (anim.animationIsCompleteCounter != nullptr) (*anim.animationIsCompleteCounter)--;
                s_Animations.erase(s_Animations.begin() + loop);
            }

            loop++;
        }
    }
}
*/

void updateAnimator() {
    if (s_FrameAnimationTimer.isCompleteReset(16)) {
        s_Frame++;

        for (uint32_t i = 0; i < s_Animations.size();) {
            auto& anim = s_Animations[i];
            printf("Accessing Animation: %i / %i\n", i, s_Animations.size());
            if (anim.second == Animation::ValType::FLT) {
                AnimationComponent<float>& comp = std::get<AnimationComponent<float>>(anim.first);
                printf("\t(FLT) Accessing Keyframe: %i / %i\n", comp.currentKeyFrame, comp.keyframes.size());
                if (comp.currentAnimationFrame == 0 && comp.currentKeyFrame == 0 && comp.animationIsCompleteCounter != nullptr)
                    (*comp.animationIsCompleteCounter)++;

                if (comp.currentKeyFrame > comp.keyframes.size() - 1) {
                    if (comp.animationIsCompleteCounter != nullptr) (*comp.animationIsCompleteCounter)--;
                    s_Animations.erase(s_Animations.begin() + i);
                    continue;
                }

                if (comp.currentAnimationFrame >= comp.getCurrentKeyframe().frameTime) {
                    if (comp.getCurrentKeyframe().set.has_value()) (*comp.value) = comp.getCurrentKeyframe().set.value();
                    else (*comp.value) += comp.getCurrentKeyframe().increment;

                    comp.currentKeyFrame++;
                    comp.currentAnimationFrame = 0;
                }
                comp.currentAnimationFrame++;
            }
            else if (anim.second == Animation::ValType::INT) {
                AnimationComponent<int32_t>& comp = std::get<AnimationComponent<int32_t>>(anim.first);
                printf("\t(INT) Accessing Keyframe: %i / %i\n", comp.currentKeyFrame, comp.keyframes.size());
                if (comp.currentAnimationFrame == 0 && comp.currentKeyFrame == 0 && comp.animationIsCompleteCounter != nullptr)
                    (*comp.animationIsCompleteCounter)++;

                if (comp.currentKeyFrame > comp.keyframes.size() - 1) {
                    if (comp.animationIsCompleteCounter != nullptr) (*comp.animationIsCompleteCounter)--;
                    s_Animations.erase(s_Animations.begin() + i);
                    continue;
                }

                if (comp.currentAnimationFrame >= comp.getCurrentKeyframe().frameTime) {
                    if (comp.getCurrentKeyframe().set.has_value()) (*comp.value) = comp.getCurrentKeyframe().set.value();
                    else (*comp.value) += comp.getCurrentKeyframe().increment;

                    comp.currentKeyFrame++;
                    comp.currentAnimationFrame = 0;
                }
                comp.currentAnimationFrame++;
            }
            i++;
        }
    }
}
/*
static std::vector<AnimationComponent<int32_t>> s_AnimationsI{};
static size_t s_FrameI{0};
static Timer s_FrameAnimationTimerI{};

void updateAnimatorI() {
    if (s_FrameAnimationTimerI.isCompleteReset(16)) {
        s_FrameI++;
        for (uint32_t loop =  0; auto& anim : s_AnimationsI) {
            if (anim.currentAnimationFrame == 0 && anim.currentKeyFrame == 0) (*anim.animationIsCompleteCounter)++;
            if (anim.currentAnimationFrame >= anim.getCurrentKeyframe().frameTime) {
                if (anim.getCurrentKeyframe().set.has_value())
                    (*anim.value) = anim.getCurrentKeyframe().set.value();
                else
                    (*anim.value) += anim.getCurrentKeyframe().increment;

                anim.currentKeyFrame++;
                anim.currentAnimationFrame = 0;
            }
            anim.currentAnimationFrame++;

            if (anim.currentKeyFrame > anim.keyframes.size() - 1) {
                if (anim.animationIsCompleteCounter != nullptr) (*anim.animationIsCompleteCounter)--;
                s_AnimationsI.erase(s_AnimationsI.begin() + loop);
            }

            loop++;
        }
    }
}
*/

void uploadAnimation(AnimationComponent<int32_t> comp) {
    s_Animations.emplace_back(comp, Animation::ValType::INT);
}
void uploadAnimation(AnimationComponent<float> comp) {
    s_Animations.emplace_back(comp, Animation::ValType::FLT);
}

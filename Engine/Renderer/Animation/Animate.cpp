//
// Created by Owner on 3/5/2023.
//

#include "Animate.hpp"

static size_t s_Frame{0};
static Timer s_FrameAnimationTimer{};

using AnimationVectorWrapper = std::vector<std::pair<std::variant<AnimationComponent<float>, AnimationComponent<int32_t>>,Animation::ValType>>;
static AnimationVectorWrapper s_Animations{};

void updateAnimator() {
    if (s_FrameAnimationTimer.isCompleteReset(16)) {
        s_Frame++;

        for (uint32_t i = 0; i < s_Animations.size();) {
            auto& anim = s_Animations[i];
            if (anim.second == Animation::ValType::FLT) {
                AnimationComponent<float>& comp = std::get<AnimationComponent<float>>(anim.first);
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


void uploadAnimation(AnimationComponent<int32_t> comp) {
    s_Animations.emplace_back(comp, Animation::ValType::INT);
}
void uploadAnimation(AnimationComponent<float> comp) {
    s_Animations.emplace_back(comp, Animation::ValType::FLT);
}

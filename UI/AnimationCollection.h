#pragma once

#include "Animation.h"

namespace UI {

    class AnimationCollection : public Animation {

        public:

            AnimationCollection(std::shared_ptr<Widget> widget, const std::vector<std::shared_ptr<Animation>>& animations) :
                Animation(widget),
                animations(animations) {};

            AnimationCollection(const std::vector<std::shared_ptr<Animation>>& animations);

            virtual ~AnimationCollection() = default;

            void doAnimationStep() override;

            void waitForAnimationToEnd() override;

        private:

            void callAnimationEndCallback() const override {
                if (animationEndCallback)
                    animationEndCallback(nullptr);
            }

            void waitForAnimations();

            std::vector<std::shared_ptr<Animation>> animations;
    };
}

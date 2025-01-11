#pragma once

#include "Animation.h"
#include "Widget.h"

namespace UI {

    class MoveAnimation : public Animation {

        public:

            MoveAnimation(int32_t posX, int32_t posY, float speed, std::shared_ptr<Widget> widget);
            virtual ~MoveAnimation() = default;

            void doAnimationStep() override;

        private:

            int32_t targetPosX;
            int32_t targetPosY;

            int32_t startPosX;
            int32_t startPosY;

            float speed;

    };

    class Movable : public virtual Widget {

        public:

            virtual ~Movable() = default;

            virtual std::shared_ptr<MoveAnimation> moveTo(int32_t posX, int32_t posY, float speed = 25.0f);

        protected:

            Movable(int32_t posX, int32_t posY, uint32_t width, uint32_t height);

            std::shared_ptr<MoveAnimation> moveAnimation;

        private:

    };
}

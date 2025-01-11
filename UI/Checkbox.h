#pragma once

#include "Clickable.h"

#include <functional>

namespace UI {

    class CheckBox : public Clickable {

        public:

            using OnClickCallback = std::function<void(std::shared_ptr<Widget>,bool)>;

            friend class Widget;

            virtual ~CheckBox() = default;

            static std::shared_ptr<CheckBox> create(auto&&... args) {
                return Widget::create<CheckBox>(std::forward<decltype(args)>(args)...);
            }

            void handleMouseClickEvent(const MouseClickEvent& event) override;

            void setOnClickCallback(const OnClickCallback& onClick) noexcept { onClickCallback = onClick; }

            void enable(bool enable) { this->enabled = enable; }
            [[nodiscard]] bool isEnabled() const noexcept { return enabled; }

            void setOn(bool on) { this->on = on; }

        protected:

            CheckBox(int32_t posX, int32_t posY, uint32_t width, uint32_t height);

            virtual void drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) override;

        private:

            void drawCheck(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom);

            OnClickCallback onClickCallback;

            bool enabled = true;
            bool on = false;
    };
}
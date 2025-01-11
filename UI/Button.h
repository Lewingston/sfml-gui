#pragma once

#include "TextWidget.h"
#include "Clickable.h"

#include <functional>

namespace UI {

    class Button : public TextWidget, public Clickable {

        public:

            using OnClickCallback = std::function<void(std::shared_ptr<Widget>)>;

            friend class Widget;

            virtual ~Button() = default;

            static std::shared_ptr<Button> create(auto&&... args) {
                return Widget::create<Button>(std::forward<decltype(args)>(args)...);
            }

            void handleMouseClickEvent(const MouseClickEvent& event) override;

            void setOnClickCallback(const OnClickCallback& onClick) noexcept { onClickCallback = onClick; }

            void enable(bool enable) { this->enabled = enable; }
            [[nodiscard]] bool isEnabled() const noexcept { return enabled; }

        protected:

            Button(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const std::string& text, const Font& font);

            virtual void drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) override;

        private:

            OnClickCallback onClickCallback;

            bool enabled = true;

    };
}

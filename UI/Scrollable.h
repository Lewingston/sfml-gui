#pragma once

#include "Widget.h"

#include <mutex>

namespace sf {
    class RenderTexture;
}

namespace UI {

    class RootWidget;
    class MouseWheelEvent;

    class Scrollable : public UI::Widget {

        public:

            friend class UI::Widget;

            virtual ~Scrollable() = default;

            static std::shared_ptr<Scrollable> create(auto&&... args) {
                return Widget::create<Scrollable>(std::forward<decltype(args)>(args)...);
            }

            virtual void draw(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) override;

            virtual void handleWindowResizeEvent(uint32_t width, uint32_t height, float zoom);
            virtual void handleMouseWheelEvent(const MouseWheelEvent& event);

            virtual void registerScrollable();

            virtual void setScrollableHeight(uint32_t height);
            [[nodiscard]] virtual uint32_t getScrollableHeight() const noexcept;

            virtual void setScrollPosition(int32_t position);

        protected:

            Scrollable(int32_t posX, int32_t posY, uint32_t width, uint32_t height);

            std::shared_ptr<sf::RenderTexture> renderTexture;
            std::shared_ptr<sf::RenderTexture> clickTexture;

            std::mutex textureMutex;

            int32_t minScroll = 0;
            int32_t maxScroll = 0;
            int32_t currentScroll = minScroll;

        private:

    };
}
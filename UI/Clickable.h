#pragma once

#include "Widget.h"
#include "Color.h"

#include <map>
#include <deque>
#include <mutex>

namespace UI {

    class MouseClickEvent;

    class Clickable : public virtual Widget {

        public:

            virtual ~Clickable();

            static std::shared_ptr<Clickable> getClickableByColorId(Color color);

            virtual void handleMouseClickEvent(const MouseClickEvent& event) = 0;

            [[nodiscard]] static uint32_t getUsedIdCount() { return clickHandler.getUsedIdCount(); }
            [[nodiscard]] static uint32_t getUnusedIdCount() { return clickHandler.getUnusedIdCount(); }

        protected:

            Clickable(int32_t posX, int32_t posY, uint32_t width, uint32_t height);

            virtual void init() override;

            [[nodiscard]] Color getIdColor() const noexcept override { return idColor; }

            void drawChildren(const UI::RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) override;

        private:

            class ClickHandler {

                public:

                    ClickHandler();
                    virtual ~ClickHandler() = default;

                    std::shared_ptr<Clickable> getWidget(Color color);

                    Color addWidget(std::shared_ptr<Clickable> clickable);
                    void removeWidget(Color color);

                    [[nodiscard]] uint32_t getUsedIdCount() const { return static_cast<uint32_t>(clickMap.size()); }
                    [[nodiscard]] uint32_t getUnusedIdCount() const { return static_cast<uint32_t>(unusedColors.size()); }

                private:

                    static constexpr uint32_t MAX_ELEMENTS = 1024;

                    void initColors();

                    std::deque<Color> unusedColors;
                    std::map<Color, std::weak_ptr<Clickable>> clickMap;

                    std::mutex clickHandlerMutex;

            };

            static ClickHandler clickHandler;

            Color idColor;

    };
}

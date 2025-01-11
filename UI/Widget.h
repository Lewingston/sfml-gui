#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <type_traits>

#include "Style.h"

namespace sf {
    class RenderTarget;
    class Shape;
}

namespace UI {

    class RenderTarget {

        public:

            RenderTarget(std::shared_ptr<sf::RenderTarget> renderTarget, std::shared_ptr<sf::RenderTarget> clickRenderTarget) :
                renderTarget(renderTarget), clickRenderTarget(clickRenderTarget) {};

            [[nodiscard]] std::shared_ptr<sf::RenderTarget> getRenderTarget() const noexcept { return renderTarget; }
            [[nodiscard]] std::shared_ptr<sf::RenderTarget> getClickRenderTarget() const noexcept { return clickRenderTarget; }

        private:

            std::shared_ptr<sf::RenderTarget> renderTarget;
            std::shared_ptr<sf::RenderTarget> clickRenderTarget;
    };

    class Widget : public std::enable_shared_from_this<Widget> {

        public:

            virtual ~Widget() = default;

            template<typename  WidgetType>
            static std::shared_ptr<WidgetType> create(auto&&... args) {
                std::shared_ptr<WidgetType> widget = std::shared_ptr<WidgetType>(new WidgetType(std::forward<decltype(args)>(args)...));
                widget->init();
                return widget;
            }

            static std::shared_ptr<Widget> create(auto&&... args) {
                return create<Widget>(std::forward<decltype(args)>(args)...);
            }

            virtual void draw(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) ;

            void addChild(std::shared_ptr<Widget> widget);
            void removeAllChildren();
            void removeChild(std::shared_ptr<Widget> widget);

            [[nodiscard]] std::vector<std::shared_ptr<Widget>> getChildren();

            [[nodiscard]] bool hasWidgetAsParent(std::shared_ptr<Widget> widget) const;
            [[nodiscard]] std::shared_ptr<Widget> getParent() const { return parent.lock(); }
            [[nodiscard]] std::shared_ptr<Widget> getRootWidget();

            virtual void setStyle(const Style& style) { this->style = style; }
            [[nodiscard]] Style getStyle() const noexcept { return style; }

            virtual void setVisible(bool visible) noexcept { this->visible = visible; }
            [[nodiscard]] bool isVisible() const noexcept { return visible; }

            virtual void setPosition(int32_t posX, int32_t posY) noexcept { this->posX = posX; this->posY = posY; }
            virtual void setSize(uint32_t width, uint32_t height);

            virtual void setCornerRadius(float radius);

            [[nodiscard]] int32_t getPosX() const noexcept { return posX; }
            [[nodiscard]] int32_t getPosY() const noexcept { return posY; }
            [[nodiscard]] uint32_t getWidth() const noexcept { return width; }
            [[nodiscard]] uint32_t getHeight() const noexcept { return height; }

            [[nodiscard]] std::pair<int32_t, int32_t> getAbsolutePosition() const;

            void setFrontColor(Color color) { style.setFrontColor(color); }
            [[nodiscard]] Color getFrontColor() const noexcept { return style.getFrontColor(); }
            void setBackColor(Color color) { style.setBackColor(color); }
            [[nodiscard]] Color getBackColor() const noexcept { return style.getBackColor(); }
            void setBorderColor(Color color) { style.setBorderColor(color); }
            [[nodiscard]] Color getBorderColor() const noexcept { return style.getBorderColor(); }

            void setBorderWidth(int32_t width) { style.setBorderWidth(width); }
            [[nodiscard]] int32_t getBorderWidth() const noexcept { return style.getBorderWidth(); }

            void setContentAlignmentVertical(AlignmentVer alignment) noexcept { style.setContentAlignmentVertical(alignment); }
            void setContentAlignmentHorizontal(AlignmentHor alignment) noexcept { style.setContentAlignmentHorizontal(alignment); }

        protected:

            Widget(int32_t posX, int32_t posY, uint32_t width, uint32_t height);

            virtual void init();

            virtual void drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom);
            virtual void drawOutline(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom);
            virtual void drawChildren(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom);

            virtual void setDrawParameters(UI::Color backColor, UI::Color frameColor, int32_t offsetX, int32_t offsetY, float zoom);

            [[nodiscard]] std::shared_ptr<sf::Shape> getShape() const noexcept { return shape; }
            void setShape(std::shared_ptr<sf::Shape> shape) { this->shape = shape; }

            [[nodiscard]] virtual Color getIdColor() const noexcept { return Color::BLACK; }

        private:

            void setParent(std::shared_ptr<Widget> parent);

            std::mutex childMutex; // Makes adding and removing children thread save

            std::vector<std::shared_ptr<Widget>> children{};

            std::weak_ptr<Widget> parent;

            Style style{};

            std::shared_ptr<sf::Shape> shape;

            int32_t posX = 0;
            int32_t posY = 0;
            uint32_t width = 0;
            uint32_t height = 0;

            bool visible = true;

    };

}

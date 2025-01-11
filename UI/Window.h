#pragma once

#include <string>
#include <thread>
#include <memory>
#include <set>
#include <functional>

#include "Widget.h"
#include "Font.h"
#include "Color.h"
#include "Event.h"

namespace sf {
    class RenderWindow;
    class RenderTexture;
    class Event;
    class Image;
}

namespace UI {

    class Clickable;
    class Selectable;
    class RootWidget;
    class Scrollable;

    class Window {

        public:

            Window(const std::string& title, uint32_t width, uint32_t height);
            virtual ~Window() = default;

            void runEventLoop();

            void addWidget(std::shared_ptr<Widget> widget);
            void removeAllChildren();

            void setFrameRateLimit(uint32_t fps);

            void setIcon(const sf::Image& image);

            void close();

        private:

            std::shared_ptr<sf::RenderWindow> window;
            std::shared_ptr<sf::RenderTexture> clickTexture;
            std::shared_ptr<sf::RenderTexture> singlePixelClickTexture;

            std::shared_ptr<RootWidget> rootWidget;

            uint32_t refWidth;
            uint32_t refHeight;

            float zoom = 1.0f;
            int32_t deltaX = 0;
            int32_t deltaY = 0;

            int32_t mousePosX = 0;
            int32_t mousePosY = 0;

            Font consoleFont = Font("resources/Fonts/consolas.ttf");
            Color clickIdColor = Color(0);

            std::chrono::steady_clock::time_point fpsTimer;
            float fpsCount;

            bool showFpsCounter = false;
            bool showClickTexture = false;

            bool fixedResolution = false;

            void drawWindow();
            void drawClickTexture();
            void drawFps();
            void calculateFps();

            void calculateDrawParameters();

            std::shared_ptr<Clickable> getElementOnMousePosition();

            void handleEvent(const sf::Event& event);
            void handleMouseEvent(const sf::Event& event);
            void handleMouseWheelEvent(const sf::Event& event);
            void handleEnterTextEvent(const sf::Event& event);
            void handleWindowResizedEvent(const sf::Event& event);

    };

    class RootWidget : public Widget {

        public:

            virtual ~RootWidget() = default;

            friend class Widget;

            static std::shared_ptr<RootWidget> create(auto&&... args) {
                return Widget::create<RootWidget>(std::forward<decltype(args)>(args)...);
            }

            bool handleKeyboardEvent(const KeyBoardEvent& event);
            void handleWindowResizedEvent(uint32_t width, uint32_t height, float zoom);
            void handleMouseWheelEvent(const MouseWheelEvent& event);

            void registerSelectedWidget(std::shared_ptr<Selectable> selectable);
            void resetSelectedWidget();

            void registerScrollableWidget(std::shared_ptr<Scrollable> scrollable);

            [[nodiscard]] std::shared_ptr<Selectable> getSelectedElement() const noexcept { return selectedElement; }

        private:

            RootWidget(int32_t posX, int32_t posY, uint32_t width, uint32_t height);

            void removeOldScrollables();

            std::shared_ptr<Selectable> selectedElement;

            std::vector<std::weak_ptr<Scrollable>> scrollables;

            float zoom = 1.0f;
            uint32_t windowWidth;
            uint32_t windowHeight;

    };

}

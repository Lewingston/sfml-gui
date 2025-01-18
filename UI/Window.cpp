
#include "Window.h"

#include "Widget.h"
#include "Clickable.h"
#include "Selectable.h"
#include "Event.h"
#include "Scrollable.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace UI;

Window::Window(const std::string& title, uint32_t width, uint32_t height) :
    window(new sf::RenderWindow(sf::VideoMode({width, height}), title)),
    clickTexture(new sf::RenderTexture({width, height})),
    singlePixelClickTexture(new sf::RenderTexture({1, 1})),
    rootWidget(RootWidget::create(0, 0, width, height)),
    refWidth(width),
    refHeight(height) {

    setFrameRateLimit(60);
}

void Window::runEventLoop() {

    while(window->isOpen()) {

        //sf::Event event;
        while (const std::optional event = window->pollEvent()) {
            handleEvent(event.value());
        }

        drawWindow();

    }
}

void Window::drawWindow() {
    // clear window
    window->clear(sf::Color::Black);
    clickTexture->clear(sf::Color::Black);

    // draw stuff here
    rootWidget->draw(RenderTarget(window, clickTexture), deltaX, deltaY, zoom);

    if (showFpsCounter)
        drawFps();

    drawClickTexture();

    // display the current frame
    window->display();

    calculateFps();
}

void Window::drawClickTexture() {

    clickTexture->display();

    const sf::Texture& texture = clickTexture->getTexture();
    sf::Sprite sprite(texture);
    sprite.setPosition({static_cast<float>(-mousePosX), static_cast<float>(-mousePosY)});
    singlePixelClickTexture->clear(sf::Color::Black);
    singlePixelClickTexture->draw(sprite);
    singlePixelClickTexture->display();

    if (showClickTexture) {
        sprite.setPosition({0, 0});
        window->draw(sprite);
    }
}

void Window::drawFps() {

    sf::RectangleShape rectangle(sf::Vector2f(400, 23));
    rectangle.setPosition({0, 0});
    rectangle.setFillColor(sf::Color(112, 112, 112));
    window->draw(rectangle);

    if (!consoleFont.getSfFont().get())
        return;

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << fpsCount;

    std::string str = "FPS: " + stream.str();
    str += " - Unused Ids: " + std::to_string(Clickable::getUnusedIdCount());
    str += " Used Ids: " + std::to_string(Clickable::getUsedIdCount());

    sf::Text text(*consoleFont.getSfFont().get(), str, 16);
    text.setFillColor(sf::Color(sf::Color::White));
    text.setPosition({5, 0});

    window->draw(text);

}

void Window::calculateFps() {

    static constexpr uint32_t frameCount = 10;

    std::chrono::steady_clock::time_point newTime = std::chrono::steady_clock::now();
    int64_t microSeconds = std::chrono::duration_cast<std::chrono::microseconds>(newTime - fpsTimer).count();
    fpsTimer = newTime;
    fpsCount = (fpsCount / frameCount) * (frameCount - 1) + (1'000'000.0f / static_cast<float>(microSeconds) / frameCount);
}

void Window::setFrameRateLimit(uint32_t fps) {
    window->setFramerateLimit(fps);
    fpsCount = static_cast<float>(fps);
}

std::shared_ptr<Clickable> Window::getElementOnMousePosition() {

   sf::Image clickImage = singlePixelClickTexture->getTexture().copyToImage();
   const Color colorId = Color(clickImage.getPixel({0, 0}).toInteger());
   return Clickable::getClickableByColorId(colorId);
}

void Window::handleEvent(const sf::Event& event) {

    if (event.is<sf::Event::Closed>()) {
        window->close();
    } else if (event.is<sf::Event::Resized>()) {
        handleWindowResizedEvent(event);
    } else if (event.is<sf::Event::MouseButtonPressed>() ||
               event.is<sf::Event::MouseButtonReleased>() ||
               event.is<sf::Event::MouseMoved>()) {
        handleMouseEvent(event);
    } else if (event.is<sf::Event::MouseWheelScrolled>()) {
        handleMouseWheelEvent(event);
    } else if (event.is<sf::Event::TextEntered>()) {
        handleEnterTextEvent(event);
    }
}

void Window::handleWindowResizedEvent(const sf::Event& event) {

    const auto& resizeEvent = event.getIf<sf::Event::Resized>();
    if (!resizeEvent)
        return;

    if (!fixedResolution) {

        sf::FloatRect visibleArea({0, 0}, {static_cast<float>(resizeEvent->size.x), static_cast<float>(resizeEvent->size.y)});
        window->setView(sf::View(visibleArea));
        clickTexture = std::shared_ptr<sf::RenderTexture>(new sf::RenderTexture(resizeEvent->size));

        calculateDrawParameters();

        rootWidget->handleWindowResizedEvent(window->getSize().x, window->getSize().y, zoom);
    }
}

void Window::handleMouseEvent(const sf::Event& event) {

    if (const auto* mouseEvent = event.getIf<sf::Event::MouseMoved>()) {

        const MouseMoveEvent moveEvent(mouseEvent->position.x, mouseEvent->position.y, mousePosX, mousePosY);

        mousePosX = moveEvent.getPosX();
        mousePosY = moveEvent.getPosY();

        getElementOnMousePosition();

    } else if (event.is<sf::Event::MouseButtonPressed>() || event.is<sf::Event::MouseButtonReleased>()) {

        bool clickableIsSelected = false;

        const MouseClickEvent mouseEvent = [&]() -> MouseClickEvent {
            if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>(); mouseEvent != nullptr) {
                return MouseClickEvent(MouseClickEvent::getMouseButton(mouseEvent->button), mouseEvent->position.x, mouseEvent->position.y, true);
            } else if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>(); mouseEvent != nullptr) {
                return MouseClickEvent(MouseClickEvent::getMouseButton(mouseEvent->button), mouseEvent->position.x, mouseEvent->position.y, false);
            } else {
                return MouseClickEvent(MouseButton::NONE, 0, 0, false);
            }
        }();

        mousePosX = mouseEvent.getPosX();
        mousePosY = mouseEvent.getPosY();

        std::shared_ptr<Clickable> clickable = getElementOnMousePosition();
        if (clickable) {
            clickable->handleMouseClickEvent(mouseEvent);
            clickableIsSelected = clickable->hasWidgetAsParent(rootWidget->getSelectedElement()) ||
                                  std::dynamic_pointer_cast<Widget>(clickable) == std::dynamic_pointer_cast<Widget>(rootWidget->getSelectedElement());
        }

        // only reset selected widget if the clicked widget is not a children of the
        // selected widget.
        if (event.is<sf::Event::MouseButtonPressed>() && !clickableIsSelected)
            rootWidget->resetSelectedWidget();
    }
}

void Window::handleMouseWheelEvent(const sf::Event& event) {

    const auto* mouseWheelEvent = event.getIf<sf::Event::MouseWheelScrolled>();
    if (!mouseWheelEvent)
        return;

    const int32_t posX = static_cast<int32_t>(static_cast<float>(mouseWheelEvent->position.x) / zoom) - deltaX;
    const int32_t posY = static_cast<int32_t>(static_cast<float>(mouseWheelEvent->position.y) / zoom) - deltaY;

    MouseWheelEvent wheelEvent(posX, posY, static_cast<int32_t>(mouseWheelEvent->delta));

    rootWidget->handleMouseWheelEvent(wheelEvent);
}

void Window::handleEnterTextEvent(const sf::Event& event) {

    const auto* textEvent = event.getIf<sf::Event::TextEntered>();
    if (!textEvent)
        return;

    KeyBoardEvent keyEvent(static_cast<KeyBoardKey>(textEvent->unicode));

    bool eventHandled = rootWidget->handleKeyboardEvent(keyEvent);
    if (eventHandled)
        return;

    if (textEvent->unicode == 'x') {
        showClickTexture = !showClickTexture;
    } else if (textEvent->unicode == 'y') {
        showFpsCounter = !showFpsCounter;
    }
}

void Window::calculateDrawParameters() {

    uint32_t width = window->getSize().x;
    uint32_t height = window->getSize().y;

    if (static_cast<float>(width) / static_cast<float>(height) < (static_cast<float>(refWidth) / static_cast<float>(refHeight))) {
        zoom = static_cast<float>(width) / static_cast<float>(refWidth);
        deltaY = static_cast<int32_t>((static_cast<float>(height)/zoom - static_cast<float>(refHeight)) / 2.0f);
        if (deltaY < 0) deltaY *= -1;
        deltaX = 0;
    } else {
        zoom = static_cast<float>(height) / static_cast<float>(refHeight);
        deltaX = static_cast<int32_t>((static_cast<float>(width)/zoom - static_cast<float>(refWidth)) / 2.0f);
        if (deltaX < 0) deltaX *= -1;
        deltaY = 0;
    }
}

void Window::addWidget(std::shared_ptr<Widget> widget) {
    rootWidget->addChild(widget);
}

void Window::removeAllChildren() {
    rootWidget->removeAllChildren();
}

void Window::setIcon(const sf::Image& image) {
    window->setIcon({image.getSize().x, image.getSize().y}, image.getPixelsPtr());
}

void Window::close() {
    window->close();
}

/******************************************************************************/
/*                            class UI::RootWidget                            */
/******************************************************************************/

RootWidget::RootWidget(int32_t posX, int32_t posY, uint32_t width, uint32_t height) :
    Widget(posX, posY, width, height),
    windowWidth(width),
    windowHeight(height) {

}

bool RootWidget::handleKeyboardEvent(const KeyBoardEvent& event) {
    if (selectedElement) {
        selectedElement->handleKeyboardEvent(event);
        return true;
    } else {
        return false;
    }
}

void RootWidget::handleWindowResizedEvent(uint32_t width, uint32_t height, float zoom) {

    windowWidth = width;
    windowHeight = height;
    this->zoom = zoom;

    bool cleanUp = false;

    for (auto scrollable : scrollables) {
        auto s = scrollable.lock();
        if (s) {
            s->handleWindowResizeEvent(width, height, zoom);
        } else {
            cleanUp = true;
        }
    }

    if (cleanUp)
        removeOldScrollables();
}

void RootWidget::handleMouseWheelEvent(const MouseWheelEvent& event) {

    bool cleanUp = false;

    for (auto scrollable : scrollables) {
        auto s = scrollable.lock();
        if (s) {
            s->handleMouseWheelEvent(event);
        } else {
            cleanUp = true;
        }
    }

    if (cleanUp)
        removeOldScrollables();
}

void RootWidget::registerSelectedWidget(std::shared_ptr<Selectable> selectable) {
    if (selectedElement)
        selectedElement->reset();
    selectedElement = selectable;
}

void RootWidget::resetSelectedWidget() {
    if (selectedElement) {
        selectedElement->reset();
        selectedElement.reset();
    }
}

void RootWidget::registerScrollableWidget(std::shared_ptr<Scrollable> scrollable) {

    scrollables.push_back(scrollable);
    scrollable->handleWindowResizeEvent(windowWidth, windowHeight, zoom);
}


void RootWidget::removeOldScrollables() {

    std::erase_if(scrollables, [](auto scrollable) {
        return scrollable.expired();
    });
}


#include "Scrollable.h"
#include "Window.h"
#include "Event.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

using namespace UI;

Scrollable::Scrollable(int32_t posX, int32_t posY, uint32_t width, uint32_t height) :
    Widget(posX, posY, width, height) {

    renderTexture = std::shared_ptr<sf::RenderTexture>(new sf::RenderTexture());
    clickTexture = std::shared_ptr<sf::RenderTexture>(new sf::RenderTexture());

    renderTexture->create(width, height);
    clickTexture->create(width, height);
}

void Scrollable::registerScrollable() {

    auto rootWidget = std::dynamic_pointer_cast<RootWidget>(getRootWidget());
    if (rootWidget) {
        rootWidget->registerScrollableWidget(std::dynamic_pointer_cast<Scrollable>(shared_from_this()));
    }
}

void Scrollable::draw(const RenderTarget& target, int32_t offsetX, int32_t offsetY, float zoom) {

    if (!isVisible())
        return;

    std::lock_guard<std::mutex> lock(textureMutex);

    renderTexture->clear(sf::Color::Black);
    clickTexture->clear(sf::Color::Black);

    RenderTarget renderTarget(renderTexture, clickTexture);

    UI::Widget::draw(renderTarget, -getPosX(), -getPosY() - currentScroll, zoom);
    renderTexture->display();
    clickTexture->display();

    {
    const sf::Texture& texture = renderTexture->getTexture();
    sf::Sprite sprite(texture);
    sprite.setPosition(static_cast<float>(offsetX + getPosX()) * zoom, static_cast<float>(offsetY + getPosY()) * zoom);
    target.getRenderTarget()->draw(sprite);
    }

    {
    const sf::Texture& texture = clickTexture->getTexture();
    sf::Sprite sprite(texture);
    sprite.setPosition(static_cast<float>(offsetX + getPosX()) * zoom, static_cast<float>(offsetY + getPosY()) * zoom);
    target.getClickRenderTarget()->draw(sprite);
    }
}

void Scrollable::handleWindowResizeEvent(uint32_t, uint32_t, float zoom) {

    std::lock_guard<std::mutex> lock(textureMutex);

    renderTexture = std::shared_ptr<sf::RenderTexture>(new sf::RenderTexture());
    clickTexture = std::shared_ptr<sf::RenderTexture>(new sf::RenderTexture());

    const uint32_t width = static_cast<uint32_t>(static_cast<float>(getWidth()) * zoom);
    const uint32_t height = static_cast<uint32_t>(static_cast<float>(getHeight()) * zoom);

    renderTexture->create(width, height);
    clickTexture->create(width, height);
}

void Scrollable::handleMouseWheelEvent(const MouseWheelEvent& event) {

    if (!isVisible())
        return;

    const auto [posX, posY] = getAbsolutePosition();
    if (event.getPosX() < posX || event.getPosY() < posY ||
        event.getPosX() > posX + static_cast<int32_t>(getWidth()) ||
        event.getPosY() > posY + static_cast<int32_t>(getHeight())) {
        return;
    }

    const int32_t scrollWidth = static_cast<int32_t>(static_cast<float>(getHeight()) * 0.1f);

    currentScroll = std::clamp(currentScroll + scrollWidth * -event.getDelta(), minScroll, maxScroll);
}

void Scrollable::setScrollableHeight(uint32_t height) {

    maxScroll = static_cast<int32_t>(height) - static_cast<int32_t>(getHeight());
    if (maxScroll < 0)
        maxScroll = 0;

    currentScroll = std::clamp(currentScroll, minScroll, maxScroll);
}

uint32_t Scrollable::getScrollableHeight() const noexcept {

    return static_cast<uint32_t>(static_cast<int32_t>(getHeight()) + maxScroll);
}

void Scrollable::setScrollPosition(int32_t position) {

    currentScroll =  std::clamp(position, minScroll, maxScroll);
}
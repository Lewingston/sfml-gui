
#include "CheckBox.h"
#include "Event.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

using namespace UI;

CheckBox::CheckBox(int32_t posX, int32_t posY, uint32_t width, uint32_t height) :
    Widget(posX, posY, width, height),
    Clickable(posX, posY, width, height) {

}

void CheckBox::drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    Widget::drawSelf(renderTarget, offsetX, offsetY, zoom);
    drawOutline(renderTarget, offsetX, offsetY, zoom);

    if (on)
        drawCheck(renderTarget, offsetX, offsetY, zoom);
}

void CheckBox::drawCheck(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    sf::RectangleShape shape;
    const float width = static_cast<float>(getWidth()) * 0.8f * zoom;
    const float height = static_cast<float>(getHeight()) * 0.8f * zoom;
    shape.setSize(sf::Vector2f(width, height));
    const float posX = (static_cast<float>(getPosX() + offsetX) + static_cast<float>(getWidth()) * 0.1f) * zoom;
    const float posY = (static_cast<float>(getPosY() + offsetY) + static_cast<float>(getWidth()) * 0.1f) * zoom;
    shape.setPosition(posX, posY);
    shape.setFillColor(sf::Color(getFrontColor().getValue()));

    renderTarget.getRenderTarget()->draw(shape);
}

void CheckBox::handleMouseClickEvent(const MouseClickEvent& event) {

    if (!event.pressed())
        return;

    if (event.getMouseButton() != MouseButton::LEFT)
        return;

    if (enabled)
        {
        on = !on;
        if (onClickCallback && enabled) {
            onClickCallback(shared_from_this(), on);
        }
    }
}
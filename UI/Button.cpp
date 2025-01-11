
#include "Button.h"

#include "Event.h"

using namespace UI;

Button::Button(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const std::string& text, const Font& font) :
    Widget(posX, posY, width, height),
    TextWidget(posX, posY, width, height, text, font),
    Clickable(posX, posY, width, height) {

}

void Button::drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    TextWidget::drawSelf(renderTarget, offsetX, offsetY, zoom);
    drawOutline(renderTarget, offsetX, offsetY, zoom);

}

void Button::handleMouseClickEvent(const MouseClickEvent& event) {

    if (!event.pressed())
        return;

    if (event.getMouseButton() != MouseButton::LEFT)
        return;

    if (onClickCallback && enabled) {
        onClickCallback(shared_from_this());
    }
}

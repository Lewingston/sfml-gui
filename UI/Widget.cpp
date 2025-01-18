
#include "Widget.h"
#include "RoundedRectangleShape.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

using namespace UI;

Widget::Widget(int32_t posX, int32_t posY, uint32_t width, uint32_t height) :
    posX(posX),
    posY(posY),
    width(width),
    height(height)
{
    shape = std::shared_ptr<RoundedRectangleShape>(new RoundedRectangleShape(static_cast<float>(width), static_cast<float>(height)));
}

void Widget::init() {

}

void Widget::draw(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    if (!visible)
        return;

    drawSelf(renderTarget, offsetX, offsetY, zoom);
    drawChildren(renderTarget, offsetX, offsetY, zoom);

}

void Widget::drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    if (!renderTarget.getRenderTarget())
        return;

    if (!shape)
        return;

    setDrawParameters(style.getBackColor(), style.getBorderColor(), offsetX, offsetY, zoom);

    renderTarget.getRenderTarget()->draw(*shape.get());

    drawOutline(renderTarget, offsetX, offsetY, zoom);
}

void Widget::drawOutline(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    if (!renderTarget.getClickRenderTarget())
        return;

    std::shared_ptr<sf::Shape> shape = getShape();
    if (!shape)
        return;

    const Color colorBack = getBackColor().isTransparent() ? Color::TRANSPARENT : getIdColor();
    const Color colorFrame = getBorderColor().isTransparent() ? Color::TRANSPARENT : getIdColor();
    setDrawParameters(colorBack, colorFrame, offsetX, offsetY, zoom);

    renderTarget.getClickRenderTarget()->draw(*shape.get());
}

void Widget::setDrawParameters(UI::Color backColor, UI::Color frameColor, int32_t offsetX, int32_t offsetY, float zoom) {

    shape->setScale({zoom, zoom});
    shape->setPosition({static_cast<float>(getPosX() + offsetX)*zoom, static_cast<float>(getPosY() + offsetY)*zoom});
    shape->setOutlineThickness(static_cast<float>(static_cast<int32_t>(static_cast<float>(style.getBorderWidth()) * zoom)));

    shape->setOutlineColor(sf::Color(frameColor.getValue()));
    shape->setFillColor(sf::Color(backColor.getValue()));
}

void Widget::drawChildren(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {
    std::vector<std::shared_ptr<Widget>> children = getChildren();
    for (std::shared_ptr<Widget> widget : children) {
        widget->draw(renderTarget, offsetX + posX, offsetY + posY, zoom);
    }
}

std::vector<std::shared_ptr<Widget>> Widget::getChildren() {
    const std::lock_guard<std::mutex> lock(childMutex);
    return children;
}

void Widget::addChild(std::shared_ptr<Widget> widget) {
    if (auto p = widget->parent.lock())
        p->removeChild(widget);
    const std::lock_guard<std::mutex> lock(childMutex);
    widget->setParent(shared_from_this());
    children.push_back(widget);
}

void Widget::removeChild(std::shared_ptr<Widget> widget) {
    const std::lock_guard<std::mutex> lock(childMutex);
    children.erase(std::remove(children.begin(), children.end(), widget), children.end());
}

void Widget::removeAllChildren() {
    const std::lock_guard<std::mutex> lock(childMutex);
    children.clear();
}

void Widget::setParent(std::shared_ptr<Widget> parent) {
    this->parent = parent;
}

void Widget::setSize(uint32_t width, uint32_t height) {

    this->width = width;
    this->height = height;

    // TODO: not a very good solution
    std::shared_ptr<RoundedRectangleShape> roundedRectangleShape = std::dynamic_pointer_cast<RoundedRectangleShape>(getShape());
    if (roundedRectangleShape) {
        roundedRectangleShape->setSize(static_cast<float>(width), static_cast<float>(height));
    }

}

bool Widget::hasWidgetAsParent(std::shared_ptr<Widget> widget) const {
    if (parent.lock() == widget)
        return true;
    else if (parent.lock())
        return parent.lock()->hasWidgetAsParent(widget);
    else
        return false;
}

std::shared_ptr<Widget> Widget::getRootWidget() {
    if (parent.lock())
        return parent.lock()->getRootWidget();
    else
        return shared_from_this();
}

std::pair<int32_t, int32_t> Widget::getAbsolutePosition() const {

    int32_t posX = getPosX();
    int32_t posY = getPosY();

    std::shared_ptr<Widget> widget = getParent();
    while (widget->getParent()) {
        posX += widget->getPosX();
        posY += widget->getPosY();
        widget = widget->getParent();
    }

    return std::pair<int32_t, int32_t>(posX, posY);
}

void Widget::setCornerRadius(float radius) {
    std::shared_ptr<RoundedRectangleShape> shape = std::dynamic_pointer_cast<RoundedRectangleShape>(this->shape);
    if (shape)
        shape->setCornerRadiusAndLineCount(radius, static_cast<uint32_t>(radius / 3) + 1);
}

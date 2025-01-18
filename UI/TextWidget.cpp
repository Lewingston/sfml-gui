
#include "TextWidget.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>

using namespace UI;

TextWidget::TextWidget(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const std::string& text, const Font& font) :
    Widget(posX, posY, width, height),
    text(text),
    font(font) {

    }

void TextWidget::drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    if (!renderTarget.getRenderTarget())
        return;

    Widget::drawSelf(renderTarget, offsetX, offsetY, zoom);

    if (!font.getSfFont())
        return;

    const uint32_t fontSize = static_cast<uint32_t>(static_cast<float>(fontStyle.getFontSize())*zoom);
    sf::Text text(*font.getSfFont().get(), this->text, fontSize);

    // set style parameters
    text.setFillColor(sf::Color(getStyle().getFrontColor().getValue()));
    text.setStyle(static_cast<uint32_t>(fontStyle.getFontStyle()));
    text.setLetterSpacing(fontStyle.getCharacterSpacingFactor());

    // TODO: Maybe dont calculate text dimensions with each draw call
    textWidth = static_cast<int32_t>(std::lround(text.getGlobalBounds().size.x));
    textHeight = static_cast<int32_t>(std::lround(text.getGlobalBounds().size.y));

    int32_t textPosX = getPosX() + offsetX;
    int32_t textPosY = getPosY() + offsetY;

    int32_t originX = 0;
    int32_t originY = 0;

    Style style = getStyle();

    // calculate horizontal position
    if (style.getContaentAlignmentHorizontal() == AlignmentHor::RIGHT) {
        originX = textWidth;
        textPosX += static_cast<int32_t>(getWidth()) - fontStyle.getHorizontalOffset();
    } else if (style.getContaentAlignmentHorizontal() == AlignmentHor::LEFT) {
        originX = 0;
        textPosX += fontStyle.getHorizontalOffset();
    } else if (style.getContaentAlignmentHorizontal() == AlignmentHor::CENTER) {
        originX = textWidth / 2 + static_cast<int32_t>(text.getLocalBounds().position.x);
        textPosX += static_cast<int32_t>(getWidth()) / 2 + fontStyle.getHorizontalOffset();
    }

    // calculate vertical position
    if (style.getContentAlignmentVertical() == AlignmentVer::BOTTOM) {
        originY = textHeight + static_cast<int32_t>(text.getLocalBounds().position.y);
        textPosY += static_cast<int32_t>(getHeight()) + fontStyle.getVerticalOffset();
    } else if (style.getContentAlignmentVertical() == AlignmentVer::TOP) {
        originY = fontStyle.getVerticalOffset();
    } else if (style.getContentAlignmentVertical() == AlignmentVer::CENTER) {
        originY = textHeight / 2 + static_cast<int32_t>(text.getLocalBounds().position.y);
        textPosY += static_cast<int32_t>(getHeight() / 2) + fontStyle.getVerticalOffset();
    }

    text.setOrigin({static_cast<float>(originX), static_cast<float>(originY)});
    text.setPosition({static_cast<float>(textPosX)*zoom, static_cast<float>(textPosY)*zoom});

    renderTarget.getRenderTarget()->draw(text);

}

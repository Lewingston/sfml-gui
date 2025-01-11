
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

    sf::Text text;

    // set style parameters
    if (font.getSfFont())
        text.setFont(*font.getSfFont().get());
    text.setString(this->text);
    text.setCharacterSize(static_cast<uint32_t>(static_cast<float>(fontStyle.getFontSize())*zoom));
    text.setFillColor(sf::Color(getStyle().getFrontColor().getValue()));
    text.setStyle(static_cast<uint32_t>(fontStyle.getFontStyle()));
    text.setLetterSpacing(fontStyle.getCharacterSpacingFactor());

    // TODO: Maybe dont calculate text dimensions with each draw call
    textWidth = static_cast<int32_t>(std::lround(text.getGlobalBounds().width));
    textHeight = static_cast<int32_t>(std::lround(text.getGlobalBounds().height));

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
        originX = textWidth / 2 + static_cast<int32_t>(text.getLocalBounds().left);
        textPosX += static_cast<int32_t>(getWidth()) / 2 + fontStyle.getHorizontalOffset();
    }

    // calculate vertical position
    if (style.getContentAlignmentVertical() == AlignmentVer::BOTTOM) {
        originY = textHeight + static_cast<int32_t>(text.getLocalBounds().top);
        textPosY += static_cast<int32_t>(getHeight()) + fontStyle.getVerticalOffset();
    } else if (style.getContentAlignmentVertical() == AlignmentVer::TOP) {
        originY = fontStyle.getVerticalOffset();
    } else if (style.getContentAlignmentVertical() == AlignmentVer::CENTER) {
        originY = textHeight / 2 + static_cast<int32_t>(text.getLocalBounds().top);
        textPosY += static_cast<int32_t>(getHeight() / 2) + fontStyle.getVerticalOffset();
    }

    text.setOrigin(static_cast<float>(originX), static_cast<float>(originY));
    text.setPosition(static_cast<float>(textPosX)*zoom, static_cast<float>(textPosY)*zoom);

    renderTarget.getRenderTarget()->draw(text);

}

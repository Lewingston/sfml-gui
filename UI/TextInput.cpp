
#include "TextInput.h"
#include "Event.h"
#include "Window.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

using namespace UI;

TextInput::TextInput(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const Font& font) :
    Widget(posX, posY, width, height),
    TextWidget(posX, posY, width, height, "", font),
    Clickable(posX, posY, width, height),
    Selectable() {

}

void TextInput::drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    TextWidget::drawSelf(renderTarget, offsetX, offsetY, zoom);

    if (selected) {
        drawCursor(renderTarget, offsetX, offsetY, zoom);
    }

    drawOutline(renderTarget, offsetX, offsetY, zoom);

}

void TextInput::drawCursor(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    cursorBlinkTimer += 1;

    if (cursorBlinkTimer > 40)
        cursorBlinkTimer = 0;

    if (cursorBlinkTimer > 20)
        return;

    sf::RectangleShape line(sf::Vector2f(2*zoom, static_cast<float>(getFontSize())));
    line.setFillColor(sf::Color(getStyle().getFrontColor().getValue()));

    int32_t posX = static_cast<int32_t>(static_cast<float>(getPosX() + offsetX + 3) * zoom);
    int32_t posY = static_cast<int32_t>(static_cast<float>(getPosY() + offsetY) * zoom);

    Style style = getStyle();

    if (style.getContaentAlignmentHorizontal() == AlignmentHor::RIGHT) {

    } else if (style.getContaentAlignmentHorizontal() == AlignmentHor::LEFT) {
        posX += getTextWidth() + getFontStyle().getHorizontalOffset();
    } else if (style.getContaentAlignmentHorizontal() == AlignmentHor::CENTER) {
        posX += static_cast<int32_t>((static_cast<float>(getWidth()) / 2.0f)*zoom + static_cast<float>(getTextWidth()) / 2.0f);
    }

    if (style.getContentAlignmentVertical() == AlignmentVer::TOP) {

    } else if (style.getContentAlignmentVertical() == AlignmentVer::BOTTOM) {
        posY += static_cast<int32_t>(static_cast<float>(getHeight())*zoom - static_cast<float>(getTextHeight()));
    } else if (style.getContentAlignmentVertical() == AlignmentVer::CENTER) {
        posY += static_cast<int32_t>(static_cast<float>(getHeight())*zoom / 2.0f) - static_cast<int32_t>(getFontSize()) / 2;
    }

    line.setPosition(static_cast<float>(posX), static_cast<float>(posY));

    renderTarget.getRenderTarget()->draw(line);
}

void TextInput::handleMouseClickEvent(const MouseClickEvent& event) {

    if (!event.pressed())
        return;

    if (event.getMouseButton() != MouseButton::LEFT)
        return;

    if (selected == true)
        return;

    cursorBlinkTimer = 0;
    selected = true;

    if (selectCallback)
        selectCallback(std::dynamic_pointer_cast<TextInput>(shared_from_this()));

    registerAsSelected();
}

void TextInput::handleKeyboardEvent(const KeyBoardEvent& event) {

    switch (event.getKeyPressed()) {
        case KeyBoardKey::NONE:
            break;
        case KeyBoardKey::RETURN:
            returnKeyPressed();
            break;
        case KeyBoardKey::BACKSPACE:
            removeCharacter();
            break;
        case KeyBoardKey::BACKSPACE_STRG:
            removeWord();
            break;
        default:
            addCharacter(static_cast<uint32_t>(event.getKeyPressed()));
            break;
    }
}

void TextInput::addCharacter(uint32_t unicodeChar) {
    setText(getText() + static_cast<char>(unicodeChar));
}

void TextInput::removeCharacter() {
    std::string text = getText();
    if (text.size() > 0)
        setText(text.substr(0, text.size() - 1));
}

void TextInput::removeWord() {

    std::string text = getText();

    // if last char is white space remove all trailing white spaces and last word
    // and again all trailing white spaces

    size_t index = text.length() - 1;

    // first search for last non white space character
    for (;;index--) {
        if (index == 0 || text[index] != ' ')
            break;
    }

    // then search for next white space character
    for (;;index--) {
        if (index == 0 || text[index] == ' ')
            break;
    }

    setText(text.substr(0, index));

}

void TextInput::returnKeyPressed() {

    std::shared_ptr<RootWidget> rootWidget = std::dynamic_pointer_cast<RootWidget>(getRootWidget());
    if (rootWidget)
        rootWidget->resetSelectedWidget();
}

void TextInput::reset() {

    if (selected == true && valueChangeCallback)
        valueChangeCallback(std::dynamic_pointer_cast<TextInput>(shared_from_this()), getText());

    selected = false;
}

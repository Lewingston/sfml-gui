
#include "Event.h"

#include <SFML/Window/Event.hpp>

using namespace UI;

/******************************************************************************/
/*                            class MouseClickEvent                           */
/******************************************************************************/

MouseClickEvent::MouseClickEvent(const sf::Event& event) : Event(EventType::MOUSE_MOVED) {

    if (event.type != sf::Event::MouseButtonPressed && event.type != sf::Event::MouseButtonReleased)
        return;

    posX = event.mouseButton.x;
    posY = event.mouseButton.y;

    click = event.type == sf::Event::MouseButtonPressed;

    switch (event.mouseButton.button) {
        case sf::Mouse::Left: button = MouseButton::LEFT; break;
        case sf::Mouse::Right: button = MouseButton::RIGHT; break;
        case sf::Mouse::Middle: button = MouseButton::MIDDLE; break;
        case sf::Mouse::XButton1: button = MouseButton::X1; break;
        case sf::Mouse::XButton2: button = MouseButton::X2; break;
        default: button = MouseButton::NONE; break;
    }
}

/******************************************************************************/
/*                             class MouseMoveEvent                           */
/******************************************************************************/

MouseMoveEvent::MouseMoveEvent(const sf::Event& event, int32_t oldX, int32_t oldY) : 
    Event(EventType::MOUSE_MOVED),
    oldX(oldX),
    oldY(oldY) {

    if (event.type != sf::Event::MouseMoved)
        return;

    posX = event.mouseMove.x;
    posY = event.mouseMove.y;
}



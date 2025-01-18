
#include "Event.h"

#include <SFML/Window/Mouse.hpp>

using namespace UI;

/******************************************************************************/
/*                            class MouseClickEvent                           */
/******************************************************************************/

MouseButton MouseClickEvent::getMouseButton(sf::Mouse::Button button) noexcept {

    switch (button) {
        case sf::Mouse::Button::Left:     return MouseButton::LEFT;
        case sf::Mouse::Button::Right:    return MouseButton::RIGHT;
        case sf::Mouse::Button::Middle:   return MouseButton::MIDDLE;
        case sf::Mouse::Button::Extra1:   return MouseButton::X1;
        case sf::Mouse::Button::Extra2:   return MouseButton::X2;
        default: return MouseButton::NONE;
    }
}

/******************************************************************************/
/*                             class MouseMoveEvent                           */
/******************************************************************************/



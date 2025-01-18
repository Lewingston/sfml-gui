#pragma once

#include <cstdint>

namespace sf {
    class Event;

    namespace Mouse {
        enum class Button;
    }
};

namespace UI {

    enum class EventType {
        NONE,
        MOUSE_CLICKED,
        MOUSE_MOVED,
        MOUSE_WHEEL,
        KEY_BOARD
    };

    enum class MouseButton {
        NONE,
        LEFT,
        RIGHT,
        MIDDLE,
        X1,
        X2
    };

    enum class KeyBoardKey : uint32_t {
        NONE = 0,
        BACKSPACE = 8,
        RETURN = 13,
        BACKSPACE_STRG = 127
    };

    class Event {
        public:

            Event(EventType type) : type(type) {}

        private:
            EventType type = EventType::NONE;
    };

    class MouseClickEvent : public Event {

        public:

            MouseClickEvent(MouseButton button, int32_t posX, int32_t posY, bool click) :
                Event(EventType::MOUSE_CLICKED),
                button(button),
                posX(posX),
                posY(posY),
                click(click) {}

            virtual ~MouseClickEvent() = default;

            [[nodiscard]] MouseButton getMouseButton() const noexcept { return button; }
            [[nodiscard]] int32_t getPosX() const noexcept { return posX; }
            [[nodiscard]] int32_t getPosY() const noexcept { return posY; }
            [[nodiscard]] bool pressed() const noexcept { return click; }
            [[nodiscard]] bool released() const noexcept { return !click; }

            [[nodiscard]] static MouseButton getMouseButton(sf::Mouse::Button button) noexcept;

        private:

            MouseButton button = MouseButton::NONE;
            int32_t posX = 0;
            int32_t posY = 0;
            bool click = false;

    };

    class MouseMoveEvent : public Event {

        public:

            MouseMoveEvent(int32_t posX, int32_t posY, int32_t oldX, int32_t oldY) :
                Event(EventType::MOUSE_MOVED),
                posX(posX),
                posY(posY),
                oldX(oldX),
                oldY(oldY) {}

            virtual ~MouseMoveEvent() = default;

            [[nodiscard]] int32_t getPosX() const noexcept { return posX; }
            [[nodiscard]] int32_t getPosY() const noexcept { return posY; }

        private:

            int32_t posX = 0;
            int32_t posY = 0;
            int32_t oldX = 0;
            int32_t oldY = 0;

    };

    class MouseWheelEvent : public Event {

        public:

            MouseWheelEvent(int32_t posX, int32_t posY, int32_t delta) :
                Event(EventType::MOUSE_WHEEL),
                posX(posX), posY(posY), delta(delta) {}

            virtual ~MouseWheelEvent() = default;

            [[nodiscard]] int32_t getPosX() const noexcept { return posX; }
            [[nodiscard]] int32_t getPosY() const noexcept { return posY; }
            [[nodiscard]] int32_t getDelta() const noexcept { return delta; }

        private:

            int32_t posX;
            int32_t posY;
            int32_t delta;

    };

    class KeyBoardEvent : public Event {

        public:

            KeyBoardEvent();
            KeyBoardEvent(KeyBoardKey keyPressed) : Event(EventType::KEY_BOARD), keyPressed(keyPressed) {}

            [[nodiscard]] KeyBoardKey getKeyPressed() const noexcept { return keyPressed; }

        private:

            KeyBoardKey keyDown = KeyBoardKey::NONE;
            KeyBoardKey keyUp = KeyBoardKey::NONE;
            KeyBoardKey keyPressed = KeyBoardKey::NONE;

    };

}

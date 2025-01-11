#pragma once

#include <cstdint>

namespace UI {

    // Class Color has no virtual destructor to be as small as possible (4 Bytes)
    // Therefore Color should not be used as base class and is declared as final
    class Color final {

        public:

            Color(uint8_t r, uint8_t g, uint8_t b) : Color(r, g, b, 255) {};

            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
                Color((static_cast<uint32_t>(r) << 24) +
                       (static_cast<uint32_t>(g) << 16) +
                       (static_cast<uint32_t>(b) << 8) +
                       static_cast<uint32_t>(a)) {};

            Color(uint32_t rgba) : color(rgba) {};

            ~Color() = default;

            bool operator< (const Color& color) const { return this->color < color.color; }
            bool operator== (const Color& color) const { return this->color == color.color; }

            [[nodiscard]] uint32_t getValue() const noexcept { return color; }
            [[nodiscard]] bool isTransparent() const noexcept { return (color & 255) == 0; }

            static const Color RED;
            static const Color DARK_RED;
            static const Color PINK;
            static const Color ORANGE;

            static const Color YELLOW;
            static const Color GOLD;

            static const Color MAGENTA;
            static const Color PURPLE;

            static const Color LIGHT_GREEN;
            static const Color GREEN;
            static const Color OLIVE;
            static const Color TEAL;

            static const Color BLUE;
            static const Color DARK_BLUE;
            static const Color LIGHT_BLUE;
            static const Color CYAN;

            static const Color BROWN;

            static const Color WHITE;
            static const Color BLACK;
            static const Color GRAY;
            static const Color LIGHT_GRAY;
            static const Color DARK_GRAY;

            static const Color TRANSPARENT;

        private:

            uint32_t color;

    };
}

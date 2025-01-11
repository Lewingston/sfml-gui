#pragma once

#include "Color.h"

namespace UI {

    enum class AlignmentHor {
        LEFT,
        RIGHT,
        CENTER
    };

    enum class AlignmentVer {
        TOP,
        BOTTOM,
        CENTER,
    };

    enum class FontAttribute : uint32_t {
        REGULAR        = 0,      // sf::Text::Regular,
        BOLD           = 1 << 0, // sf::Text::Bold,
        ITALIC         = 1 << 1, // sf::Text::Italic,
        UNDERLIND      = 1 << 2, // sf::Text::Underlined,
        STRIKE_THROUGH = 1 << 3, // sf::Text::StrikeThrough
    };

    class Style {

        public:

            Style() = default;
            Style(const Color& frontColor, const Color& backColor) :
                frontColor(frontColor), backColor(backColor) {};

            Style(const Color& frontColor,
                  const Color& backColor,
                  const Color& borderColor,
                  int32_t borderWidth) :
                  frontColor(frontColor),
                  backColor(backColor),
                  borderColor(borderColor),
                  borderWidth(borderWidth) {};

            Style(const Color& frontColor,
                  const Color& backColor,
                  const Color& borderColor,
                  int32_t borderWidth,
                  AlignmentVer ver,
                  AlignmentHor hor) :
                  frontColor(frontColor),
                  backColor(backColor),
                  borderColor(borderColor),
                  borderWidth(borderWidth),
                  contentAlignmentVertical(ver),
                  contentAlignmentHorizontal(hor) {};

            virtual ~Style() = default;

            [[nodiscard]] Color getFrontColor() const noexcept { return frontColor; }
            void setFrontColor(const Color& color) noexcept { frontColor = color; }

            [[nodiscard]] Color getBackColor() const noexcept { return backColor; }
            void setBackColor(const Color& color) noexcept { backColor = color; }

            [[nodiscard]] Color getBorderColor() const noexcept { return borderColor; }
            void setBorderColor(const Color& color) noexcept { borderColor = color; }

            [[nodiscard]] int32_t getBorderWidth() const noexcept { return borderWidth; }
            void setBorderWidth(int32_t width) noexcept { borderWidth = width; }

            [[nodiscard]] AlignmentVer getContentAlignmentVertical() const noexcept { return contentAlignmentVertical; }
            void setContentAlignmentVertical(AlignmentVer alignment) noexcept { contentAlignmentVertical = alignment; }

            [[nodiscard]] AlignmentHor getContaentAlignmentHorizontal() const noexcept { return contentAlignmentHorizontal; }
            void setContentAlignmentHorizontal(AlignmentHor alignment) noexcept { contentAlignmentHorizontal = alignment; }

        private:

            Color frontColor = Color::BLACK;
            Color backColor = Color::WHITE;
            Color borderColor = Color::BLACK;

            int32_t borderWidth = 1;

            AlignmentVer contentAlignmentVertical = AlignmentVer::CENTER;
            AlignmentHor contentAlignmentHorizontal = AlignmentHor::CENTER;

    };

    class FontStyle {

        public:

            FontStyle() = default;
            FontStyle(uint32_t fontSize) : fontSize(fontSize) {}

            FontStyle(uint32_t fontSize, FontAttribute style, float characterSpacingFactor) :
                fontSize(fontSize), style(style), characterSpacingFactor(characterSpacingFactor) {}

            virtual ~FontStyle() = default;

            [[nodiscard]] uint32_t getFontSize() const noexcept { return fontSize; }
            void setFontSize(uint32_t size) noexcept { fontSize = size; }

            [[nodiscard]] FontAttribute getFontStyle() const noexcept { return style; }
            void setFontStyle(FontAttribute style) noexcept { this->style = style; }

            [[nodiscard]] float getCharacterSpacingFactor() const noexcept { return characterSpacingFactor; }
            void setCharacterSpacingFector(float factor) noexcept { characterSpacingFactor = factor; }

            [[nodiscard]] int32_t getHorizontalOffset() const noexcept { return horizontalOffset; }
            void setHorizontalOffset(int32_t offset) noexcept { horizontalOffset = offset; }

            [[nodiscard]] int32_t getVerticalOffset() const noexcept { return verticalOffset; }
            void setVerticalOffset(int32_t offset) noexcept { verticalOffset = offset; };

        private:

            uint32_t fontSize = 24;
            FontAttribute style = FontAttribute::REGULAR;
            float characterSpacingFactor = 1.0f;

            int32_t horizontalOffset = 0;
            int32_t verticalOffset = 0;

    };

}

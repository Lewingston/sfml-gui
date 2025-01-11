#pragma once

#include "Widget.h"
#include "Font.h"

namespace UI {

    class TextWidget : public virtual Widget {

        public:

            virtual ~TextWidget() = default;

            friend class Widget;

            static std::shared_ptr<TextWidget> create(auto&&... args) {
                return Widget::create<TextWidget>(std::forward<decltype(args)>(args)...);
            }

            [[nodiscard]] std::string getText() const noexcept { return text; }
            void setText(const std::string& text) noexcept { this->text = text; }

            [[nodiscard]] const Font& getFont() const noexcept { return font; }
            void setFont(const Font& font) noexcept { this->font = font; }
            [[nodiscard]] const FontStyle getFontStyle() const noexcept { return fontStyle; }
            void setFontStyle(const FontStyle& style) noexcept { fontStyle = style; }
            [[nodiscard]] uint32_t getFontSize() const noexcept { return fontStyle.getFontSize(); }
            void setFontSize(uint32_t size) noexcept { fontStyle.setFontSize(size); }
            [[nodiscard]] FontAttribute getFontAttribute() const noexcept { return fontStyle.getFontStyle(); }
            void setFontAttribute(FontAttribute style) noexcept { fontStyle.setFontStyle(style); }
            [[nodiscard]] float getFontSpacingFactor() const noexcept { return fontStyle.getCharacterSpacingFactor(); }
            void setFontSpacingFactor(float factor) { fontStyle.setCharacterSpacingFector(factor); }
        protected:

            TextWidget(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const std::string& text, const Font& font);

            virtual void drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) override;

            [[nodiscard]] int32_t getTextWidth() const noexcept { return textWidth; }
            [[nodiscard]] int32_t getTextHeight() const noexcept { return textHeight; }

        private:

            std::string text;
            Font font;
            FontStyle fontStyle;

            int32_t textWidth = 0;
            int32_t textHeight = 0;

    };

}

#pragma once

#include "TextWidget.h"
#include "Clickable.h"
#include "Selectable.h"

#include <functional>

namespace UI {

    class TextInput : public TextWidget, public Clickable, public Selectable {

        public:

            using OnClickCallback = std::function<void(std::shared_ptr<TextInput>)>;
            using OnValueChangeCallback = std::function<void(std::shared_ptr<TextInput>, const std::string&)>;

            friend class Widget;

            virtual ~TextInput() = default;

            static std::shared_ptr<TextInput> create(auto&&... args) {
                return Widget::create<TextInput>(std::forward<decltype(args)>(args)...);
            }

            void handleMouseClickEvent(const MouseClickEvent& event) override;
            void handleKeyboardEvent(const KeyBoardEvent& event) override;

            void reset() override;

            void setOnSelectCallback(const OnClickCallback& callback) noexcept { selectCallback = callback; }
            void setOnValueChangeCallback(const OnValueChangeCallback& callback) noexcept { valueChangeCallback = callback; }

        private:

            TextInput(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const Font& font);

            virtual void drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) override;
            void drawCursor(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom);

            void addCharacter(uint32_t unicodeChar);
            void removeCharacter();
            void removeWord();

            void returnKeyPressed();

            bool selected = false;

            uint32_t cursorBlinkTimer = 0;

            OnClickCallback selectCallback;
            OnValueChangeCallback valueChangeCallback;

    };

}

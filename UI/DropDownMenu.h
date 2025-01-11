#pragma once

#include "Button.h"
#include "TextWidget.h"
#include "Selectable.h"

namespace UI {

    class DropDownMenu : public Selectable {

        public:

            using OnValueChangeCallback = std::function<void(const std::pair<std::string, uint64_t>&)>;

            friend class Widget;

            virtual ~DropDownMenu() = default;

            static std::shared_ptr<DropDownMenu> create(auto&&... args) {
                return Widget::create<DropDownMenu>(std::forward<decltype(args)>(args)...);
            }

            void setStyle(const Style& style) override;

            [[nodiscard]] std::pair<std::string, uint64_t> getSelectedItem() const noexcept { return std::pair<std::string, uint64_t>(label->getText(), selectedRefValue); }
            void setSelectedItem(const std::pair<std::string, uint64_t>& item);
            void setSelectedItem(const std::string& text, uint64_t refValue = 0) { setSelectedItem(std::pair<std::string, uint64_t>(text, refValue)); }
            void addItem(const std::pair<std::string, uint64_t>& item);
            void addItem(const std::string& text, uint64_t refValue = 0) { addItem(std::pair<std::string, uint64_t>(text, refValue)); }

            void setOnValueChangeCallback(const OnValueChangeCallback& callback) noexcept { valueChangeCallback = callback; }

            void reset() override;

        private:

            DropDownMenu(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const Font& font);

            void drawSelf(const RenderTarget&, int32_t, int32_t, float) override {};

            void dropDownButtonPressed(std::shared_ptr<Widget> widget);
            void expand();
            void collapse();

            void init() override;

            void itemClicked(std::shared_ptr<Widget> widget, const std::pair<std::string, uint64_t>& item);

            bool expanded = false;

            std::shared_ptr<Button> downButton;
            std::shared_ptr<TextWidget> label;
            std::shared_ptr<Widget> container;

            std::vector<std::pair<std::string, uint64_t>> items;

            Font font;

            uint64_t selectedRefValue = 0;

            OnValueChangeCallback valueChangeCallback;

    };

}


#include "UI/Window.h"
#include "UI/Button.h"
#include "UI/TextWidget.h"
#include "UI/TextInput.h"
#include "UI/DropDownMenu.h"
#include "UI/Scrollable.h"
#include "UI/Checkbox.h"

int main() {

    UI::Window window("SFML UI Demo Window", 1600, 900);

    const UI::Style style = UI::Style(UI::Color::LIGHT_GRAY,
                                      UI::Color(61, 61, 61),
                                      UI::Color::GRAY, 3);

    const UI::Style labelStyle = UI::Style(UI::Color::LIGHT_GRAY,
                                           UI::Color::TRANSPARENT,
                                           UI::Color::TRANSPARENT, 0);

    const UI::FontStyle fontStyle = UI::FontStyle(33);

    std::shared_ptr<UI::Widget> rootWidget = UI::Widget::create(0, 0, 1600, 900);
    rootWidget->setBackColor(UI::Color(47, 47, 47));
    window.addWidget(rootWidget);

    std::shared_ptr<UI::TextWidget> label = UI::TextWidget::create(25, 25, 300, 50, "Text Label", UI::Font());
    label->setStyle(labelStyle);
    label->setFontSize(33);
    label->setContentAlignmentHorizontal(UI::AlignmentHor::LEFT);
    rootWidget->addChild(label);

    std::shared_ptr<UI::Button> button = UI::Button::create(325, 25, 300, 50, "Button", UI::Font());
    button->setStyle(style);
    button->setFontSize(33);
    rootWidget->addChild(button);

    std::shared_ptr<UI::TextInput> textInput = UI::TextInput::create(25, 125, 600, 50, UI::Font());
    textInput->setText("Text Input");
    textInput->setStyle(style);
    textInput->setFontSize(33);
    textInput->setFontHorizontalOffset(12);
    textInput->setContentAlignmentHorizontal(UI::AlignmentHor::LEFT);
    rootWidget->addChild(textInput);

    std::shared_ptr<UI::DropDownMenu> dropDownMenu = UI::DropDownMenu::create(25, 225, 600, 50, UI::Font());
    dropDownMenu->setStyle(style);
    dropDownMenu->setFontStyle(fontStyle);
    dropDownMenu->setSelectedItem("Drop Down Menu");
    dropDownMenu->addItem("Drop Down Menu");
    dropDownMenu->addItem("Item 1");
    dropDownMenu->addItem("Item 2");
    dropDownMenu->addItem("Item 3");
    rootWidget->addChild(dropDownMenu);

    std::shared_ptr<UI::TextWidget> listLabel = UI::TextWidget::create(25, 325, 300, 50, "Scroll List", UI::Font());
    listLabel->setStyle(labelStyle);
    listLabel->setFontSize(33);
    listLabel->setContentAlignmentHorizontal(UI::AlignmentHor::LEFT);
    rootWidget->addChild(listLabel);

    // TODO: Create real scroll list widget

    std::shared_ptr<UI::Widget> scrollListContainer = UI::Widget::create(25, 400, 600, 225);
    scrollListContainer->setStyle(style);
    rootWidget->addChild(scrollListContainer);

    std::shared_ptr<UI::Scrollable> scrollList = UI::Scrollable::create(0, 0, 600, 225);
    scrollList->setStyle(style);
    scrollList->setScrollableHeight(300);
    scrollListContainer->addChild(scrollList);
    scrollList->registerScrollable();

    for (uint32_t ii = 0; ii < 6; ii++) {
        std::shared_ptr<UI::TextWidget> listItem = UI::TextWidget::create(0, ii * 50, 600, 50, "List Item " + std::to_string(ii + 1), UI::Font());
        listItem->setStyle(style);
        listItem->setBorderWidth(1);
        listItem->setFontSize(33);
        scrollList->addChild(listItem);
    }

    std::shared_ptr<UI::TextWidget> checkBoxLabel = UI::TextWidget::create(25, 675, 200, 50, "Check Box", UI::Font());
    checkBoxLabel->setStyle(labelStyle);
    checkBoxLabel->setFontSize(33);
    checkBoxLabel->setContentAlignmentHorizontal(UI::AlignmentHor::LEFT);
    rootWidget->addChild(checkBoxLabel);

    std::shared_ptr<UI::CheckBox> checkBox = UI::CheckBox::create(210, 675, 50, 50);
    checkBox->setStyle(style);
    rootWidget->addChild(checkBox);

    window.runEventLoop();

    return 0;
};
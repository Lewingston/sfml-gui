
#include "DropDownMenu.h"
#include "Window.h"

using namespace UI;

DropDownMenu::DropDownMenu(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const Font& font) :
    Widget(posX, posY, width, height),
    Selectable(),
    font(font) {

}

void DropDownMenu::init() {
    label = TextWidget::create(0, 0, getWidth()-getHeight(), getHeight(), "", font);
    addChild(label);

    downButton = Button::create(static_cast<int32_t>(getWidth()-getHeight()), 0, getHeight(), getHeight(), "V", font);
    downButton->setStyle(getStyle());
    downButton->setOnClickCallback(std::bind(&DropDownMenu::dropDownButtonPressed, this, std::placeholders::_1));
    addChild(downButton);
}

void DropDownMenu::setStyle(const Style& style) {
    Widget::setStyle(style);
    label->setStyle(style);
    downButton->setStyle(style);
}

void DropDownMenu::setFontStyle(const FontStyle& fontStyle) {
    this->fontStyle = fontStyle;
    label->setFontStyle(fontStyle);
}

void DropDownMenu::setSelectedItem(const std::pair<std::string, uint64_t>& item) {
    selectedRefValue = item.second;
    label->setText(item.first);
}

void DropDownMenu::addItem(const std::pair<std::string, uint64_t>& item) {
    items.push_back(item);
}

void DropDownMenu::dropDownButtonPressed(std::shared_ptr<Widget>) {

    if (expanded)
        collapse();
    else
        expand();

}

void DropDownMenu::itemClicked(std::shared_ptr<Widget>, const std::pair<std::string, uint64_t>& item) {
    setSelectedItem(item);

    collapse();

    if (valueChangeCallback)
        valueChangeCallback(item);
}

void DropDownMenu::expand() {

    if (items.size() == 0)
        return;

    expanded = true;

    std::pair<int32_t, int32_t> absPos = getAbsolutePosition();
    container = Widget::create<Widget>(absPos.first, absPos.second + static_cast<int32_t>(getHeight()), getWidth(), static_cast<uint32_t>(getHeight() * items.size()));

    for (size_t ii = 0; ii < items.size(); ii++) {
        std::shared_ptr<Button> button = Button::create(0, static_cast<int32_t>(getHeight() * ii), getWidth(), getHeight(), items.at(ii).first, label->getFont());
        button->setStyle(getStyle());
        button->setFontStyle(fontStyle);
        button->setOnClickCallback(std::bind(&DropDownMenu::itemClicked, this, std::placeholders::_1, items.at(ii)));
        container->addChild(button);
    }

    std::shared_ptr<RootWidget> rootWidget = getRootWidget();
    if (rootWidget)
        rootWidget->addChild(container);

    registerAsSelected();
}

void DropDownMenu::collapse() {
    expanded = false;
    std::shared_ptr<RootWidget> rootWidget = getRootWidget();
    if (rootWidget) {
        rootWidget->removeChild(container);
        rootWidget->resetSelectedWidget();
    }
}

void DropDownMenu::reset() {
    if (expanded == true)
        collapse();
}

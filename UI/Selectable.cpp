
#include "Selectable.h"
#include "Window.h"

using namespace UI;

bool Selectable::registerAsSelected() {
    std::shared_ptr<RootWidget> root = getRootWidget();
    if (!root)
        return false;
    root->registerSelectedWidget(std::dynamic_pointer_cast<Selectable>(shared_from_this()));
    return true;
}

std::shared_ptr<RootWidget> Selectable::getRootWidget() const {

    std::shared_ptr<Widget> widget = getParent();
    while (widget->getParent()) {
        widget = widget->getParent();
    }

    return std::dynamic_pointer_cast<RootWidget>(widget);
}

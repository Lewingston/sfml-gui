
#include "Clickable.h"

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>

//#include <iostream>

using namespace UI;

Clickable::ClickHandler Clickable::clickHandler;

Clickable::Clickable(int32_t posX, int32_t posY, uint32_t width, uint32_t height) :
    Widget(posX, posY, width, height),
    idColor(0) {

    }

Clickable::~Clickable() {
    clickHandler.removeWidget(idColor);
}

void Clickable::init() {
    idColor = clickHandler.addWidget(std::dynamic_pointer_cast<Clickable>(shared_from_this()));
}

void Clickable::drawChildren(const UI::RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {
    // Dont draw children on click surface so they dont overlay the parent element
    UI::Widget::drawChildren(UI::RenderTarget(renderTarget.getRenderTarget(), nullptr), offsetX, offsetY, zoom);
}

std::shared_ptr<Clickable> Clickable::getClickableByColorId(Color color) {
    return clickHandler.getWidget(color);
}

/******************************************************************************/
/*                        class Clickable::ClickHandler                       */
/******************************************************************************/

Clickable::ClickHandler::ClickHandler() {
    initColors();
}

std::shared_ptr<Clickable> Clickable::ClickHandler::getWidget(Color color) {

    const std::lock_guard<std::mutex> lock(clickHandlerMutex);

    if (auto search = clickMap.find(color); search != clickMap.end()) {
        return search->second.lock();
    } else {
        return nullptr;
    }
}

void Clickable::ClickHandler::initColors() {

    // This algorithm to generate the ids for the clickable elements is pure fluff
    // The algorithm is designed so the color of new generated elements is maximum
    // distinguised from allready generated elements.
    // The only requirement is that no two colors are the same.

    /*constexpr uint32_t TOTAL_MAX_ELEMENTS = 0xff'ff'ff;
    static_assert(TOTAL_MAX_ELEMENTS >= MAX_ELEMENTS);

    constexpr uint32_t numOfIterations = static_cast<uint32_t>(std::log2(TOTAL_MAX_ELEMENTS));

    uint32_t interval = 1 << (numOfIterations - 1);
    uint32_t totalGeneratedIds = 0;

    for (uint32_t ii = 0; ii < numOfIterations && totalGeneratedIds < MAX_ELEMENTS; ii++) {
        uint32_t id = interval;
        bool skip = false;
        while (id <= TOTAL_MAX_ELEMENTS && totalGeneratedIds < MAX_ELEMENTS) {
            if (!skip || ii == 0)
                {
                totalGeneratedIds += 1;
                unusedColors.push_back(Color((id << 8) + 255));
                }
            skip = !skip;
            id += interval;
        }
        interval /= 2;
    }*/

   for (uint32_t ii = 1; ii < MAX_ELEMENTS; ii++) {
        unusedColors.push_back(Color((ii << 8) + 255));
   }
}

Color Clickable::ClickHandler::addWidget(std::shared_ptr<Clickable> clickable) {

    const std::lock_guard<std::mutex> lock(clickHandlerMutex);

    if (unusedColors.size() == 0)
        throw std::out_of_range("No more click ids! Total elements: " + std::to_string(clickMap.size()));

    Color color = unusedColors.front();
    unusedColors.pop_front();
    clickMap.insert({color, clickable});

    //std::cout << "Click: Element added - Use count: " << clickMap.size() << " - Available count: " << unusedColors.size() << std::endl;

    return color;
}

void Clickable::ClickHandler::removeWidget(Color color) {

    const std::lock_guard<std::mutex> lock(clickHandlerMutex);

    clickMap.erase(color);
    unusedColors.push_front(color);

    //std::cout << "Click: Element removoed - Use count: " << clickMap.size() << " - Available count: " << unusedColors.size() << std::endl;
}

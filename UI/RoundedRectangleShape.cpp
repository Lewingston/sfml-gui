
#include "RoundedRectangleShape.h"

#include <cmath>

using namespace UI;

RoundedRectangleShape::RoundedRectangleShape(float width, float height, float cornerRadius, uint32_t cornerLineCount) :
    sf::Shape(),
    width(width),
    height(height),
    cornerRadius(cornerRadius),
    cornerLineCount(cornerLineCount) {

    updatePoints();
}

void RoundedRectangleShape::setSize(float width, float height) {
    this->width = width;
    this->height = height;
    updatePoints();
}

void RoundedRectangleShape::setCornerRadiusAndLineCount(float radius, uint32_t lineCount) {
    cornerRadius = radius;
    cornerLineCount = lineCount;
    updatePoints();
}

std::size_t RoundedRectangleShape::getPointCount() const {

    if (cornerRadius  == 0)
        return 4;
    else
        return (cornerLineCount + 1) * 4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const {
    return points.at(index);
}

void RoundedRectangleShape::updatePoints() {

    const std::size_t pointCount = getPointCount();

    points.clear();
    points.reserve(pointCount);

    float cornerRadius = this->cornerRadius;
    uint32_t lineCount = cornerLineCount;

    if (lineCount == 0)
        cornerRadius = 0;

    if (cornerRadius == 0)
        lineCount = 0;

    double deltaAngel = 0.0;
    if (lineCount > 0)
        deltaAngel = 90.0 / static_cast<double>(lineCount);

    for (std::size_t ii = 0; ii < pointCount; ii++) {

        uint32_t cornerIndex = static_cast<uint32_t>(ii) / (cornerLineCount + 1);
        uint32_t cornerPointIndex = static_cast<uint32_t>(ii) % (cornerLineCount + 1);
        float deltaX = cornerRadius - static_cast<float>(std::cos(deltaAngel * static_cast<double>(cornerPointIndex) * std::numbers::pi / 180)) * cornerRadius;
        float deltaY = cornerRadius - static_cast<float>(std::sin(deltaAngel * static_cast<double>(cornerPointIndex) * std::numbers::pi / 180)) * cornerRadius;

        switch (cornerIndex) {
            case 0: points.push_back(sf::Vector2f(0.0f + deltaX, 0.0f + deltaY)); break;
            case 1: points.push_back(sf::Vector2f(width - deltaY, 0.0f + deltaX)); break;
            case 2: points.push_back(sf::Vector2f(width - deltaX, height - deltaY)); break;
            case 3: points.push_back(sf::Vector2f(0.0f + deltaY, height - deltaX)); break;
            default: break;
        }
    }

    update();
}

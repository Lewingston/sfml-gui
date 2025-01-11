#pragma once

#include <SFML/Graphics/Shape.hpp>

#include <cstdint>

namespace UI {

    class RoundedRectangleShape : public sf::Shape {

        public:

            RoundedRectangleShape(float width, float height, float cornerRadius = 0, uint32_t cornerLineCount = 0);
            virtual ~RoundedRectangleShape() = default;

            void setSize(float width, float height);
            void setCornerRadiusAndLineCount(float radius, uint32_t lineCount);

            [[nodiscard]] virtual std::size_t getPointCount() const override;
            [[nodiscard]] sf::Vector2f getPoint(std::size_t index) const override;

        private:

            float width;
            float height;

            float cornerRadius;
            uint32_t cornerLineCount;

            std::vector<sf::Vector2f> points;

            void updatePoints();

    };
}

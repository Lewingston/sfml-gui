#pragma once

#include "Widget.h"

#include <memory>
#include <map>
#include <string>

namespace sf {
    class Texture;
}

namespace UI {

    class Image : public UI::Widget {

        public:

            virtual ~Image() = default;

            friend class Widget;

            static std::shared_ptr<Image> create(auto&&... args) {
                return Widget::create<Image>(std::forward<decltype(args)>(args)...);
            }

            static std::shared_ptr<sf::Texture> loadImage(const std::string& imagePath);

        protected:

            virtual void drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) override;

        private:

            class ImageLoader {

                public:

                    ImageLoader() = default;
                    virtual ~ImageLoader() = default;

                    std::shared_ptr<sf::Texture> loadImage(const std::string& imagePath);

                private:

                    std::shared_ptr<sf::Texture> loadNewImage(const std::string& imagePath);

                    std::map<std::string, std::shared_ptr<sf::Texture>> imageMap;

            };

            Image(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const std::string& imagePath);

            std::shared_ptr<sf::Texture> image;

            static ImageLoader imageLoader;

    };
}
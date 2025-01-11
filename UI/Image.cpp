
#include "Image.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

using namespace UI;

Image::ImageLoader Image::imageLoader;

Image::Image(int32_t posX, int32_t posY, uint32_t width, uint32_t height, const std::string& imagePath) :
    UI::Widget(posX, posY, width, height),
    image(loadImage(imagePath)) {

    setBackColor(Color::TRANSPARENT);
    setBorderColor(Color::TRANSPARENT);
    setBorderWidth(0);
    }

std::shared_ptr<sf::Texture> Image::loadImage(const std::string& imagePath) {

    return imageLoader.loadImage(imagePath);
}

void Image::drawSelf(const RenderTarget& renderTarget, int32_t offsetX, int32_t offsetY, float zoom) {

    if (!renderTarget.getRenderTarget())
        return;

    Widget::drawSelf(renderTarget, offsetX, offsetY, zoom);

    if (!image)
        return;

    const int32_t posX = getPosX() + offsetX;
    const int32_t posY = getPosY() + offsetY;

    const float scaleX = static_cast<float>(getWidth()) * zoom / static_cast<float>(image->getSize().x);
    const float scaleY = static_cast<float>(getHeight()) * zoom / static_cast<float>(image->getSize().y);

    sf::Sprite sprite;
    sprite.setTexture(*image.get());
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    sprite.setPosition(static_cast<float>(posX) * zoom, static_cast<float>(posY) * zoom);
    renderTarget.getRenderTarget()->draw(sprite);
}

/************************************************************************/
/*                        class Image::ImageLoader                      */
/************************************************************************/

std::shared_ptr<sf::Texture> Image::ImageLoader::loadImage(const std::string& imagePath) {

    auto image = imageMap.find(imagePath);
    if (image == imageMap.end()) {
        return loadNewImage(imagePath);
    } else {
        return image->second;
    }
}

std::shared_ptr<sf::Texture> Image::ImageLoader::loadNewImage(const std::string& imagePath) {

    std::shared_ptr<sf::Texture> image = std::shared_ptr<sf::Texture>(new sf::Texture());
    if (!image->loadFromFile(imagePath)) {
        std::cerr << "Unable to load image: " << imagePath << '\n';
        return nullptr;
    }

    imageMap.insert({imagePath, image});

    return image;
}
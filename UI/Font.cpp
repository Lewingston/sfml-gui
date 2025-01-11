
#include "Font.h"

#include <SFML/Graphics/Font.hpp>

#include <iostream>

using namespace UI;

Font::FontLoader Font::fontLoader;

Font::Font(const std::string& fontPath) :
    font(fontLoader.loadFont(fontPath)) {

}

/******************************************************************************/
/*                             class FontLoader                               */
/******************************************************************************/

std::shared_ptr<sf::Font> Font::FontLoader::loadFont(const std::string& fontPath) {

    // check if font is allready loaded
    auto font = fontMap.find(fontPath);
    if (font == fontMap.end()) {

        // if font is not loaded try to load font
        std::shared_ptr<sf::Font> font = std::shared_ptr<sf::Font>(new sf::Font());
        if (!font->loadFromFile(fontPath)) {
            // handle font load error
            std::cout << "Unable to load font: " << fontPath << std::endl;
            return nullptr;
        }

        // add font to list of loaded fonts
        fontMap.insert({fontPath, font});

        return font;

    } else {
        return font->second;
    }
}

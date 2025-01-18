
#include "Font.h"

#include <SFML/Graphics/Font.hpp>

#include <iostream>

using namespace UI;

extern unsigned char res_consolas_ttf[];
extern unsigned int res_consolas_ttf_len;

Font::FontLoader Font::fontLoader;

std::shared_ptr<sf::Font> Font::defaultFont;

Font::Font(const std::string& fontPath) :
    font(fontLoader.loadFont(fontPath)) {

}

Font::Font() {

    if (!defaultFont) {
        loadDefaultFont();
    }

    font = defaultFont;
}

void Font::loadDefaultFont() {

    defaultFont = std::shared_ptr<sf::Font>(new sf::Font());

    if (!defaultFont->openFromMemory(&res_consolas_ttf, res_consolas_ttf_len)) {
        std::cout << "Failed to load font from program data\n";
        defaultFont.reset();
    }
}

/******************************************************************************/
/*                             class FontLoader                               */
/******************************************************************************/

std::shared_ptr<sf::Font> Font::FontLoader::loadFont(const std::string& fontPath) {

    // check if font is allready loaded
    auto find = fontMap.find(fontPath);
    if (find != fontMap.end()) {
        return find->second;
    }

    // if font is not loaded try to load font
    std::shared_ptr<sf::Font> font = std::shared_ptr<sf::Font>(new sf::Font());
    if (!font->openFromFile(fontPath)) {
        // handle font load error
        std::cout << "Unable to load font: " << fontPath << std::endl;
        return nullptr;
    }

    // add font to list of loaded fonts
    fontMap.insert({fontPath, font});

    return font;
}

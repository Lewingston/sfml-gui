#pragma once

#include <string>
#include <memory>
#include <map>

namespace sf {
    class Font;
}

namespace UI {

    class Font {

        public:

            Font(const std::string& fontPath);
            ~Font() = default;

            [[nodiscard]] std::shared_ptr<sf::Font> getSfFont() const noexcept { return font; }

        private:

            class FontLoader {

                public:

                    FontLoader() = default;
                    virtual ~FontLoader() = default;

                    std::shared_ptr<sf::Font> loadFont(const std::string& fontPath);

                private:

                    std::map<std::string, std::shared_ptr<sf::Font>> fontMap;

            };

            std::shared_ptr<sf::Font> font;

            static FontLoader fontLoader;

    };

}

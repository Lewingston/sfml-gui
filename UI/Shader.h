#pragma once

#include <SFML/Graphics/Shader.hpp>

#include <memory>
#include <string>


namespace UI {

    class Shader {

        public:

            enum class Type {
                VERTEX,
                FRAGMENT
            };

            virtual ~Shader() = default;

            static std::shared_ptr<Shader> load(const std::string& shaderCode, Type type);
            static std::shared_ptr<Shader> loadFromFile(const std::string& filePath, Type type);

            [[nodiscard]] bool isReady() const noexcept { return ready; }

            [[nodiscard]] const sf::Shader& getSfmlShader() const noexcept { return sfmlShader; }

            void setCurrentTexture(const std::string& name);

        private:

            Shader() = default;

            static sf::Shader::Type getSfmlShaderType(Type type);

            sf::Shader sfmlShader;

            bool ready = false; 

    };
}
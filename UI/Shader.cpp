
#include "Shader.h"

#include <iostream>

using namespace UI;

std::shared_ptr<Shader> Shader::load(const std::string& shaderCode, Type type) {

    auto shader = std::shared_ptr<Shader>(new Shader());

    if (sf::Shader::isAvailable()) {
        shader->ready = shader->sfmlShader.loadFromMemory(shaderCode, getSfmlShaderType(type));
        if (!shader->ready) {
            std::cerr << "UI: Failed to load shader:\n";
            std::cerr << shaderCode << '\n';
        }
    } else {
        std::cerr << "UI: Shaders are not available!\n";
    }

    return shader;
}

std::shared_ptr<Shader> Shader::loadFromFile(const std::string& filePath, Type type) {

    auto shader = std::shared_ptr<Shader>(new Shader());

    if (sf::Shader::isAvailable()) {
        shader->ready = shader->sfmlShader.loadFromFile(filePath, getSfmlShaderType(type));
        if (!shader->ready) {
            std::cerr << "UI: Failed to load shader from file: " << filePath << '\n';
        }
    } else {
        std::cerr << "UI: Shaders are not available!\n";
    }

    return shader;
}

sf::Shader::Type Shader::getSfmlShaderType(Type type) {

    if (type == Type::VERTEX)
        return sf::Shader::Type::Vertex;
    else
        return sf::Shader::Type::Fragment;
}

void Shader::setCurrentTexture(const std::string& name) {

    sfmlShader.setUniform(name, sf::Shader::CurrentTexture);
}
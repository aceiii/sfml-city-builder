//
// Created by Borin Ouch on 2016-03-22.
//

#include "texture_manager.h"


TextureManager::TextureManager() {
}

void TextureManager::loadTexture(const std::string &name, const std::string &filename) {
    sf::Texture texture;
    texture.loadFromFile(filename);

    textures[name] = texture;
}

sf::Texture& TextureManager::getRef(const std::string &texture) {
    return textures.at(texture);
}

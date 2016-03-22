//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_TEXTURE_MANAGER_H
#define SFML_TEST_TEXTURE_MANAGER_H


#include <map>
#include <string>

#include <SFML/Graphics.hpp>


class TextureManager {
private:
    std::map<std::string, sf::Texture> textures;

public:
    void loadTexture(const std::string& name, const std::string& filename);

    sf::Texture& getRef(const std::string& texture);

public:
    TextureManager();
};


#endif //SFML_TEST_TEXTURE_MANAGER_H

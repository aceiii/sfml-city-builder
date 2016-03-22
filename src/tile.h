//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_TILE_H
#define SFML_TEST_TILE_H


#include <SFML/Graphics.hpp>
#include <vector>

#include "animation_handler.h"


enum class TileType {
    VOID_TILE,
    GRASS,
    FOREST,
    WATER,
    RESIDENTIAL,
    COMMERCIAL,
    INDUSTRIAL,
    ROAD,
};


std::string tileTypeToStr(TileType type);


class Tile {

public:
    AnimationHandler animHandler;

    sf::Sprite sprite;

    TileType  tileType;

    int tileVariant;
    unsigned int regions[1];
    unsigned int cost;
    double population;
    unsigned int maxPopPerLevel;
    unsigned int maxLevels;
    float production;
    float storedGoods;

public:
    Tile();
    Tile(const unsigned int tileSize, const unsigned int height, sf::Texture& texture,
        const std::vector<Animation>& animations, const TileType tileType,
        const unsigned int cost, const unsigned int maxPopPerLevel,
        const unsigned int maxLevels);

public:
    void draw(sf::RenderWindow& window, float dt);
    void update();

    std::string getCost();
};


#endif //SFML_TEST_TILE_H

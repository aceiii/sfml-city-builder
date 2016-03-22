//
// Created by Borin Ouch on 2016-03-22.
//

#include <tcl.h>
#include "tile.h"


Tile::Tile() {
}

Tile::Tile(const unsigned int tileSize, const unsigned int height, sf::Texture& texture,
           const std::vector<Animation>& animations, const TileType tileType, const unsigned int cost,
           const unsigned int maxPopPerLevel, const unsigned int maxLevels)
{
    this->tileType = tileType;
    this->tileVariant = 0;
    this->regions[0] = 0;

    this->cost = cost;
    this->population = 0;
    this->maxPopPerLevel = maxPopPerLevel;
    this->maxLevels = maxLevels;
    this->production = 0;
    this->storedGoods = 0;

    this->sprite.setOrigin(sf::Vector2f(0.0f, tileSize * (height - 1)));
    this->sprite.setTexture(texture);

    this->animHandler.frameSize = sf::IntRect(0, 0, tileSize * 2, tileSize * height);

    for (auto animation : animations) {
        animHandler.addAnim(animation);
    }

    this->animHandler.update(0.0f);
}

void Tile::draw(sf::RenderWindow &window, float dt) {
    animHandler.changeAnim(tileVariant);
    animHandler.update(dt);

    sprite.setTextureRect(animHandler.bounds);

    window.draw(sprite);
}

void Tile::update() {
    if ((tileType == TileType::RESIDENTIAL ||
        tileType == TileType::COMMERCIAL ||
        tileType == TileType::INDUSTRIAL) &&
        population == maxPopPerLevel * (tileVariant + 1) &&
        tileVariant < maxLevels)
    {
        if (rand() % int(1e4) < 1e2 / (tileVariant + 1)) {
            tileVariant += 1;
        }
    }
}

std::string Tile::getCost() {
    return std::to_string(cost);
}


std::string tileTypeToStr(TileType type) {
    switch (type) {
        default:
        case TileType::VOID_TILE:       return "Void";
        case TileType::GRASS:           return "Grass";
        case TileType::FOREST:          return "Forest";
        case TileType::WATER:           return "Water";
        case TileType::ROAD:            return "Road";
        case TileType::RESIDENTIAL:     return "Residential Zone";
        case TileType::COMMERCIAL:      return "Commercial Zone";
        case TileType::INDUSTRIAL:      return "Industrial Zone";
    }
}

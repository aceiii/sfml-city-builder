//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_MAP_H
#define SFML_TEST_MAP_H


#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <map>

#include "tile.h"


class Map {

private:
    void depthFirstSearch(std::vector<TileType>& whitelist, sf::Vector2i pos, int label, int type);

public:
    unsigned int width;
    unsigned int height;

    std::vector<Tile> tiles;

    std::vector<int> resources;

    unsigned int tileSize;
    unsigned int numSelected;
    unsigned int numRegions[1];

public:
    void load(const std::string& filename, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas);
    void save(const std::string& filename);

    void draw(sf::RenderWindow& window, float dt);

    void findConnectedRegions(std::vector<TileType> whitelist, int type);

    void updateDirection(TileType tileType);

public:
    Map();
    Map(const std::string& filename, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas);

};


#endif //SFML_TEST_MAP_H

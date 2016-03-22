//
// Created by Borin Ouch on 2016-03-22.
//

#include <SFML/Graphics.hpp>

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include "map.h"


Map::Map() {
    tileSize = 0;
    width = 0;
    height = 0;
    numRegions[0] = 1;
}

Map::Map(const std::string &filename, unsigned int width, unsigned int height,
         std::map<std::string, Tile> &tileAtlas)
{
    tileSize = 0;
    load(filename, width, height, tileAtlas);
}

void Map::load(const std::string &filename, unsigned int width, unsigned int height,
               std::map<std::string, Tile> &tileAtlas)
{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);

    this->width = width;
    this->height = height;

    for (int pos = 0; pos < width * height; pos += 1) {
        resources.push_back(255);

        TileType tileType;
        inputFile.read((char*)&tileType, sizeof(int));

        switch (tileType) {
            default:
            case TileType::VOID_TILE:
            case TileType::GRASS:
                tiles.push_back(tileAtlas.at("grass"));
                break;
            case TileType::FOREST:
                tiles.push_back(tileAtlas.at("forest"));
                break;
            case TileType::WATER:
                tiles.push_back(tileAtlas.at("water"));
                break;
            case TileType::RESIDENTIAL:
                tiles.push_back(tileAtlas.at("residential"));
                break;
            case TileType::COMMERCIAL:
                tiles.push_back(tileAtlas.at("commercial"));
                break;
            case TileType::INDUSTRIAL:
                tiles.push_back(tileAtlas.at("industrial"));
                break;
            case TileType::ROAD:
                tiles.push_back(tileAtlas.at("road"));
                break;
        }

        Tile& tile = tiles.back();

        inputFile.read((char*)&tile.tileVariant, sizeof(int));
        inputFile.read((char*)&tile.regions, sizeof(int) * 1);
        inputFile.read((char*)&tile.population, sizeof(double));
        inputFile.read((char*)&tile.storedGoods, sizeof(float));
    }

    inputFile.close();
}

void Map::save(const std::string &filename) {
    std::ofstream outputFile;
    outputFile.open(filename, std::ios::out | std::ios::binary);

    for (auto tile : tiles) {
        outputFile.write((char*)&tile.tileType, sizeof(int));
        outputFile.write((char*)&tile.tileVariant, sizeof(int));
        outputFile.write((char*)&tile.regions, sizeof(int) * 1);
        outputFile.write((char*)&tile.population, sizeof(double));
        outputFile.write((char*)&tile.storedGoods, sizeof(float));
    }

    outputFile.close();
}

void Map::draw(sf::RenderWindow &window, float dt) {
    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            sf::Vector2f pos;
            pos.x = (x - y) * tileSize + width * tileSize;
            pos.y = (x + y) * tileSize * 0.5f;

            tiles[y * width + x].sprite.setPosition(pos);
            tiles[y * width + x].draw(window, dt);
        }
    }
}

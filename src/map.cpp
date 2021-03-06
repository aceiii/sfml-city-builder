//
// Created by Borin Ouch on 2016-03-22.
//

#include <SFML/Graphics.hpp>

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "map.h"


Map::Map() {
    numSelected = 0;
    tileSize = 8;
    width = 0;
    height = 0;
    numRegions[0] = 1;
}

Map::Map(const std::string &filename, unsigned int width, unsigned int height,
         std::map<std::string, Tile> &tileAtlas)
{
    numSelected = 0;
    tileSize = 8;
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
        selected.push_back(0);

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

            if (selected[y * width + x]) {
                tiles[y * width + x].sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
            } else {
                tiles[y * width + x].sprite.setColor(sf::Color(0xff, 0xff, 0xff));
            }

            tiles[y * width + x].draw(window, dt);
        }
    }
}

void Map::updateDirection(TileType tileType) {
    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            int pos = y * width + x;

            if (tiles[pos].tileType != tileType) {
                continue;
            }

            bool adjacentTiles[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

            if (x > 0 && y > 0) {
                adjacentTiles[0][0] = (tiles[(y - 1) * width + (x - 1)].tileType == tileType);
            }
            if (y > 0) {
                adjacentTiles[0][1] = (tiles[(y - 1) * width + x].tileType == tileType);
            }
            if (x < width - 1 && y > 0) {
                adjacentTiles[0][2] = (tiles[(y - 1) * width + (x + 1)].tileType == tileType);
            }
            if (x > 0) {
                adjacentTiles[1][0] = (tiles[y * width + (x - 1)].tileType == tileType);
            }
            if (x < width - 1) {
                adjacentTiles[1][2] = (tiles[y * width + (x + 1)].tileType == tileType);
            }
            if (x > 0 && y < height - 1) {
                adjacentTiles[2][0] = (tiles[(y + 1) * width + x].tileType == tileType);
            }
            if (y < height - 1) {
                adjacentTiles[2][1] = (tiles[(y + 1) * width + x].tileType == tileType);
            }
            if (x < width - 1 && y < height - 1) {
                adjacentTiles[2][2] = (tiles[(y + 1) * width + (x + 1)].tileType == tileType);
            }

            if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1] && adjacentTiles[2][1]) {
                tiles[pos].tileVariant = 2;
            } else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1]) {
                tiles[pos].tileVariant = 7;
            } else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[2][1]) {
                tiles[pos].tileVariant = 8;
            } else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][0]) {
                tiles[pos].tileVariant = 9;
            } else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][2]) {
                tiles[pos].tileVariant = 10;
            } else if (adjacentTiles[1][0] && adjacentTiles[1][2]) {
                tiles[pos].tileVariant = 0;
            } else if (adjacentTiles[0][1] && adjacentTiles[2][1]) {
                tiles[pos].tileVariant = 1;
            } else if (adjacentTiles[2][1] && adjacentTiles[1][0]) {
                tiles[pos].tileVariant = 3;
            } else if (adjacentTiles[0][1] && adjacentTiles[1][2]) {
                tiles[pos].tileVariant = 4;
            } else if (adjacentTiles[1][0] && adjacentTiles[0][1]) {
                tiles[pos].tileVariant = 5;
            } else if (adjacentTiles[2][1] && adjacentTiles[1][2]) {
                tiles[pos].tileVariant = 6;
            } else if (adjacentTiles[1][0]) {
                tiles[pos].tileVariant = 0;
            } else if (adjacentTiles[1][2]) {
                tiles[pos].tileVariant = 0;
            } else if (adjacentTiles[0][1]) {
                tiles[pos].tileVariant = 1;
            } else if (adjacentTiles[2][1]) {
                tiles[pos].tileVariant = 1;
            }
        }
    }
}

void Map::findConnectedRegions(std::vector<TileType> whitelist, int regionType) {
    int regions = 1;

    for (auto& tile : tiles) {
        tile.regions[regionType] = 0;
    }

    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            bool found = false;

            for (auto type : whitelist) {
                if (type == tiles[y * width + x].tileType) {
                    found = true;
                    break;
                }
            }

            if (tiles[y * width + x].regions[regionType] == 0 && found) {
                depthFirstSearch(whitelist, sf::Vector2i(x, y), regions++, regionType);
            }
        }
    }

    numRegions[regionType] = regions;
}

void Map::depthFirstSearch(std::vector<TileType>& whitelist, sf::Vector2i pos, int label, int regionType = 0) {
    if (pos.x < 0 || pos.x >= width) {
        return;
    }
    if (pos.y < 0 || pos.y >= height) {
        return;
    }
    if (tiles[pos.y * width + pos.x].regions[regionType] != 0) {
        return;
    }

    bool found = false;

    for (auto type : whitelist) {
        if (type == tiles[pos.y * width + pos.x].tileType) {
            found = true;
            break;
        }
    }

    if (!found) {
        return;
    }

    tiles[pos.y * width + pos.x].regions[regionType] = label;

    depthFirstSearch(whitelist, pos + sf::Vector2i(-1, 0), label, regionType);
    depthFirstSearch(whitelist, pos + sf::Vector2i(0, 1), label, regionType);
    depthFirstSearch(whitelist, pos + sf::Vector2i(1, 0), label, regionType);
    depthFirstSearch(whitelist, pos + sf::Vector2i(0, -1), label, regionType);
}

void Map::select(sf::Vector2i start, sf::Vector2i end, std::vector<TileType> blacklist) {
    if (end.y < start.y) {
        std::swap(start.y, end.y);
    }
    if (end.x < start.x) {
        std::swap(start.x, end.x);
    }

    if (end.x >= width) {
        end.x = width - 1;
    } else if (end.x < 0) {
        end.x = 0;
    }

    if (end.y >= height) {
        end.y = height - 1;
    } else if (end.y < 0) {
        end.y = 0;
    }

    if (start.x >= width) {
        start.x = width - 1;
    } else if (start.x < 0) {
        start.x = 0;
    }

    if (start.y >= height) {
        start.y = height - 1;
    } else if (start.y < 0) {
        start.y = 0;
    }

    for (int y = start.y; y <= end.y; y += 1) {
        for (int x = start.x; x <= end.x; x += 1) {
            selected[y * width + x] = 1;

            numSelected += 1;

            for (auto type : blacklist) {
                if (tiles[y * width + x].tileType == type) {
                    selected[y * width + x] = 2;
                    numSelected -= 1;
                    break;
                }
            }
        }
    }
}

void Map::clearSelected() {
    for (auto& tile : selected) {
        tile = 0;
    }

    numSelected = 0;
}

//
// Created by Borin Ouch on 2016-03-24.
//


#include <numeric>

#include "city.h"


City::City() {
    birthRate = 0.00055;
    deathRate = 0.00023;
    propCanWork = 0.50;
    populationPool = 0;
    population = populationPool;
    employmentPool = 0;
    employable = employmentPool;
    residentialTax = 0.05;
    commercialTax = 0.05;
    industrialTax = 0.05;
    earnings = 0;
    funds = 0;
    currentTime = 0;
    timePerDay = 1.0;
    day = 0;
};

City::City(std::string cityName, unsigned int tileSize, std::map<std::string, Tile> &tileAtlas):City() {
    map.tileSize = tileSize;
    load(cityName, tileAtlas);
}

void City::load(std::string cityName, std::map<std::string, Tile> &tileAtlas) {

}

void City::save(std::string cityName) {

}

void City::update(float dt) {

}

void City::bulldoze(const Tile &tile) {
    for (int pos = 0; pos < map.width * map.height; pos += 1) {
        if (map.selected[pos] == 1) {
            if (map.tiles[pos].tileType == TileType::RESIDENTIAL) {
                populationPool += map.tiles[pos].population;
            } else if (map.tiles[pos].tileType == TileType::COMMERCIAL) {
                employmentPool += map.tiles[pos].population;
            } else if (map.tiles[pos].tileType == TileType::INDUSTRIAL) {
                employmentPool += map.tiles[pos].population;
            }
            map.tiles[pos] = tile;
        }
    }
}

void City::shuffleTiles() {
    while (shuffledTiles.size() < map.tiles.size()) {
        shuffledTiles.push_back(0);
    }

    std::iota(shuffledTiles.begin(), shuffledTiles.end(), 1);
    std::random_shuffle(shuffledTiles.begin(), shuffledTiles.end());
}

void City::tileChanged() {
    map.updateDirection(TileType::ROAD);
    map.findConnectedRegions({ TileType::ROAD, TileType::RESIDENTIAL, TileType::COMMERCIAL, TileType::INDUSTRIAL }, 0);
}

double City::getHomeless() {
    return populationPool;
}

double City::getUnemployed() {
    return employmentPool;
}

double City::distributionPool(double &pool, Tile &tile, double rate) {
    const static int moveRate = 4;

    double maxPop = tile.maxPopPerLevel * (tile.tileVariant + 1);

    if (pool > 0) {
        double moving = maxPop - tile.population;

        if (moving > moveRate) {
            moving = moveRate;
        }

        if (pool - moving < 0) {
            moving = pool;
        }

        pool -= moving;
        tile.population += moving;
    }

    tile.population += tile.population * rate;

    if (tile.population > maxPop) {
        pool += tile.population - maxPop;
        tile.population = maxPop;
    }

    return tile.population;
}

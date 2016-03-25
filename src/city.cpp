//
// Created by Borin Ouch on 2016-03-24.
//


#include <numeric>
#include <fstream>
#include <iostream>
#include <sstream>

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
    int width = 0;
    int height = 0;

    std::ifstream inputFile("data/config/" + cityName + "_cfg.dat", std::ios::in);

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream lineStream(line);
        std::string key;
        if (std::getline(lineStream, key, '=')) {
            std::string value;
            if (std::getline(lineStream, value)) {
                if (key == "width") {
                    width = std::stoi(value);
                } else if (key == "height") {
                    height = std::stoi(value);
                } else if (key == "day") {
                    day = std::stoi(value);
                } else if (key == "populationPool") {
                    populationPool = std::stod(value);
                } else if (key == "employmentPool") {
                    employmentPool = std::stod(value);
                } else if (key == "population") {
                    population = std::stod(value);
                } else if (key == "employable") {
                    employable = std::stod(value);
                } else if (key == "birthRate") {
                    birthRate = std::stod(value);
                } else if (key == "deathRate") {
                    deathRate = std::stod(value);
                } else if (key == "residentialTax") {
                    residentialTax = std::stod(value);
                } else if (key == "commercialTax") {
                    commercialTax = std::stod(value);
                } else if (key == "industrialTax") {
                    industrialTax = std::stod(value);
                } else if (key == "funds") {
                    funds = std::stod(value);
                } else if (key == "earnings") {
                    earnings = std::stod(value);
                }
            } else {
                std::cerr << "Error, no value for key " << key << std::endl;
            }
        }
    }

    inputFile.close();

    map.load("data/maps/" + cityName + "_map.dat", width, height, tileAtlas);

    tileChanged();
}

void City::save(std::string cityName) {
    std::ofstream outputFile("data/config/" + cityName + "_cfg.dat", std::ios::out);

    outputFile << "width=" << map.width << std::endl;
    outputFile << "height=" << map.height << std::endl;
    outputFile << "day=" << day << std::endl;
    outputFile << "populationPool=" << populationPool << std::endl;
    outputFile << "employmentPool=" << employmentPool << std::endl;
    outputFile << "population=" << population << std::endl;
    outputFile << "employable=" << employable << std::endl;
    outputFile << "birthRate=" << birthRate << std::endl;
    outputFile << "deathRate=" << deathRate << std::endl;
    outputFile << "residentialTax=" << residentialTax << std::endl;
    outputFile << "commercialTax=" << commercialTax << std::endl;
    outputFile << "industrialTax=" << industrialTax << std::endl;
    outputFile << "funds=" << funds << std::endl;
    outputFile << "earnings=" << earnings << std::endl;

    outputFile.close();

    map.save("data/maps/" + cityName + "_map.dat");
}

void City::update(float dt) {
    double popTotal = 0;
    double commercialRevenue = 0;
    double industrialRevenue = 0;

    currentTime += dt;
    if (currentTime < timePerDay) {
        return;
    }

    day += 1;
    currentTime = 0;

    if (day % 30 == 0) {
        funds += earnings;
        earnings = 0;
    }

    for (int i = 0; i < map.tiles.size(); i += 1) {
        Tile& tile = map.tiles[shuffledTiles[i]];

        if (tile.tileType == TileType::RESIDENTIAL) {
            distributionPool(populationPool, tile, birthRate - deathRate);
            popTotal += tile.population;
        } else if (tile.tileType == TileType::COMMERCIAL) {
            if (rand() % 100 < 15 * (1.0 - commercialTax)) {
                distributionPool(employmentPool, tile, 0.0);
            }
        } else if (tile.tileType == TileType::INDUSTRIAL) {
            if (map.resources[i] > 0 && rand() % 100 < population) {
                tile.population += 1;
                map.resources[i] -= 1;
            }

            if (rand() % 100 < 15 * (1.0 - industrialTax)) {
                distributionPool(employmentPool, tile, 0.0);
            }
        }

        tile.update();
    }

    for (int i = 0; i < map.tiles.size(); i += 1) {
        Tile& tile = map.tiles[shuffledTiles[i]];

        if (tile.tileType == TileType::INDUSTRIAL) {
            int receivedResources = 0;

            for (auto& tile2 : map.tiles) {
                if (tile2.regions[0] == tile.regions[0] && tile2.tileType == TileType::INDUSTRIAL) {
                    if (tile2.production > 0) {
                        receivedResources += 1;
                        tile2.production -= 1;
                    }
                    if (receivedResources >= tile.tileVariant + 1) {
                        break;
                    }
                }
            }

            tile.storedGoods += (receivedResources + tile.production) * (tile.tileVariant + 1);
        }
    }

    for (int i = 0; i < map.tiles.size(); i += 1) {
        Tile& tile = map.tiles[shuffledTiles[i]];

        if (tile.tileType == TileType::COMMERCIAL) {
            int receivedGoods = 0;
            double maxCustomers = 0.0;

            for (auto& tile2 : map.tiles) {
                if (tile2.regions[0] == tile.regions[0] && tile2.tileType == TileType::INDUSTRIAL && tile2.storedGoods > 0) {
                    while (tile2.storedGoods > 0 && receivedGoods != tile.tileVariant + 1) {
                        tile2.storedGoods -= 1;
                        receivedGoods += 1;
                        industrialRevenue += 100 * (1.0 - industrialTax);
                    }
                } else if (tile2.regions[0] == tile.regions[0] && tile2.tileType == TileType::RESIDENTIAL) {
                    maxCustomers += tile2.population;
                }

                if (receivedGoods == tile.tileVariant + 1) {
                    break;
                }
            }

            tile.production = float((receivedGoods * 100.0 + rand() % 20) * (1.0 - commercialTax));

            double revenue = tile.production * maxCustomers * tile.population / 100.0;

            commercialRevenue += revenue;
        }
    }

    populationPool = adjustPopulation(populationPool, birthRate - deathRate);
    popTotal += populationPool;

    float newWorkers = (popTotal - population) * propCanWork;
    newWorkers *= (newWorkers < 0 ? -1 : 1);

    employmentPool += newWorkers;
    employable += newWorkers;

    if (employmentPool < 0) {
        employmentPool = 0;
    }

    if (employable < 0) {
        employable = 0;
    }

    population = popTotal;

    earnings = (population - populationPool) * 15 * residentialTax;
    earnings += commercialRevenue * commercialTax;
    earnings += industrialRevenue * industrialTax;
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

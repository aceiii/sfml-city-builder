//
// Created by Borin Ouch on 2016-03-24.
//

#ifndef SFML_TEST_CITY_H
#define SFML_TEST_CITY_H


#include <vector>
#include <Map>

#include "map.h"


class City {

private:
    float currentTime;
    float timePerDay;

    std::vector<int> shuffledTiles;

    double populationPool;
    double employmentPool;
    float propCanWork;

    double birthRate;
    double deathRate;

private:
    double distributionPool(double& pool, Tile& tile, double rate);

public:
    Map map;

    double population;
    double employable;

    double residentialTax;
    double commercialTax;
    double industrialTax;

    double earnings;
    double funds;

    int day;

public:
    City();
    City(std::string cityName, unsigned int tileSize, std::map<std::string, Tile>& tileAtlas);

public:
    void load(std::string cityName, std::map<std::string, Tile>& tileAtlas);
    void save(std::string cityName);

    void update(float dt);
    void bulldoze(const Tile& tile);
    void shuffleTiles();
    void tileChanged();

    double getHomeless();
    double getUnemployed();
};


#endif //SFML_TEST_CITY_H

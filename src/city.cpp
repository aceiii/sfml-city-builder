//
// Created by Borin Ouch on 2016-03-24.
//

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

}

void City::shuffleTiles() {

}

void City::tileChanged() {

}

double City::getHomeless() {
    return populationPool;
}

double City::getUnemployed() {
    return employmentPool;
}

double City::distributionPool(double &pool, Tile &tile, double rate) {
    return 0;
}

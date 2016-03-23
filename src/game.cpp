//
// Created by Borin Ouch on 2016-03-22.
//

#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "game.h"
#include "game_state.h"
#include "animation_handler.h"


void Game::pushState(GameState* state) {
    states.push(state);
}

void Game::popState() {
    delete states.top();
    states.pop();
}

void Game::changeState(GameState *state) {
    if (!states.empty()) {
        popState();
    }
    pushState(state);
}

GameState* Game::peekState() {
    if (states.empty()) {
        return nullptr;
    }
    return states.top();
}

void Game::gameLoop() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        if (peekState() == nullptr) {
            continue;
        }

        peekState()->handleInput();
        peekState()->update(dt);

        window.clear(sf::Color::Black);

        peekState()->draw(dt);

        window.display();
    }
}

Game::Game() {
    loadTextures();
    loadTiles();

    window.create(sf::VideoMode(800, 600), "City Builder");
    window.setFramerateLimit(60);

    background.setTexture(texmgr.getRef("background"));
}

Game::~Game() {
    while (!states.empty()) {
        popState();
    }
}

void Game::loadTextures() {
    texmgr.loadTexture("grass", "data/media/grass.png");
    texmgr.loadTexture("forest", "data/media/forest.png");
    texmgr.loadTexture("water", "data/media/water.png");
    texmgr.loadTexture("residential", "data/media/residential.png");
    texmgr.loadTexture("commercial", "data/media/commercial.png");
    texmgr.loadTexture("industrial", "data/media/industrial.png");
    texmgr.loadTexture("road", "data/media/road.png");

    texmgr.loadTexture("background", "data/media/background.png");
}

void Game::loadTiles() {
    Animation staticAnim(0, 0, 1.0f);

    tileAtlas["grass"] = Tile(tileSize, 1, texmgr.getRef("grass"), { staticAnim }, TileType::GRASS, 50, 0, 1);
    tileAtlas["forest"] = Tile(tileSize, 1, texmgr.getRef("forest"), { staticAnim }, TileType::FOREST, 100, 0, 1);
    tileAtlas["water"] =
        Tile(tileSize, 1, texmgr.getRef("water"),
             { Animation(0, 3, 0.5f), Animation(0, 3, 0.5f), Animation(0, 3, 0.5f) },
             TileType::WATER, 0, 0, 1);
    tileAtlas["residential"] =
        Tile(tileSize, 2, texmgr.getRef("residential"),
             { staticAnim, staticAnim, staticAnim,
             staticAnim, staticAnim, staticAnim },
             TileType::RESIDENTIAL, 300, 50, 6);
    tileAtlas["commercial"] =
        Tile(tileSize, 2, texmgr.getRef("commercial"),
             { staticAnim, staticAnim, staticAnim, staticAnim },
             TileType::COMMERCIAL, 300, 50, 4);
    tileAtlas["industrial"] =
        Tile(tileSize, 2, texmgr.getRef("industrial"),
             { staticAnim, staticAnim, staticAnim, staticAnim },
             TileType::INDUSTRIAL, 300, 50, 4);
    tileAtlas["road"] =
        Tile(tileSize, 1, texmgr.getRef("road"),
             { staticAnim, staticAnim, staticAnim,
             staticAnim, staticAnim, staticAnim,
             staticAnim, staticAnim, staticAnim,
             staticAnim, staticAnim },
             TileType::ROAD, 100, 0, 1);
}

void Game::loadStylesheets() {
    stylesheets["button"] = GuiStyle(&fonts.at("main_font"), 1,
                                     sf::Color(0xc6, 0xc6, 0xc6), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00),
                                     sf::Color(0x61, 0x61, 0x61), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00));
    stylesheets["text"] = GuiStyle(&fonts.at("main_font"), 0,
                                     sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0xff, 0xff),
                                     sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0x00, 0x00));
}

void Game::loadFonts() {
    sf::Font font;
    font.loadFromFile("data/media/font.tff");

    fonts["main_font"] = font;
}

//
// Created by Borin Ouch on 2016-03-22.
//

#include <stack>

#include <SFML/Graphics.hpp>

#include "game.h"
#include "game_state.h"
#include "resource_manager.h"


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
    initResourceManager();

    loadTextures();
    loadTiles();
    loadFonts();
    loadStylesheets();

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
    ResourceManager* resourceManager = ResourceManager::getInstance();

    texmgr.loadTexture(tileTypeShortName(TileType::GRASS), resourceManager->getResourcePath("grass.png"));
    texmgr.loadTexture(tileTypeShortName(TileType::FOREST), resourceManager->getResourcePath("forest.png"));
    texmgr.loadTexture(tileTypeShortName(TileType::WATER), resourceManager->getResourcePath("water.png"));
    texmgr.loadTexture(tileTypeShortName(TileType::RESIDENTIAL), resourceManager->getResourcePath("residential.png"));
    texmgr.loadTexture(tileTypeShortName(TileType::COMMERCIAL), resourceManager->getResourcePath("commercial.png"));
    texmgr.loadTexture(tileTypeShortName(TileType::INDUSTRIAL), resourceManager->getResourcePath("industrial.png"));
    texmgr.loadTexture(tileTypeShortName(TileType::ROAD), resourceManager->getResourcePath("road.png"));
    texmgr.loadTexture("background", resourceManager->getResourcePath("background.png"));
}

void Game::loadTiles() {
    Animation staticAnim(0, 0, 1.0f);

    tileAtlas[tileTypeShortName(TileType::GRASS)] =
        Tile(tileSize, 1, texmgr.getRef(tileTypeShortName(TileType::GRASS)),
             { staticAnim }, TileType::GRASS, 50, 0, 1);
    tileAtlas[tileTypeShortName(TileType::FOREST)] =
        Tile(tileSize, 1, texmgr.getRef(tileTypeShortName(TileType::FOREST)),
             { staticAnim }, TileType::FOREST, 100, 0, 1);
    tileAtlas[tileTypeShortName(TileType::WATER)] =
        Tile(tileSize, 1, texmgr.getRef(tileTypeShortName(TileType::WATER)),
             { Animation(0, 3, 0.5f), Animation(0, 3, 0.5f), Animation(0, 3, 0.5f) },
             TileType::WATER, 0, 0, 1);
    tileAtlas[tileTypeShortName(TileType::RESIDENTIAL)] =
        Tile(tileSize, 2, texmgr.getRef(tileTypeShortName(TileType::RESIDENTIAL)),
             { staticAnim, staticAnim, staticAnim,
             staticAnim, staticAnim, staticAnim },
             TileType::RESIDENTIAL, 300, 50, 6);
    tileAtlas[tileTypeShortName(TileType::COMMERCIAL)] =
        Tile(tileSize, 2, texmgr.getRef(tileTypeShortName(TileType::COMMERCIAL)),
             { staticAnim, staticAnim, staticAnim, staticAnim },
             TileType::COMMERCIAL, 300, 50, 4);
    tileAtlas[tileTypeShortName(TileType::INDUSTRIAL)] =
        Tile(tileSize, 2, texmgr.getRef(tileTypeShortName(TileType::INDUSTRIAL)),
             { staticAnim, staticAnim, staticAnim, staticAnim },
             TileType::INDUSTRIAL, 300, 50, 4);
    tileAtlas[tileTypeShortName(TileType::ROAD)] =
        Tile(tileSize, 1, texmgr.getRef(tileTypeShortName(TileType::ROAD)),
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
    font.loadFromFile(ResourceManager::getInstance()->getResourcePath("font.ttf"));

    fonts["main_font"] = font;
}

void Game::initResourceManager() {
    ResourceManager* resourceManager = ResourceManager::getInstance();

    resourceManager->setBasePath("data");

    resourceManager->addResourceMapping("dat", "maps");
    resourceManager->addResourceMapping("ttf", "fonts");
    resourceManager->addResourceMapping("png", "media");
    resourceManager->addResourceMapping("jpg", "media");
    resourceManager->addResourceMapping("jpeg", "media");
    resourceManager->addResourceMapping("gif", "media");
}

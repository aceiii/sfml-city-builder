//
// Created by Borin Ouch on 2016-03-22.
//

#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "game.h"
#include "game_state.h"


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
    texmgr.loadTexture("background", "data/media/background.png");
}

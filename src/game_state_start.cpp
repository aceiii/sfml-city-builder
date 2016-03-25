//
// Created by Borin Ouch on 2016-03-22.
//


#include <iostream>
#include <utility>

#include "game_state_start.h"
#include "game_state_editor.h"


void GameStateStart::draw(const float dt) {
    game->window.setView(view);
    game->window.clear(sf::Color::Black);
    game->window.draw(game->background);

    for (auto gui : guiSystem) {
        game->window.draw(gui.second);
    }
}

void GameStateStart::update(const float dt) {

}

void GameStateStart::handleInput() {
    sf::Event event;

    sf::Vector2f mousePos = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window), view);

    while (game->window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
            {
                game->window.close();
                break;
            }
            case sf::Event::Resized:
            {
                view.setSize(event.size.width, event.size.height);

                game->background.setPosition(game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
                game->background.setScale(float(event.size.width) / float(game->background.getTexture()->getSize().x),
                                          float(event.size.height) / float(game->background.getTexture()->getSize().y));

                sf::Vector2f pos(event.size.width, event.size.height);
                pos *= 0.5f;
                pos = game->window.mapPixelToCoords(sf::Vector2i(pos), view);

                guiSystem.at("menu").setPosition(pos);

                game->background.setScale(
                    float(event.size.width) / float(game->background.getTexture()->getSize().x),
                    float(event.size.height) / float(game->background.getTexture()->getSize().y));

                break;
            }
            case sf::Event::MouseMoved:
            {
                guiSystem.at("menu").highlight(guiSystem.at("menu").getEntry(mousePos));
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    std::string msg = guiSystem.at("menu").activate(mousePos);

                    if (msg == "load_game") {
                        loadGame();
                    }
                }
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Escape) {
                    game->window.close();
                }
                /*else if (event.key.code == sf::Keyboard::Space) {
                    loadGame();
                }*/
                break;
            }
            default:
                break;
        }
    }
}

GameStateStart::GameStateStart(Game *game) {
    this->game = game;

    sf::Vector2f size = sf::Vector2f(game->window.getSize());
    sf::Vector2f center = size * 0.5f;

    view.setSize(size);
    view.setCenter(center);

    guiSystem.emplace(std::string("menu"),
                      Gui(sf::Vector2f(192, 32), 4, false,
                          game->stylesheets.at("button"),
                          { std::make_pair("Load Game", "load_game") }));

    guiSystem.at("menu").setPosition(center);
    guiSystem.at("menu").setOrigin(96, 16);
    guiSystem.at("menu").show();
}

void GameStateStart::loadGame() {
    game->pushState(new GameStateEditor(game));
}

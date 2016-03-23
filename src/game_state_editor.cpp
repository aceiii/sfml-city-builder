//
// Created by Borin Ouch on 2016-03-22.
//

#include "game_state_editor.h"


void GameStateEditor::draw(const float dt) {
    game->window.clear(sf::Color::Black);

    game->window.setView(guiView);
    game->window.draw(game->background);

    game->window.setView(gameView);
    map.draw(game->window, dt);
}

void GameStateEditor::update(const float dt) {
}

void GameStateEditor::handleInput() {
    sf::Event event;

    while (game->window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
            {
                game->window.close();
                break;
            }
            case sf::Event::Resized:
            {
                gameView.setCenter(event.size.width, event.size.height);
                gameView.setSize(event.size.width, event.size.height);

                game->background.setPosition(game->window.mapPixelToCoords(sf::Vector2i(0, 0), guiView));
                game->background.setScale(float(event.size.width) / float(game->background.getTexture()->getSize().x),
                                          float(event.size.height) / float(game->background.getTexture()->getSize().y));
                break;
            }
            default:
                break;
        }
    }
}

GameStateEditor::GameStateEditor(Game *game) {
    this->game = game;

    sf::Vector2f size = sf::Vector2f(game->window.getSize());
    sf::Vector2f center = size * 0.5f;

    guiView.setSize(size);
    gameView.setSize(size);

    guiView.setCenter(center);
    gameView.setCenter(center);

    map = Map("data/maps/city_map.dat", 64, 64, game->tileAtlas);

    zoomLevel = 1.0f;

    sf::Vector2f mapCenter(map.width, map.height * 0.5);
    mapCenter *= float(map.tileSize);

    gameView.setCenter(mapCenter);

    actionState = ActionState::NONE;
}

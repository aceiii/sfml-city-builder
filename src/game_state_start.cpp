//
// Created by Borin Ouch on 2016-03-22.
//


#include "game_state_start.h"
#include "game_state_editor.h"


void GameStateStart::draw(const float dt) {
    game->window.setView(view);
    game->window.clear(sf::Color::Black);
    game->window.draw(game->background);
}

void GameStateStart::update(const float dt) {

}

void GameStateStart::handleInput() {
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
                view.setSize(event.size.width, event.size.height);

                game->background.setPosition(game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
                game->background.setScale(float(event.size.width) / float(game->background.getTexture()->getSize().x),
                                          float(event.size.height) / float(game->background.getTexture()->getSize().y));
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Escape) {
                    game->window.close();
                }
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

    view.setSize(size);
    view.setCenter(size * 0.5f);
}


void GameStateStart::loadGame() {
    game->pushState(new GameStateEditor(game));
}

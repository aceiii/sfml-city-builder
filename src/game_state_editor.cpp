//
// Created by Borin Ouch on 2016-03-22.
//


#include <iostream>

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
            case sf::Event::MouseMoved:
            {
                if (actionState == ActionState::PANNING) {
                    sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(game->window) - panningAnchor);
                    gameView.move(-1.0f * pos * zoomLevel);
                    panningAnchor = sf::Mouse::getPosition(game->window);
                } else if (actionState == ActionState::SELECTING) {
                    sf::Vector2f pos = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window), gameView);

                    selectionEnd.x = int(pos.y / map.tileSize + pos.x / (2 * map.tileSize) - map.width * 0.5 - 0.5);
                    selectionEnd.y = int(pos.y / map.tileSize - pos.x / (2 * map.tileSize) + map.width * 0.5 + 0.5);

                    map.clearSelected();

                    if (currentTile->tileType == TileType::GRASS) {
                        map.select(selectionStart, selectionEnd, { currentTile->tileType, TileType::WATER });
                    } else {
                        map.select(selectionStart, selectionEnd, {
                            currentTile->tileType, TileType::FOREST,
                            TileType::WATER, TileType::ROAD,
                            TileType::RESIDENTIAL, TileType::COMMERCIAL,
                            TileType::INDUSTRIAL
                        });
                    }
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    if (actionState != ActionState::PANNING) {
                        actionState = ActionState::PANNING;
                        panningAnchor = sf::Mouse::getPosition(game->window);
                    }
                } else if (event.mouseButton.button == sf::Mouse::Left) {
                    if (actionState != ActionState::SELECTING) {
                        actionState = ActionState::SELECTING;

                        sf::Vector2f pos = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window), gameView);

                        selectionStart.x = int(pos.y / map.tileSize + pos.x / (2.0f * map.tileSize) - map.width * 0.5f - 0.5f);
                        selectionStart.y = int(pos.y / map.tileSize - pos.x / (2.0f * map.tileSize) + map.width * 0.5f + 0.5f);
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (actionState == ActionState::SELECTING) {
                        actionState = ActionState::NONE;
                        map.clearSelected();
                    }
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    actionState = ActionState::NONE;
                } else if (event.mouseButton.button == sf::Mouse::Left) {
                    if (actionState == ActionState::SELECTING) {
                        actionState = ActionState::NONE;
                        map.clearSelected();
                    }
                }
                break;
            }
            case sf::Event::MouseWheelMoved:
            {
                if (event.mouseWheel.delta < 0) {
                    gameView.zoom(2.0f);
                    zoomLevel *= 2.0f;
                } else if (event.mouseWheel.delta > 0) {
                    gameView.zoom(0.5f);
                    zoomLevel *= 0.5f;
                };
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

    sf::Vector2f mapCenter(map.width, map.height * 0.5f);
    mapCenter *= float(map.tileSize);

    gameView.setCenter(mapCenter);

    selectionStart = sf::Vector2i(0, 0);
    selectionEnd = sf::Vector2i(0, 0);

    currentTile = &game->tileAtlas.at("grass");

    actionState = ActionState::NONE;
}

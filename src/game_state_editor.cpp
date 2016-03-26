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
    city.map.draw(game->window, dt);

    game->window.setView(guiView);
    for (auto gui : guiSystem) {
        game->window.draw(gui.second);
    }
}

void GameStateEditor::update(const float dt) {
    city.update(dt);

    guiSystem.at("infoBar").setEntryText(0, "Day: " + std::to_string(city.day));
    guiSystem.at("infoBar").setEntryText(1, "$" + std::to_string(long(city.funds)));
    guiSystem.at("infoBar").setEntryText(2, std::to_string(long(city.population)) + " (" + std::to_string(long(city.getHomeless())) + ")");
    guiSystem.at("infoBar").setEntryText(3, std::to_string(long(city.employable)) + " (" + std::to_string(long(city.getUnemployed())) + ")");

    guiSystem.at("rightClickMenu").highlight(guiSystem.at("rightClickMenu").getEntry(game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window), guiView)));
}

void GameStateEditor::handleInput() {
    sf::Event event;

    sf::Vector2f guiPos = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window), guiView);
    sf::Vector2f gamePos = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window), gameView);

    while (game->window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
            {
                game->window.close();
                break;
            }
            case sf::Event::Resized:
            {
                gameView.setSize(event.size.width, event.size.height);
                gameView.zoom(zoomLevel);

                guiView.setSize(event.size.width, event.size.height);

                guiSystem.at("infoBar").setDiemnsions(sf::Vector2f(event.size.width / guiSystem.at("infoBar").entries.size(), 16));
                guiSystem.at("infoBar").setPosition(game->window.mapPixelToCoords(sf::Vector2i(0, event.size.height - 16), guiView));
                guiSystem.at("infoBar").show();

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

                    selectionEnd.x = int(pos.y / city.map.tileSize + pos.x / (2 * city.map.tileSize) - city.map.width * 0.5 - 0.5);
                    selectionEnd.y = int(pos.y / city.map.tileSize - pos.x / (2 * city.map.tileSize) + city.map.width * 0.5 + 0.5);

                    city.map.clearSelected();

                    if (currentTile->tileType == TileType::GRASS) {
                        city.map.select(selectionStart, selectionEnd, { currentTile->tileType, TileType::WATER });
                    } else {
                        city.map.select(selectionStart, selectionEnd, {
                            currentTile->tileType, TileType::FOREST,
                            TileType::WATER, TileType::ROAD,
                            TileType::RESIDENTIAL, TileType::COMMERCIAL,
                            TileType::INDUSTRIAL
                        });
                    }

                    guiSystem.at("selectionCostText").setEntryText(0, "$" + std::to_string(currentTile->cost * city.map.numSelected));

                    if (city.funds <= city.map.numSelected * currentTile->cost) {
                        guiSystem.at("selectionCostText").highlight(0);
                    } else {
                        guiSystem.at("selectionCostText").highlight(-1);
                    }

                    guiSystem.at("selectionCostText").setPosition(guiPos + sf::Vector2f(16, -16));
                    guiSystem.at("selectionCostText").show();
                }

                guiSystem.at("rightClickMenu").highlight(guiSystem.at("rightClickMenu").getEntry(guiPos));

                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Middle) {

                    guiSystem.at("rightClickMenu").hide();
                    guiSystem.at("selectionCostText").hide();

                    if (actionState != ActionState::PANNING) {
                        actionState = ActionState::PANNING;
                        panningAnchor = sf::Mouse::getPosition(game->window);
                    }
                } else if (event.mouseButton.button == sf::Mouse::Left) {
                    if (guiSystem.at("rightClickMenu").visible) {
                        std::string msg = guiSystem.at("rightClickMenu").activate(guiPos);

                        if (msg != "null") {
                            currentTile = &game->tileAtlas.at(msg);
                        }

                        guiSystem.at("rightClickMenu").hide();
                    } else {
                        if (actionState != ActionState::SELECTING) {
                            actionState = ActionState::SELECTING;

                            sf::Vector2f pos = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window),
                                                                             gameView);

                            selectionStart.x = int(
                                pos.y / city.map.tileSize + pos.x / (2.0f * city.map.tileSize) - city.map.width * 0.5f -
                                0.5f);
                            selectionStart.y = int(
                                pos.y / city.map.tileSize - pos.x / (2.0f * city.map.tileSize) + city.map.width * 0.5f +
                                0.5f);
                        }
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (actionState == ActionState::SELECTING) {
                        actionState = ActionState::NONE;
                        city.map.clearSelected();
                    } else {
                        sf::Vector2f pos = guiPos;

                        if (pos.x > game->window.getSize().x - guiSystem.at("rightClickMenu").getSize().x) {
                            pos -= sf::Vector2f(guiSystem.at("rightClickMenu").getSize().x, 0);
                        }

                        if (pos.y > game->window.getSize().y - guiSystem.at("rightClickMenu").getSize().y) {
                            pos -= sf::Vector2f(0, guiSystem.at("rightClickMenu").getSize().y);
                        }

                        guiSystem.at("rightClickMenu").setPosition(pos);
                        guiSystem.at("rightClickMenu").show();
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
                        if (currentTile != nullptr) {
                            unsigned int cost = currentTile->cost * city.map.numSelected;
                            if (city.funds >= cost) {
                                city.bulldoze(*currentTile);
                                city.funds -= currentTile->cost * city.map.numSelected;
                                city.tileChanged();
                            }
                        }
                        guiSystem.at("selectionCostText").hide();
                        actionState = ActionState::NONE;
                        city.map.clearSelected();
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

    city = City("city", game->tileSize, game->tileAtlas);
    city.shuffleTiles();

    zoomLevel = 1.0f;

    sf::Vector2f mapCenter(city.map.width, city.map.height * 0.5f);
    mapCenter *= float(city.map.tileSize);

    gameView.setCenter(mapCenter);

    selectionStart = sf::Vector2i(0, 0);
    selectionEnd = sf::Vector2i(0, 0);

    currentTile = &game->tileAtlas.at("grass");

    actionState = ActionState::NONE;

    guiSystem.emplace(std::make_pair("rightClickMenu", Gui(sf::Vector2f(196, 16), 2, false, game->stylesheets.at("button"),
                                            {
                                                std::make_pair("Flatten $" + game->tileAtlas["grass"].getCost(), "grass"),
                                                std::make_pair("Forest $" + game->tileAtlas["forest"].getCost(), "forest"),
                                                std::make_pair("Residential Zone $" + game->tileAtlas["residential"].getCost(), "residential"),
                                                std::make_pair("Commercial Zone $" + game->tileAtlas["commercial"].getCost(), "commercial"),
                                                std::make_pair("Industrial Zone $" + game->tileAtlas["industrial"].getCost(), "industrial"),
                                                std::make_pair("Road $" + game->tileAtlas["road"].getCost(), "road")
                                            })));

    guiSystem.emplace(std::make_pair("selectionCostText", Gui(sf::Vector2f(192, 16), 0, false, game->stylesheets.at("text"), { std::make_pair("", "") })));

    guiSystem.emplace(std::make_pair("infoBar", Gui(sf::Vector2f(game->window.getSize().x / 5, 16), 2, true, game->stylesheets.at("button"),
                                                    {
                                                        std::make_pair("time", "time"),
                                                        std::make_pair("funds", "funds"),
                                                        std::make_pair("population", "population"),
                                                        std::make_pair("employment", "employment"),
                                                        std::make_pair("current tile", "tile")
                                                    })));

    guiSystem.at("infoBar").setPosition(sf::Vector2f(0, game->window.getSize().y - 16));
    guiSystem.at("infoBar").show();
}

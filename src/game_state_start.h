//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_GAME_STATE_START_H
#define SFML_TEST_GAME_STATE_START_H


#include <SFML/Graphics.hpp>

#include <map>
#include <string>

#include "game_state.h"
#include "gui.h"


class GameStateStart : public GameState {

private:
    sf::View view;
    std::map<std::string, Gui> guiSystem;

private:
    void loadGame();

public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

public:
    GameStateStart(Game* game);
};


#endif //SFML_TEST_GAME_STATE_START_H

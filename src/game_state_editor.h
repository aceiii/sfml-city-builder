//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_GAME_STATE_EDITOR_H
#define SFML_TEST_GAME_STATE_EDITOR_H


#include "game_state.h"


class GameStateEditor : public GameState {
private:
    sf::View gameView;
    sf::View guiView;

public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

public:
    GameStateEditor(Game* game);
};


#endif //SFML_TEST_GAME_STATE_EDITOR_H

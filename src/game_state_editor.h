//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_GAME_STATE_EDITOR_H
#define SFML_TEST_GAME_STATE_EDITOR_H


#include "game_state.h"
#include "map.h"


enum class ActionState {
    NONE,
    PANNING,
};


class GameStateEditor : public GameState {
private:
    ActionState actionState;

    sf::View gameView;
    sf::View guiView;

    Map map;

    sf::Vector2i panningAnchor;
    float zoomLevel;

public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

public:
    GameStateEditor(Game* game);
};


#endif //SFML_TEST_GAME_STATE_EDITOR_H

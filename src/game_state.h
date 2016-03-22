//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_GAME_STATE_H
#define SFML_TEST_GAME_STATE_H


class Game;

class GameState {

public:
    Game* game;

    virtual void draw(const float dt) = 0;
    virtual void update(const float dt) = 0;
    virtual void handleInput() = 0;
};


#endif //SFML_TEST_GAME_STATE_H

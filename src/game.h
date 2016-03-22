//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_GAME_H
#define SFML_TEST_GAME_H


#include <stack>
#include <SFML/Graphics.hpp>

#include "texture_manager.h"


class GameState;

class Game {

private:
    void loadTextures();

public:
    std::stack<GameState*> states;

    sf::RenderWindow window;
    sf::Sprite background;

    TextureManager texmgr;

    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* peekState();

    void gameLoop();

public:
    Game();
    ~Game();
};


#endif //SFML_TEST_GAME_H

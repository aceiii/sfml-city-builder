//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_GAME_H
#define SFML_TEST_GAME_H

#include <string>
#include <map>
#include <stack>

#include <SFML/Graphics.hpp>

#include "texture_manager.h"
#include "tile.h"
#include "gui.h"


class GameState;

class Game {

private:
    void initResourceManager();
    void loadTextures();
    void loadTiles();
    void loadStylesheets();
    void loadFonts();

public:
    const static int tileSize = 8;

    std::stack<GameState*> states;

    sf::RenderWindow window;
    sf::Sprite background;

    TextureManager texmgr;

    std::map<std::string, Tile> tileAtlas;
    std::map<std::string, GuiStyle> stylesheets;
    std::map<std::string, sf::Font> fonts;

public:
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

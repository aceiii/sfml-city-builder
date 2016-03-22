

#include "game.h"
#include "game_state_start.h"


int main(int argc, char* argv[]) {

    Game game;

    game.pushState(new GameStateStart(&game));
    game.gameLoop();

    return 0;
}

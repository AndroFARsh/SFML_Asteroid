#include "game/game.h"

int main(int argc, char *argv[]) {
    auto path = (argc >= 2) ? argv[1] : "./res/config.ini";
    Game game(path);

    game.run();

    return 0;
}
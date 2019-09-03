#include <iostream>
#include "gameSystem.h"

using namespace std;

int main()
{
    srand(time(0));
    gameSystem game;
    game.gameLoop();

    return 0;
}

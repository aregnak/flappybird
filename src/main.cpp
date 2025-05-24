#include "game.h"

#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 800
#define ASPECT_RATIO (float)INITIAL_WIDTH / INITIAL_HEIGHT

#define WALL_GAP 400

int main()
{
    Game game;
    game.run();
    return 0;
}

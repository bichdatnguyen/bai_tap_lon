#include<SDL.h>
#include<SDL_image.h>
#include "chess.h"
using namespace std;

int main(int argc, char* argv[])

{
    Chess game;

    game.graphic.init();

    game.gameProcess();

    game.cleanUp();

    return 0;
}

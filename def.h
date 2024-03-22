#ifndef _DEF__H_
#define _DEF__H_
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const char* WINDOW_TITLE = "CHESS";

const int PROMOTE_SCREEN_WIDTH = 400;
const int PROMOTE_SCREEN_HEIGHT = 100;
const char* PROMOTE_WINDOW = "Promote pawn to: ";

#define CELL_SIZE 100
#define BOARD_SIZE 8
#define EMPTY_CELL "   "

#endif

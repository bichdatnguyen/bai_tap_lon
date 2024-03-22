#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>
#include "graphics.h"
#include "promote.h"
#include "logic.h"
using namespace std;

void waitUntilKeyPressed()
{
    SDL_Event e;
    while(true)
    {
        if((SDL_PollEvent(&e) != 0) && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}

int main(int argc, char* argv[])
{
    Graphics graphic;
    graphic.init();

    Chess game;
    game.init();
    graphic.render();

    SDL_Event event;
    cout << "White turn: " << endl;
    bool end = false;
    while(!game.isCheckmate())
    {
        Coordinate piecePos;
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            game.pick(piecePos);
            cout << "Pick piece position: " << piecePos.x << " " << piecePos.y << endl;
            if(!game.legalPick(piecePos))
                break;
            Coordinate nextMove = game.move(piecePos);
            cout << (game.whiteTurn ? "White turn: " : "Black turn: ") << endl;
            string piece = game.board[nextMove.y][nextMove.x];
            graphic.updateMoveSDL(game.board, game.isCheckmate());
            break;
        }
    }
    waitUntilKeyPressed();
    graphic.quitSDL();
    return 0;
}

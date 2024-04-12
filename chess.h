#ifndef _CHESS__H
#define _CHESS__H

#include "logic.h"
#include "graphics.h"

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

struct Chess
{
    Graphics graphic;
    logicGame logic;

    void initGame()
    {
        logic.init();
        graphic.renderInGame();
    }

    void pick(Coordinate &pos)
    {
        logic.choose(pos);
        graphic.playMouseClick();
    }

    char beforeGame()
    {
        graphic.renderMenu();
        bool quit = false;
        while(!quit)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    Coordinate click;
                    pick(click);
                    if(click.x >= 3 && click.x < 6 && click.y == 4) return '1';
                    if(click.x >= 3 && click.x < 6 && click.y == 5) return '2';
                    if(click.x >= 3 && click.x < 6 && click.y == 6) return 'e';
                    break;
                }
            }
        }
    }

    void pauseGame(char &choice, Coordinate piecePos, Coordinate moveTo)
    {
        graphic.renderPauseGame();
        bool quit = false;
        while(!quit)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    Coordinate click;
                    pick(click);
                    if(click.x >= 2 && click.x < 5 && click.y == 4) {
                        graphic.updateMoveSDL(logic, piecePos, moveTo);
                        choice = 'x';
                        quit = true;
                    }
                    if(click.x >= 2 && click.x < 5 && click.y == 5) {
                        choice = 'p';
                        quit = true;
                    }
                    if(click.x >= 2 && click.x < 5 && click.y == 6) {
                        choice = 'e';
                        quit = true;
                    }
                    break;
                }
            }
        }
    }

    char choosePiece()
    {
        graphic.renderChoosePiece();
        bool quit = false;
        while(!quit)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    Coordinate click;
                    pick(click);
                    if(click.x >= 3 && click.x < 6 && click.y == 4) return 'w';
                    if(click.x >= 3 && click.x < 6 && click.y == 5) return 'b';
                    break;
                }
            }
        }
    }

    bool processChoose(Coordinate &piecePos)
    {
        pick(piecePos);
        if(!logic.legalPick(piecePos)) return false;
        return true;
    }

    Coordinate nextMove(Coordinate &piecePos, bool &choosePiece)
    {
        bool quit = false;
        Coordinate moveTo;
        while(!quit)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    logic.choose(moveTo);
                    if(logic.board[moveTo.y][moveTo.x][0] == logic.board[piecePos.y][piecePos.x][0] && logic.legalPick(moveTo)) {
                        piecePos.x = moveTo.x;
                        piecePos.y = moveTo.y;
                        quit = true;
                        break;
                    }
                    if(!logic.legalMove(piecePos, moveTo)) break;
                    choosePiece = true;
                    quit = true;
                    break;
                }
            }
        }
        return moveTo;
    }

    void processMoveTo(Coordinate &piecePos, Coordinate &moveTo)
    {
        bool choosePiece = false;
        while(!choosePiece) {
            graphic.playMouseClick();
            graphic.showLegalMove(logic, piecePos);
            moveTo = nextMove(piecePos, choosePiece);
        }
        graphic.movePiece(logic, moveTo);
    }

    void processSetUp(Coordinate piecePos, Coordinate moveTo)
    {
        logic.setUpForNextMove(piecePos, moveTo);
        logic.updateMove(piecePos, moveTo);
        graphic.updateMoveSDL(logic, piecePos, moveTo);
        logic.updateTurn();
    }

    void prepareForPromote(Coordinate piecePos, Coordinate moveTo, char color)
    {
        graphic.promotePawn(logic, moveTo, color);
        graphic.updateMoveSDL(logic, piecePos, moveTo);
    }

    void gameProcess()
    {
        bool quit = false;
        while(!quit)
        {
            char winner, colorPlayer;
            char choice = beforeGame();
            if(choice == 'e') break;
            if(choice == '1') colorPlayer = choosePiece();
            initGame();
            if(choice == '1') winner = inGameOnePlayer(colorPlayer);
            else winner = inGameTwoPlayer();
            if(winner == 'p') continue;
            if(winner == 'e') break;
            quit = endGame(winner);
        }
    }

    void chooseBotPiecePos(Coordinate &piecePos, char botColor)
    {
        bool haveChoose = false;
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(logic.board[i][j][0] == botColor && logic.canMove({j, i})) {
                    piecePos.x = j;
                    piecePos.y = i;
                    haveChoose = true;
                    break;
                }
            }
            if(haveChoose) break;
        }
    }

    void chooseBotMoveTo(Coordinate piecePos, Coordinate &moveTo)
    {
        bool haveChoose = false;
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(logic.legalMove(piecePos, {j, i})) {
                    moveTo.x = j;
                    moveTo.y = i;
                    haveChoose = true;
                    break;
                }
            }
            if(haveChoose) break;
        }
    }

    void botMove(Coordinate &piecePos, Coordinate &moveTo, char botColor)
    {
        SDL_Delay(500);
        chooseBotPiecePos(piecePos, botColor);
        graphic.showLegalMove(logic, piecePos);
        graphic.playMouseClick();
        SDL_Delay(500);
        chooseBotMoveTo(piecePos, moveTo);
        graphic.movePiece(logic, moveTo);
    }

    void botPromote(char color, Coordinate moveTo)
    {
        if(color == 'w') logic.board[moveTo.y][moveTo.x] = "wQu";
        if(color == 'b') logic.board[moveTo.y][moveTo.x] = "bQu";
    }

    char inGameOnePlayer(char colorPlayer)
    {
        char winner;

        bool gameIsRunning = true;

        while(gameIsRunning) {
            SDL_Event event;
            char color = (logic.whiteTurn ? 'w' : 'b');
            char botColor = (colorPlayer == 'w' ? 'b' : 'w');
            Coordinate piecePos, moveTo;
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    gameIsRunning = false;
                    break;
                }
                if(event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.sym == SDLK_ESCAPE) {
                        char choice;
                        pauseGame(choice, piecePos, moveTo);
                        if(choice == 'p' || choice == 'e') {
                            winner = choice;
                            gameIsRunning = false;
                            break;
                        }
                    }
                }
                if(color == botColor) {
                    botMove(piecePos, moveTo, botColor);
                    processSetUp(piecePos, moveTo);
                    if(logic.pawnGoToBottom(color, moveTo)) {
                        botPromote(color, moveTo);
                        graphic.updateMoveSDL(logic, piecePos, moveTo);
                    }
                }
                if(color == colorPlayer && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if(!processChoose(piecePos)) break;
                    processMoveTo(piecePos, moveTo);
                    processSetUp(piecePos, moveTo);
                    if(logic.pawnGoToBottom(color, moveTo)) {
                        prepareForPromote(piecePos, moveTo, color);
                    }
                }
                if(logic.isCheckmate() || logic.isStalemate()) {
                    winner = prepareForEndGame(gameIsRunning, color);
                    break;
                }
            }
        }
        return winner;
    }

    char inGameTwoPlayer()
    {
        char winner;

        bool gameIsRunning = true;

        while(gameIsRunning) {
            SDL_Event event;
            char color = (logic.whiteTurn ? 'w' : 'b');
            Coordinate piecePos, moveTo;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT) {
                    gameIsRunning = false;
                    break;
                }
                if(event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.sym == SDLK_ESCAPE) {
                        char choice;
                        pauseGame(choice, piecePos, moveTo);
                        if(choice == 'p' || choice == 'e') {
                            winner = choice;
                            gameIsRunning = false;
                            break;
                        }
                    }
                }
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if(!processChoose(piecePos)) break;
                    processMoveTo(piecePos, moveTo);
                    processSetUp(piecePos, moveTo);
                    if(logic.pawnGoToBottom(color, moveTo)) {
                        prepareForPromote(piecePos, moveTo, color);
                    }
                    if(logic.isCheckmate() || logic.isStalemate()) {
                        winner = prepareForEndGame(gameIsRunning, color);
                        break;
                    }
                }
            }
        }
        return winner;
    }

    char prepareForEndGame(bool &gameIsRunning, char color)
    {
        gameIsRunning = false;
        if(logic.isCheckmate()) return color;
        if(logic.isStalemate()) return 'n';
        return 'x';
    }

    bool endGame(char winner)
    {
        if(winner == 'n') graphic.renderStalemate();
        if(winner == 'w' || winner == 'b') graphic.renderCheckmate(winner);

        bool haveChoose = false;

        while(!haveChoose)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT) {
                    haveChoose = true;
                    break;
                }
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    Coordinate option;
                    pick(option);
                    if(option.x >= 3 && option.x < 6 && option.y == 4) return false;
                    if(option.x >= 3 && option.x < 6 && option.y == 5) return true;
                    break;
                }
            }
        }
    }

    void cleanUp()
    {
        graphic.freeChunk();
        graphic.quitSDL();
        waitUntilKeyPressed();
    }
};

#endif

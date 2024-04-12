#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include "logic.h"
#include "def.h"

struct Graphics
{
    SDL_Window *window;
    SDL_Renderer* renderer;
    SDL_Texture *chessBoard, *whitePawn, *whiteRook, *whiteKnight, *whiteBishop, *whiteKing, *whiteQueen,
                *blackPawn, *blackRook, *blackKnight, *blackBishop, *blackKing, *blackQueen;
    Mix_Chunk *click, *slide, *capture, *windowPop;

    void logErrorAndExit(const char* message, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", error);
        SDL_Quit();
    }

    void initSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) logErrorAndExit("Create Window", SDL_GetError());

        if(!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)))
            logErrorAndExit("SDL_image", SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == nullptr) logErrorAndExit("Create Renderer", SDL_GetError());
        if (TTF_Init() == -1) logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ", TTF_GetError());

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
            logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    SDL_Texture* loadTexture(const char* file)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s:", file);

        SDL_Texture *texture = IMG_LoadTexture(renderer, file);
        if(texture == nullptr)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", SDL_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture* texture, int x, int y)
    {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void initTexture()
    {
        chessBoard = loadTexture("images//chessBoard.png");
        whitePawn = loadTexture("images//whitePawn.png");
        whiteRook = loadTexture("images//whiteRook.png");
        whiteKnight = loadTexture("images//whiteKnight.png");
        whiteBishop = loadTexture("images//whiteBishop.png");
        whiteKing = loadTexture("images//whiteKing.png");
        whiteQueen = loadTexture("images//whiteQueen.png");
        blackPawn = loadTexture("images//blackPawn.png");
        blackRook = loadTexture("images//blackRook.png");
        blackKnight = loadTexture("images//blackKnight.png");
        blackBishop = loadTexture("images//blackBishop.png");
        blackKing = loadTexture("images//blackKing.png");
        blackQueen = loadTexture("images//blackQueen.png");
    }

    void initSound()
    {
        click = loadSound("sounds\\mouseClick.wav");
        slide = loadSound("sounds\\pieceSlide.wav");
        capture = loadSound("sounds\\pieceCapture.wav");
        windowPop = loadSound("sounds\\showWindow.wav");
    }

    void init()
    {
        initSDL();
        initTexture();
        initSound();
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    void renderButton(const char* text, int pos_x, int pos_y, int width, int height)
    {
        TTF_Font* fontPlayer = loadFont("assets/font-times-new-roman.ttf", 50);
        SDL_Texture *textButton;

        SDL_Color fontColor = {255, 255, 0, 0};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_Rect button = {pos_x, pos_y, width, height};
        SDL_RenderFillRect(renderer, &button);
        textButton = renderText(text, fontPlayer, fontColor);

        renderTexture(textButton, pos_x, pos_y);
    }

    void renderMenu()
    {
        SDL_RenderClear(renderer);
        SDL_Rect menu = {0, 0, BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE};
        SDL_SetRenderDrawColor(renderer, 160, 160, 0, 0);
        SDL_RenderFillRect(renderer, &menu);

        TTF_Font* fontTitle = loadFont("assets/font-times-new-roman.ttf", 100);
        TTF_Font* fontAuthor = loadFont("assets/font-times-new-roman.ttf", 30);

        SDL_Color fontColor = {255, 255, 0, 0};
        SDL_Texture* titleText = renderText("CHESS GAME", fontTitle, fontColor);
        SDL_Texture* authorText = renderText("Made by: Nguyen Bich Dat", fontAuthor, fontColor);

        renderTexture(titleText, 90, 100);
        renderTexture(authorText, 400, 200);

        renderButton("one player", 300, 400, 300, 99);
        renderButton("two player", 300, 500, 300, 99);
        renderButton("exit", 300, 600, 300, 99);

        presentScene();
    }

    void renderChoosePiece()
    {
        SDL_RenderClear(renderer);
        SDL_Rect menu = {0, 0, BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE};
        SDL_SetRenderDrawColor(renderer, 160, 160, 0, 0);
        SDL_RenderFillRect(renderer, &menu);

        TTF_Font* fontTitle = loadFont("assets/font-times-new-roman.ttf", 50);

        SDL_Color fontColor = {255, 255, 0, 0};
        SDL_Texture* titleText = renderText("CHOOSE PIECE: ", fontTitle, fontColor);

        renderTexture(titleText, 200, 300);

        renderButton("white", 200, 400, 300, 99);
        renderButton("black", 200, 500, 300, 99);

        presentScene();
    }

    void renderInGame()
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, chessBoard, NULL, NULL);

        for(int i=0; i<BOARD_SIZE; i++) {
            renderTexture(blackPawn, CELL_SIZE * i, CELL_SIZE);
            renderTexture(whitePawn, CELL_SIZE * i, CELL_SIZE * 6);
        }
        renderTexture(blackRook, CELL_SIZE * 0, 0);
        renderTexture(whiteRook, CELL_SIZE * 0, CELL_SIZE * 7);
        renderTexture(blackRook, CELL_SIZE * 7, 0);
        renderTexture(whiteRook, CELL_SIZE * 7, CELL_SIZE * 7);
        renderTexture(blackKnight, CELL_SIZE * 1, 0);
        renderTexture(whiteKnight, CELL_SIZE * 1, CELL_SIZE * 7);
        renderTexture(blackKnight, CELL_SIZE * 6, 0);
        renderTexture(whiteKnight, CELL_SIZE * 6, CELL_SIZE * 7);
        renderTexture(blackBishop, CELL_SIZE * 2, 0);
        renderTexture(whiteBishop, CELL_SIZE * 2, CELL_SIZE * 7);
        renderTexture(blackBishop, CELL_SIZE * 5, 0);
        renderTexture(whiteBishop, CELL_SIZE * 5, CELL_SIZE * 7);
        renderTexture(blackQueen, CELL_SIZE * 3, 0);
        renderTexture(whiteQueen, CELL_SIZE * 3, CELL_SIZE * 7);
        renderTexture(blackKing, CELL_SIZE * 4, 0);
        renderTexture(whiteKing, CELL_SIZE * 4, CELL_SIZE * 7);

        presentScene();
    }

    void renderPauseGame()
    {
        SDL_Rect menu = {0, 0, BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE};
        SDL_SetRenderDrawColor(renderer, 160, 160, 0, 0);
        SDL_RenderFillRect(renderer, &menu);

        TTF_Font* fontTitle = loadFont("assets/font-times-new-roman.ttf", 100);

        SDL_Color fontColor = {255, 255, 0, 0};
        SDL_Texture* titleText = renderText("PAUSE GAME", fontTitle, fontColor);

        renderTexture(titleText, 90, 100);

        renderButton("Back to game", 300, 400, 300, 99);
        renderButton("Play again", 300, 500, 300, 99);
        renderButton("exit", 300, 600, 300, 99);

        presentScene();
    }

    TTF_Font* loadFont(const char* path, int size)
    {
        TTF_Font* gFont = TTF_OpenFont( path, size );
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
        }
    }

    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
        }

        SDL_FreeSurface( textSurface );
        return texture;
    }

    void renderPromote(char color)
    {
        SDL_Rect menu = {200, 150, CELL_SIZE * 4, 3 * CELL_SIZE/2};
        SDL_SetRenderDrawColor(renderer, 160, 160, 0, 0);
        SDL_RenderFillRect(renderer, &menu);

        TTF_Font* font = loadFont("assets/font-times-new-roman.ttf", 42);

        SDL_Color fontColor = {255, 255, 0, 0};
        SDL_Texture* promoteText = renderText("Promote: ", font, fontColor);

        renderTexture(promoteText, 200, 150);

        if(color == 'w') {
            renderTexture(whiteRook, 200, 200);
            renderTexture(whiteKnight, 300, 200);
            renderTexture(whiteBishop, 400, 200);
            renderTexture(whiteQueen, 500, 200);
        }
        else if(color == 'b')
        {
            renderTexture(blackRook, 200, 200);
            renderTexture(blackKnight, 300, 200);
            renderTexture(blackBishop, 400, 200);
            renderTexture(blackQueen, 500, 200);
        }

        play(windowPop);
        SDL_Delay(200);
        presentScene();
    }

    void renderCheckmate(char whoWin)
    {
        SDL_Rect endGame = {200, 100, 500, 600};
        SDL_SetRenderDrawColor(renderer, 160, 160, 0, 0);
        SDL_RenderFillRect(renderer, &endGame);

        TTF_Font* font = loadFont("assets/font-times-new-roman.ttf", 45);
        SDL_Color fontColor = {255, 255, 0, 0};

        SDL_Texture* winningText;

        if(whoWin == 'w') winningText = renderText("WHITE WIN", font, fontColor);
        if(whoWin == 'b') winningText = renderText("BLACK WIN", font, fontColor);
        renderTexture(winningText, 300, 200);

        renderButton("play again", 300, 400, 300, 99);
        renderButton("exit", 300, 500, 300, 100);

        play(windowPop);
        SDL_Delay(200);
        presentScene();
    }

    void renderStalemate()
    {

        SDL_Rect endGame = {200, 100, 400, 600};
        SDL_SetRenderDrawColor(renderer, 160, 160, 0, 0);
        SDL_RenderFillRect(renderer, &endGame);

        TTF_Font* font = loadFont("assets/font-times-new-roman.ttf", 45);
        SDL_Color fontColor = {255, 255, 0, 0};

        SDL_Texture* stalemateText = renderText("STALEMATE", font, fontColor);

        renderTexture(stalemateText, 300, 200);

        renderButton("play again", 300, 400, 300, 99);
        renderButton("exit", 300, 500, 300, 100);

        play(windowPop);
        SDL_Delay(200);
        presentScene();
    }

    void promotePawn(logicGame& logic, Coordinate moveTo, char color)
    {
        renderPromote(color);
        Coordinate choosePromote;
        bool quit = false;
        while(!quit)
        {
            SDL_Event e;
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    logic.choose(choosePromote);
                    if(choosePromote.x == 2 && choosePromote.y == 2) {
                        if(color == 'w') logic.board[moveTo.y][moveTo.x] = "wRo";
                        else logic.board[moveTo.y][moveTo.x] = "bRo";
                    }
                    else if(choosePromote.x == 3 && choosePromote.y == 2) {
                        if(color == 'w') logic.board[moveTo.y][moveTo.x] = "wKn";
                        else logic.board[moveTo.y][moveTo.x] = "bKn";
                    }
                    else if(choosePromote.x == 4 && choosePromote.y == 2) {
                        if(color == 'w') logic.board[moveTo.y][moveTo.x] = "wBi";
                        else logic.board[moveTo.y][moveTo.x] = "bBi";
                    }
                    else if(choosePromote.x == 5 && choosePromote.y == 2) {
                        if(color == 'w') logic.board[moveTo.y][moveTo.x] = "wQu";
                        else logic.board[moveTo.y][moveTo.x] = "bQu";
                    }
                    else break;
                    quit = true;
                    break;
                }
            }
        }
        play(click);
    }

    void renderCheckSDL(Coordinate pos)
    {
        SDL_Rect rect = {pos.x * CELL_SIZE + 1, pos.y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderFillRect(renderer, &rect);
    }

    void isCheckSDL(logicGame logic)
    {
        Coordinate posWhite, posBlack;
        posWhite = logic.kingPosition('w');
        posBlack = logic.kingPosition('b');
        if(logic.isCheck(posWhite, 'w')) {
            renderCheckSDL(posWhite);
            renderTexture(whiteKing, posWhite.x * CELL_SIZE, posWhite.y * CELL_SIZE);
        }
        if(logic.isCheck(posBlack, 'b')) {
            renderCheckSDL(posBlack);
            renderTexture(blackKing, posWhite.x * CELL_SIZE, posWhite.y * CELL_SIZE);
        }
    }

    void renderCanMoveSquare(Coordinate pos)
    {
        SDL_Rect rect = {pos.x * CELL_SIZE + 1, pos.y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2};
        SDL_SetRenderDrawColor(renderer, 200, 255, 0, 0);
        SDL_RenderFillRect(renderer, &rect);
    }

    void renderPiece(logicGame logic, int i, int j)
    {
        if(logic.board[j][i] == "bPa") renderTexture(blackPawn, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "wPa") renderTexture(whitePawn, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "bRo") renderTexture(blackRook, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "wRo") renderTexture(whiteRook, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "bKn") renderTexture(blackKnight, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "wKn") renderTexture(whiteKnight, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "bBi") renderTexture(blackBishop, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "wBi") renderTexture(whiteBishop, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "bQu") renderTexture(blackQueen, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "wQu") renderTexture(whiteQueen, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "bKi") renderTexture(blackKing, i * CELL_SIZE, j * CELL_SIZE);
        else if(logic.board[j][i] == "wKi") renderTexture(whiteKing, i * CELL_SIZE, j * CELL_SIZE);
        else return;
    }

    void renderBoard(logicGame logic)
    {
        SDL_RenderCopy(renderer, chessBoard, NULL, NULL);

        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                renderPiece(logic, i, j);
            }
        }
    }

    void showLegalMove(logicGame logic, Coordinate pos)
    {
        renderBoard(logic);

        renderCanMoveSquare(pos);
        renderPiece(logic, pos.x, pos.y);
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(logic.legalMove(pos, {i, j})) {
                    renderCanMoveSquare({i, j});
                    renderPiece(logic, i, j);
                }
                else continue;
            }
        }
        presentScene();
    }

    void updateMoveSDL(logicGame& logic, Coordinate piecePos, Coordinate moveTo)
    {
        SDL_RenderCopy(renderer, chessBoard, NULL, NULL);

        renderCanMoveSquare(piecePos);
        isCheckSDL(logic);
        renderPiece(logic, piecePos.x, piecePos.y);
        renderCanMoveSquare(moveTo);

        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                renderPiece(logic, i, j);
            }
        }
        presentScene();
    }

    Mix_Chunk* loadSound(const char* path) {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
    }

    void play(Mix_Chunk* gChunk) {
        if(gChunk != nullptr) {
            Mix_PlayChannel(-1, gChunk, 0);
        }
    }

    void playMouseClick()
    {
        play(click);
    }

    void movePiece(logicGame logic, Coordinate moveTo)
    {
        if(logic.board[moveTo.y][moveTo.x] == EMPTY_CELL) play(slide);
        if(logic.board[moveTo.y][moveTo.x] != EMPTY_CELL) play(capture);
    }

    void freeChunk()
    {
        if(click != nullptr) Mix_FreeChunk(click);
        if(slide != nullptr) Mix_FreeChunk(slide);
        if(capture != nullptr) Mix_FreeChunk(capture);
        if(windowPop != nullptr) Mix_FreeChunk(windowPop);
    }

    void quitSDL()
    {
        SDL_DestroyTexture(chessBoard);
        SDL_DestroyTexture(whitePawn);
        SDL_DestroyTexture(whiteRook);
        SDL_DestroyTexture(whiteKnight);
        SDL_DestroyTexture(whiteBishop);
        SDL_DestroyTexture(whiteKing);
        SDL_DestroyTexture(whiteQueen);
        SDL_DestroyTexture(blackPawn);
        SDL_DestroyTexture(blackRook);
        SDL_DestroyTexture(blackKnight);
        SDL_DestroyTexture(blackBishop);
        SDL_DestroyTexture(blackKing);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
};

#endif

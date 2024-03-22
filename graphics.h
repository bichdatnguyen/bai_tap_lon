#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include "def.h"

struct Graphics
{
    SDL_Window *window;
    SDL_Renderer* renderer;
    SDL_Texture *chessBoard, *whitePawn, *whiteRook, *whiteKnight, *whiteBishop, *whiteKing, *whiteQueen,
                *blackPawn, *blackRook, *blackKnight, *blackBishop, *blackKing, *blackQueen, *gameOver;

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
        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if(!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)))
            logErrorAndExit("SDL_image", SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
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

    void init()
    {
        initSDL();
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

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    void render()
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

    void updateMoveSDL(string board[][BOARD_SIZE], bool end)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, chessBoard, NULL, NULL);
        if(end) {
            gameOver = loadTexture("images//gameOver.png");
            SDL_RenderCopy(renderer, gameOver, NULL, NULL);
        }
        else {
            for(int i=0; i<BOARD_SIZE; i++) {
                for(int j=0; j<BOARD_SIZE; j++) {
                    if(board[i][j] == "wPa") renderTexture(whitePawn, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "bPa") renderTexture(blackPawn, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "wRo") renderTexture(whiteRook, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "bRo") renderTexture(blackRook, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "wKn") renderTexture(whiteKnight, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "bKn") renderTexture(blackKnight, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "wBi") renderTexture(whiteBishop, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "bBi") renderTexture(blackBishop, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "wQu") renderTexture(whiteQueen, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "bQu") renderTexture(blackQueen, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "wKi") renderTexture(whiteKing, j * CELL_SIZE, i * CELL_SIZE);
                    if(board[i][j] == "bKi") renderTexture(blackKing, j * CELL_SIZE, i * CELL_SIZE);
                }
            }
        }

        presentScene();
    }

    void quitSDL()
    {
        chessBoard = nullptr;
        whitePawn = nullptr;
        whiteRook = nullptr;
        whiteKnight = nullptr;
        whiteBishop = nullptr;
        whiteKing = nullptr;
        whiteQueen = nullptr;
        blackPawn = nullptr;
        blackRook = nullptr;
        blackKnight = nullptr;
        blackBishop = nullptr;
        blackKing = nullptr;
        gameOver = nullptr;
        IMG_Quit();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }
};

#endif

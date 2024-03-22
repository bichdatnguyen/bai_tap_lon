#ifndef _PROMOTE__H
#define _PROMOTE__H
#include "def.h"

struct Promote
{
    SDL_Window *window;
    SDL_Renderer* renderer;
    SDL_Texture *whiteRook, *whiteKnight, *whiteBishop, *whiteQueen, *blackRook, *blackKnight, *blackBishop, *blackQueen;

    void logErrorAndExit(const char* message, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", error);
        SDL_Quit();
    }

    void initSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(PROMOTE_WINDOW, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, PROMOTE_SCREEN_WIDTH, PROMOTE_SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
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
        cerr << dest.w << " " << dest.h << endl;
    }

    void init()
    {
        initSDL();
        whiteRook = loadTexture("images//whiteRook.png");
        whiteKnight = loadTexture("images//whiteKnight.png");
        whiteBishop = loadTexture("images//whiteBishop.png");
        whiteQueen = loadTexture("images//whiteQueen.png");
        blackRook = loadTexture("images//blackRook.png");
        blackKnight = loadTexture("images//blackKnight.png");
        blackBishop = loadTexture("images//blackBishop.png");
        blackQueen = loadTexture("images//blackQueen.png");
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    void render(char color)
    {
        SDL_RenderClear(renderer);

        if(color == 'w') {
            renderTexture(whiteRook, CELL_SIZE * 0, 0);
            renderTexture(whiteKnight, CELL_SIZE * 1, 0);
            renderTexture(whiteBishop, CELL_SIZE * 2, 0);
            renderTexture(whiteQueen, CELL_SIZE * 3, 0);
        }
        else if(color == 'b')
        {
            renderTexture(blackRook, CELL_SIZE * 0, 0);
            renderTexture(blackKnight, CELL_SIZE * 1, 0);
            renderTexture(blackBishop, CELL_SIZE * 2, 0);
            renderTexture(blackQueen, CELL_SIZE * 3, 0);
        }
        presentScene();
    }

    void quitSDL()
    {
        whiteRook = nullptr;
        whiteKnight = nullptr;
        whiteBishop = nullptr;
        whiteQueen = nullptr;
        blackRook = nullptr;
        blackKnight = nullptr;
        blackBishop = nullptr;
        IMG_Quit();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }
};

#endif

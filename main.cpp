#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_Main.h>
#include <main.h>

using namespace std;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;

bool init() {
    //setup
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    IMG_Init(IMG_INIT_JPG || IMG_INIT_PNG);
    TTF_Init();
    gWindow = SDL_CreateWindow("Sliding Puzzle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    return true;
}

void close() {
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    init();

    bool quit = false;
    SDL_Event e;

    MainScreen::start(gRenderer);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN)
                if( e.button.button == SDL_BUTTON_LEFT ){
                    int x = e.button.x;
                    int y = e.button.y;
                    //New Game
                    if (259 <= x && x <= 479 && 284 <= y && y <= 356) {
                        OptionScreen::start(gRenderer, quit);
                        MainScreen::start(gRenderer);
                    }
                    //Introduction
                    if (259 <= x && x <= 479 && 419 <= y && y <= 490) {
                        InstructionScreen::start(gRenderer, quit);
                        MainScreen::start(gRenderer);
                    }
                    //High score
                    if (259 <= x && x <= 479 && 563 <= y && y <= 635) {
                        HighscoreScreen::start(gRenderer, quit);
                        MainScreen::start(gRenderer);
                    }
                }
        }
    }

    close();
    return 0;
}

#include <main.h>
#include <iostream>

void MainScreen::start(SDL_Renderer* gRenderer, bool& quit) {
    bool goBack = false;

    //clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(gRenderer);

    //load background
    SDL_Surface* surface = IMG_Load("Picture/background.png");
    SDL_Texture* background = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy( gRenderer, background, NULL, NULL );

    SDL_RenderPresent( gRenderer );

    SDL_Event e;
    while (!quit && !goBack) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN)
                if( e.button.button == SDL_BUTTON_LEFT ){
                    int x = e.button.x;
                    int y = e.button.y;
                    //New Game
                    if (259 <= x && x <= 479 && 284 <= y && y <= 356) {
                        OptionScreen::start(gRenderer, quit);
                        goBack = true;
                    }
                    //Introduction
                    if (259 <= x && x <= 479 && 419 <= y && y <= 490) {
                        InstructionScreen::start(gRenderer, quit);
                        goBack = true;
                    }
                    //High score
                    if (259 <= x && x <= 479 && 563 <= y && y <= 635) {
                        HighscoreScreen::start(gRenderer, quit);
                        goBack = true;
                    }
                }
        }
    }

    SDL_DestroyTexture(background);
}

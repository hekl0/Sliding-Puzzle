#include <main.h>
#include <iostream>

SDL_Texture* backgroundMainScreenContinue = NULL;

void MainScreenContinue::start(SDL_Renderer* gRenderer, bool& quit) {
    bool goBack = false;

    //clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(gRenderer);

    //load background
    if (backgroundMainScreenContinue == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/backgroundContinue.png");
        backgroundMainScreenContinue = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }
    SDL_RenderCopy( gRenderer, backgroundMainScreenContinue, NULL, NULL );
    SDL_RenderPresent( gRenderer );

    SDL_Event e;
    while (!quit && !goBack) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN)
                if( e.button.button == SDL_BUTTON_LEFT ){
                    int x = e.button.x;
                    int y = e.button.y;
                    //continue
                    if (248 <= x && x <= 470 && 247 <= y && y <= 316) {
                        GameScreen::start(gRenderer, quit, 0, "", true);
                        goBack = true;
                    }
                    //New Game
                    if (248 <= x && x <= 470 && 371 <= y && y <= 437) {
                        OptionScreen::start(gRenderer, quit);
                        goBack = true;
                    }
                    //Introduction
                    if (248 <= x && x <= 470 && 490 <= y && y <= 560) {
                        InstructionScreen::start(gRenderer, quit);
                        goBack = true;
                    }
                    //High score
                    if (248 <= x && x <= 470 && 605 <= y && y <= 680) {
                        HighscoreScreen::start(gRenderer, quit);
                        goBack = true;
                    }
                }
        }
    }
}


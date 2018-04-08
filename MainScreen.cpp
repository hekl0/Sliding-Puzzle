#include <main.h>
#include <iostream>

SDL_Texture* backgroundMainScreen = NULL;

void MainScreen::start(SDL_Renderer* gRenderer) {
    //clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(gRenderer);

    //load background
    if (backgroundMainScreen == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/background.png");
        backgroundMainScreen = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }
    SDL_RenderCopy( gRenderer, backgroundMainScreen, NULL, NULL );
    SDL_RenderPresent( gRenderer );
}

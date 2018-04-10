#include <main.h>

SDL_Texture* gTexture = NULL;

void OutroScreen::loseScreen(SDL_Renderer* gRenderer, bool& quit) {
    //draw window
    SDL_Surface* surface = IMG_Load("Picture/LoseScreen.png");
    gTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect;
    for (int i = 1; i <= 350; i++) {
        dstRect.x = 350 - i;
        dstRect.y = 350 - i;
        dstRect.w = i * 2;
        dstRect.h = i * 2;
        SDL_RenderCopy(gRenderer, gTexture, NULL, &dstRect);

        SDL_RenderPresent(gRenderer);
    }

    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0)
            if (e.type == SDL_QUIT) quit = true;

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if (currentKeyStates[SDL_SCANCODE_RETURN]) break;
    }

    SDL_DestroyTexture(gTexture);
}

void OutroScreen::winScreen(SDL_Renderer* gRenderer, bool& quit, int mode, int score) {
    //draw window
    SDL_Surface* surface = IMG_Load("Picture/WinScreen.png");
    gTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dstRect;
    for (int i = 1; i <= 350; i++) {
        dstRect.x = 350 - i;
        dstRect.y = 350 - i;
        dstRect.w = i * 2;
        dstRect.h = i * 2;
        SDL_RenderCopy(gRenderer, gTexture, NULL, &dstRect);

        SDL_RenderPresent(gRenderer);
    }

    //update screen
    SDL_RenderPresent(gRenderer);

    SDL_Event e;
    string inputText = "";
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) inputText.pop_back();
                drawName(gRenderer, inputText);
            }
            if (e.type == SDL_TEXTINPUT && inputText.length() < 10) {
                inputText += e.text.text;
                drawName(gRenderer, inputText);
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if (currentKeyStates[SDL_SCANCODE_RETURN]) break;
    }

    DataManager::addItem(mode, inputText, score);

    SDL_DestroyTexture(gTexture);
}

void OutroScreen::drawName(SDL_Renderer* gRenderer, string text) {
    if (text == "") text = " ";

    TTF_Font* gFontOutroScreen = TTF_OpenFont( "Font/score.ttf", 30 );
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFontOutroScreen, text.c_str(), {255,0,0,255} );
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );

    SDL_Rect dstRect;
    dstRect.w = textSurface->w;
    dstRect.h = textSurface->h;
    dstRect.x = 355 - dstRect.w / 2;
    dstRect.y = 390;

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
    SDL_RenderCopy(gRenderer, textTexture, NULL, &dstRect);

    SDL_RenderPresent(gRenderer);

    SDL_DestroyTexture(textTexture);
}

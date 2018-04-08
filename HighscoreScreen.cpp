#include <main.h>

SDL_Texture* backgroundHighscoreScreen = NULL;
SDL_Texture* easyChoosed = NULL;
SDL_Texture* mediumChoosed = NULL;
SDL_Texture* hardChoosed = NULL;

TTF_Font* gFontHighscoreScreen = NULL;

void HighscoreScreen::start(SDL_Renderer* gRenderer, bool& quit) {
    bool backPressed = false;
    int mode = 0;

    loadBackground(gRenderer, mode);

    SDL_Event e;
    while (!quit && !backPressed) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN)
                if( e.button.button == SDL_BUTTON_LEFT ){
                    int x = e.button.x;
                    int y = e.button.y;
                    //New Game
                    if (75 <= x && x <= 125 && 75 <= y && y <= 110) backPressed = true;

                    if (83 <= x && x <= 190 && 164 <= y && y <= 203 && mode != 0) {
                        mode = 0;
                        loadBackground(gRenderer, mode);
                    }
                    if (224 <= x && x <= 433 && 164 <= y && y <= 203 && mode != 1) {
                        mode = 1;
                        loadBackground(gRenderer, mode);
                    }
                    if (487 <= x && x <= 617 && 164 <= y && y <= 203 && mode != 2) {
                        mode = 2;
                        loadBackground(gRenderer, mode);
                    }
                }
        }
    }
}

void HighscoreScreen::loadBackground(SDL_Renderer* gRenderer, int mode) {
    //clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(gRenderer);

    //load background
    if (backgroundHighscoreScreen == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/HighscoreBackground.png");
        backgroundHighscoreScreen = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }
    SDL_RenderCopy(gRenderer, backgroundHighscoreScreen, NULL, NULL);

    if (mode == 0) easyOption(gRenderer, mode);
    if (mode == 1) mediumOption(gRenderer, mode);
    if (mode == 2) hardOption(gRenderer, mode);

    loadHighscore(gRenderer, mode);

    SDL_RenderPresent(gRenderer);
}

void HighscoreScreen::loadHighscore(SDL_Renderer* gRenderer, int mode) {
    //Sleep(100);
    vector< pair<string, int> > data = DataManager::getAllItem(mode);

    //215 305
    if (data.size() >= 1) {
        drawText(gRenderer, data[0].first, 215, 305);
        string text = "";
        text = text + to_string(data[0].second) + "s";
        drawText(gRenderer, text, 555, 305);
    }
    else drawText(gRenderer, "~~~~~~~~~~~~~~~~~~", 215, 305);

    if (data.size() >= 2) {
        drawText(gRenderer, data[1].first, 215, 445);
        string text = "";
        text = text + to_string(data[1].second) + "s";
        drawText(gRenderer, text, 555, 445);
    }
    else drawText(gRenderer, "~~~~~~~~~~~~~~~~~~", 215, 445);

    if (data.size() >= 3) {
        drawText(gRenderer, data[2].first, 215, 575);
        string text = "";
        text = text + to_string(data[2].second) + "s";
        drawText(gRenderer, text, 555, 575);
    }
    else drawText(gRenderer, "~~~~~~~~~~~~~~~~~~", 215, 575);
}

void HighscoreScreen::drawText(SDL_Renderer* gRenderer, string text, int x, int y) {
    //cout << text << " " << x << " " << y << endl;
    if (gFontHighscoreScreen == NULL) gFontHighscoreScreen = TTF_OpenFont( "brush.ttf", 40 );
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFontHighscoreScreen, text.c_str(), {255,255,255,255} );
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
    //SDL_FreeSurface(textSurface);

    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = textSurface->w;
    dstRect.h = textSurface->h;

    SDL_RenderCopy(gRenderer, textTexture, NULL, &dstRect);
}

void HighscoreScreen::easyOption(SDL_Renderer* gRenderer, int mode) {
    if (easyChoosed == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/EasyChoosed.png");
        easyChoosed = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect dstRect;
    dstRect.x = 69;
    dstRect.y = 153;
    dstRect.w = 142;
    dstRect.h = 66;

    SDL_RenderCopy(gRenderer, easyChoosed, NULL, &dstRect);
}

void HighscoreScreen::mediumOption(SDL_Renderer* gRenderer, int mode) {
    if (mediumChoosed == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/MediumChoosed.png");
        mediumChoosed = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect dstRect;
    dstRect.x = 226;
    dstRect.y = 153;
    dstRect.w = 220;
    dstRect.h = 64;

    SDL_RenderCopy(gRenderer, mediumChoosed, NULL, &dstRect);
}

void HighscoreScreen::hardOption(SDL_Renderer* gRenderer, int mode) {
    if (hardChoosed == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/HardChoosed.png");
        hardChoosed = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect dstRect;
    dstRect.x = 468;
    dstRect.y = 154;
    dstRect.w = 164;
    dstRect.h = 63;

    SDL_RenderCopy(gRenderer, hardChoosed, NULL, &dstRect);
}

#include <main.h>

void OptionScreen::start(SDL_Renderer* gRenderer, bool& quit) {
    bool nextScreen = false;
    int mode = 0;
    string src = "Picture/default.jpg";

    loadBackground(gRenderer, mode, src);

    SDL_Event e;
    while (!quit && !nextScreen) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN)
                if( e.button.button == SDL_BUTTON_LEFT ){
                    int x = e.button.x;
                    int y = e.button.y;
                    //left
                    if (173 <= x && x <= 248 && 587 <= y && y <= 629) {
                            mode = (mode - 1 + 3) % 3;
                            loadBackground(gRenderer, mode, src);
                    }
                    //right
                    if (464 <= x && x <= 540 && 587 <= y && y <= 629) {
                            mode = (mode + 1) % 3;
                            loadBackground(gRenderer, mode, src);
                    }
                    //image source 137 55 575 518
                    if (137 <= x && x <= 575 && 55 <= y && y <= 518) {
                            changeImageSource(src);
                            loadBackground(gRenderer, mode, src);
                    }
                }
        }
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if (currentKeyStates[SDL_SCANCODE_RETURN]) nextScreen = true;
    }

    if (nextScreen) GameScreen::start(gRenderer, quit, mode+2, src, false);
}

void OptionScreen::changeImageSource(string& src) {
    bool quit = false;
    bool choosed = false;
    std::string textInput = "";

    SDL_Window* subWindow  = SDL_CreateWindow( "Changing image" , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 570, 169, SDL_WINDOW_SHOWN);
    SDL_Renderer* subRenderer  = SDL_CreateRenderer( subWindow, -1, SDL_RENDERER_ACCELERATED );
    SDL_SetWindowIcon(subWindow, IMG_Load("Picture/icon.png"));
    int id = SDL_GetWindowID(subWindow);

    drawSubScreen(subRenderer, textInput);

    SDL_Event e;
    SDL_StartTextInput();
    while (!quit && !choosed) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == id) quit = true;
            if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST && e.window.windowID == id) SDL_RaiseWindow(subWindow);

            if (e.type == SDL_MOUSEBUTTONDOWN)
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int x = e.button.x;
                    int y = e.button.y;

                    if (485 <= x && x <= 526 && 88 <= y && y <= 136) choosed = true;
                }

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && textInput.length() > 0) textInput.pop_back();
                if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) textInput = SDL_GetClipboardText();
                drawSubScreen(subRenderer, textInput);
            }
            if (e.type == SDL_TEXTINPUT)
                if (!(( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C')
                && (e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V') && SDL_GetModState() & KMOD_CTRL)) {
                    textInput += e.text.text;
                    drawSubScreen(subRenderer, textInput);
                    cout << textInput << endl;
                }
        }
    }
    SDL_StopTextInput();

    if (textInput != "") {
        for (int i = 0; i < textInput.length(); i++)
            if (textInput[i] == '\\')
                textInput[i] = '/';
        if (IMG_Load(textInput.c_str()) != NULL) src = textInput;
    }

    SDL_DestroyRenderer( subRenderer );
    SDL_DestroyWindow( subWindow );
    subWindow = NULL;
    subRenderer = NULL;
}

void OptionScreen::drawSubScreen(SDL_Renderer* subRenderer, string textInput) {
    SDL_SetRenderDrawColor( subRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( subRenderer );

    //load background
    SDL_Surface* surface = IMG_Load("Picture/ImageSource.png");
    SDL_Texture* background = SDL_CreateTextureFromSurface(subRenderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(subRenderer, background, NULL, NULL);
    SDL_DestroyTexture(background);

    //make text
    if (textInput == "") textInput = " ";
    TTF_Font* gFontOptionScreen = TTF_OpenFont( "Font/score.ttf", 30 );
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFontOptionScreen, textInput.c_str(), {255,255,255,255} );
    SDL_Texture* text = SDL_CreateTextureFromSurface( subRenderer, textSurface );

    int mWidth = textSurface->w;
    int mHeight = textSurface->h;
    SDL_FreeSurface( textSurface );
    int blockWidth = min(380, mWidth);

    SDL_Rect srcRect;
    srcRect.x = mWidth - blockWidth;
    srcRect.y = 0;
    srcRect.w = blockWidth;
    srcRect.h = 33;

    SDL_Rect dstRect;
    dstRect.x = 70;
    dstRect.y = 90;
    dstRect.w = blockWidth;
    dstRect.h = 33;

    SDL_RenderCopy(subRenderer, text, &srcRect, &dstRect);
    SDL_DestroyTexture(text);

    SDL_RenderPresent( subRenderer );
}

void OptionScreen::loadBackground(SDL_Renderer* gRenderer, int mode, string src) {
    //clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(gRenderer);

    //load background
    SDL_Surface* surface = IMG_Load("Picture/OptionBackground.png");
    SDL_Texture* background = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy( gRenderer, background, NULL, NULL );
    SDL_DestroyTexture(background);

    loadImage(gRenderer, src);

    if (mode == 0) EasyOption(gRenderer);
    if (mode == 1) MediumOption(gRenderer);
    if (mode == 2) HardOption(gRenderer);

    SDL_RenderPresent( gRenderer );
}

void OptionScreen::loadImage(SDL_Renderer* gRenderer, string src) {
    SDL_Surface* surface = IMG_Load(src.c_str());
    SDL_Texture* background = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dsrect;
    dsrect.x = 170;
    dsrect.y = 85;
    dsrect.w = 375;
    dsrect.h = 405;

    SDL_RenderCopy( gRenderer, background, NULL, &dsrect);
    SDL_DestroyTexture(background);

    surface = IMG_Load("Picture/border.png");
    SDL_Texture* border = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    dsrect.x = 167;
    dsrect.y = 79;
    dsrect.w = 382;
    dsrect.h = 417;

    SDL_RenderCopy( gRenderer, border, NULL, &dsrect);
    SDL_DestroyTexture(border);
}

void OptionScreen::EasyOption(SDL_Renderer* gRenderer) {
    SDL_Surface* surface = IMG_Load("Picture/EasyMode.png");
    SDL_Texture* easyMode = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dsrect;
    dsrect.x = 285;
    dsrect.y = 560;
    dsrect.w = 154;
    dsrect.h = 107;

    SDL_RenderCopy( gRenderer, easyMode, NULL, &dsrect);
    SDL_DestroyTexture(easyMode);
}

void OptionScreen::MediumOption(SDL_Renderer* gRenderer) {
    SDL_Surface* surface = IMG_Load("Picture/MediumMode.png");
    SDL_Texture* mediumMode = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dsrect;
    dsrect.x = 270;
    dsrect.y = 590;
    dsrect.w = 176;
    dsrect.h = 35;

    SDL_RenderCopy( gRenderer, mediumMode, NULL, &dsrect);
    SDL_DestroyTexture(mediumMode);
}

void OptionScreen::HardOption(SDL_Renderer* gRenderer) {
    SDL_Surface* surface = IMG_Load("Picture/HardMode.png");
    SDL_Texture* hardMode = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dsrect;
    dsrect.x = 305;
    dsrect.y = 590;
    dsrect.w = 107;
    dsrect.h = 34;

    SDL_RenderCopy( gRenderer, hardMode, NULL, &dsrect);
    SDL_DestroyTexture(hardMode);
}

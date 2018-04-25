#include <main.h>
#include <time.h>

GAME gameValue;
int imageWidth;
int imageHeight;

SDL_Texture* imageTexture = NULL;
SDL_Texture* backgroundGameScreen = NULL;
SDL_Texture* borderGameScreen = NULL;

bool isPreviewing = false;

//2x + 5 = 3y + 10 = 4z + 15 => square size 475
void GameScreen::start(SDL_Renderer* gRenderer, bool& quit, int mode, string src, bool isContinuing) {
    bool giveUp = false;
    bool backPressed = false;

    //generate game data
    gameValue.width = mode;
    gameValue.height = mode;
    gameValue.score = 0;
    gameValue.isSetStartTime = false;
    gameValue.pos0 = {mode-1, mode-1};
    for (int i = 0; i < mode; i++)
        for (int j = 0; j < mode; j++)
            gameValue.table[i][j] = i*mode + j + 1;
    gameValue.table[mode-1][mode-1] = 0;

    shuffleGame(gameValue, mode);

    //set up data
    loadData(gRenderer, src, mode, isContinuing);

    loadBackground(gRenderer, mode);

    SDL_Event e;
    while (!quit && !backPressed) {
        if (checkFinished(mode)) break;
        showScore(gRenderer);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN)
                if( e.button.button == SDL_BUTTON_LEFT ) {
                    //give up
                    if (isPreviewing) continue;

                    int x = e.button.x;
                    int y = e.button.y;
                    //back
                    if (0 <= x && x <= 80 && 0 <= y && y <= 80) backPressed = true;
                    //give up
                    if (370 <= x && x <= 600 && 40 <= y && y <= 100) {
                        giveUp = true;
                        string solution = SolvingPuzzle::solvePuzzle(gameValue);
                        for (int i = 0; i < solution.size(); i++) {
                            SDL_PollEvent(&e);
                            if (e.type == SDL_QUIT) {
                                quit = true;
                                break;
                            }
                            if (solution[i] == 'U')
                                swapPiece(gRenderer, gameValue, mode, gameValue.pos0.X+1, gameValue.pos0.Y, gameValue.pos0.X, gameValue.pos0.Y);
                            if (solution[i] == 'D')
                                swapPiece(gRenderer, gameValue, mode, gameValue.pos0.X-1, gameValue.pos0.Y, gameValue.pos0.X, gameValue.pos0.Y);
                            if (solution[i] == 'L')
                                swapPiece(gRenderer, gameValue, mode, gameValue.pos0.X, gameValue.pos0.Y+1, gameValue.pos0.X, gameValue.pos0.Y);
                            if (solution[i] == 'R')
                                swapPiece(gRenderer, gameValue, mode, gameValue.pos0.X, gameValue.pos0.Y-1, gameValue.pos0.X, gameValue.pos0.Y);
                        }
                    }
                }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if (currentKeyStates[SDL_SCANCODE_UP])
            swapPiece(gRenderer, gameValue, mode, gameValue.pos0.X+1, gameValue.pos0.Y, gameValue.pos0.X, gameValue.pos0.Y);
        if (currentKeyStates[SDL_SCANCODE_DOWN])
            swapPiece(gRenderer, gameValue, mode, gameValue.pos0.X-1, gameValue.pos0.Y, gameValue.pos0.X, gameValue.pos0.Y);
        if (currentKeyStates[SDL_SCANCODE_LEFT])
            swapPiece(gRenderer, gameValue, mode, gameValue.pos0.X, gameValue.pos0.Y+1, gameValue.pos0.X, gameValue.pos0.Y);
        if (currentKeyStates[SDL_SCANCODE_RIGHT])
            swapPiece(gRenderer, gameValue, mode, gameValue.pos0.X, gameValue.pos0.Y-1, gameValue.pos0.X, gameValue.pos0.Y);

        //preview image
        if (currentKeyStates[SDL_SCANCODE_SPACE]) {
            isPreviewing = !isPreviewing;
            if (!isPreviewing) {
                loadBackground(gRenderer, mode);
                gameValue.startTime = SDL_GetTicks() / 1000 - gameValue.score;
            } else {
                for (int i = 0; i < mode; i++)
                    for (int j = 0; j < mode; j++) {
                        int w = (475 - (mode-1)*5) / mode;
                        int h = (475 - (mode-1)*5) / mode;
                        int x = j * h + j * 5 + 125;
                        int y = i * w + i * 5 + 176;
                        if (i < mode-1 || j < mode-1) drawPiece(gRenderer, mode, x, y, w, h, i*mode + j + 1);
                        else drawPiece(gRenderer, mode, x, y, w, h, 0);
                }
                SDL_RenderPresent(gRenderer);
            }
            Sleep(200);
        }
    }

    Sleep(100);
    if (!quit && !backPressed) {
        DataManager::saveLastGame(false, gameValue, src);
        if (giveUp) OutroScreen::loseScreen(gRenderer, quit);
        else OutroScreen::winScreen(gRenderer, quit, mode-2, gameValue.score);
    }
    else
        DataManager::saveLastGame(true, gameValue, src);

    SDL_DestroyTexture(imageTexture);
}

//get data about the image
void GameScreen::loadData(SDL_Renderer* gRenderer, string& src, int& mode, bool isContinuing) {
    if (isContinuing) {
        src = DataManager::dataLastGame(gameValue);
        mode = gameValue.height;
    }

    SDL_Surface* surface = IMG_Load(src.c_str());
    if (surface == NULL) surface = IMG_Load("Picture/default.jpg");

    imageWidth = surface->w;
    imageHeight = surface->h;

    imageTexture = SDL_CreateTextureFromSurface(gRenderer, surface);

    SDL_FreeSurface(surface);
}

//draw screen
void GameScreen::loadBackground(SDL_Renderer* gRenderer, int mode) {
    //clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(gRenderer);

    //draw background
    if (backgroundGameScreen == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/GameBackground.png");
        backgroundGameScreen = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }
    SDL_RenderCopy( gRenderer, backgroundGameScreen, NULL, NULL );

    //draw image
    drawImage(gRenderer, mode);

    //show current score
    drawText(gRenderer, to_string(gameValue.score), 220, 57, {0,0,0,255}, 34);
    SDL_RenderPresent(gRenderer);

    SDL_RenderPresent( gRenderer );
}

bool GameScreen::checkFinished(int mode) {
    for (int i = 0; i < mode; i++)
        for (int j = 0; j < mode; j++)
            if (i != mode-1 || j != mode-1)
                if (gameValue.table[i][j] != i*mode + j + 1)
                    return false;
    return true;
}

void GameScreen::drawImage(SDL_Renderer* gRenderer, int mode) {
    for (int i = 0; i < mode; i++)
        for (int j = 0; j < mode; j++) {
            int w = (475 - (mode-1)*5) / mode;
            int h = (475 - (mode-1)*5) / mode;
            int x = j * h + j * 5 + 125;
            int y = i * w + i * 5 + 176;
            drawPiece(gRenderer, mode, x, y, w, h, gameValue.table[i][j]);
        }
}

//toPiece is empty 0
void GameScreen::swapPiece(SDL_Renderer* gRenderer, GAME& gameValue, int mode, int fromI, int fromJ, int toI, int toJ) {
    if (isPreviewing) return;

    if (fromI < 0 || fromI > mode-1 || fromJ < 0 || fromJ > mode-1) return;

    if (!gameValue.isSetStartTime) {
        gameValue.isSetStartTime = true;
        gameValue.startTime = SDL_GetTicks() / 1000 - gameValue.score;
    }

    int value = gameValue.table[fromI][fromJ];
    gameValue.table[fromI][fromJ] = 0;

    slidingAnimation(gRenderer, mode, value, fromI, fromJ, toI, toJ);

    gameValue.table[toI][toJ] = value;
    gameValue.pos0 = {fromI, fromJ};

    loadBackground(gRenderer, mode);
}

void GameScreen::showScore(SDL_Renderer* gRenderer) {
    if (!gameValue.isSetStartTime || isPreviewing) return;

    if (SDL_GetTicks() / 1000 - gameValue.startTime != gameValue.score) {
        gameValue.score = SDL_GetTicks() / 1000 - gameValue.startTime;

        erasePiece(gRenderer, 203, 51, 110, 48);
        drawText(gRenderer, to_string(gameValue.score), 220, 57, {0,0,0,255}, 34);

        SDL_RenderPresent(gRenderer);
    }
}

void GameScreen::slidingAnimation(SDL_Renderer* gRenderer, int mode, int value, int fromI, int fromJ, int toI, int toJ) {
    int sizePiece = (475 - (mode-1)*5) / mode;

    int fromY = fromI * sizePiece + fromI * 5 + 176;
    int fromX = fromJ * sizePiece + fromJ * 5 + 125;

    int toY = toI * sizePiece + toI * 5 + 176;
    int toX = toJ * sizePiece + toJ * 5 + 125;

    while (fromX != toX || fromY != toY) {
        showScore(gRenderer);

        erasePiece(gRenderer, fromX, fromY, sizePiece, sizePiece);
        //4x4 = 115 | 3x3 = 155 | 2x2 = 235
        int add = (sizePiece + 5) / 20;

        if (fromX < toX) fromX+=add;
        if (fromX > toX) fromX-=add;

        if (fromY < toY) fromY+=add;
        if (fromY > toY) fromY-=add;

        drawPiece(gRenderer, mode, fromX, fromY, sizePiece, sizePiece, value);

        SDL_RenderPresent( gRenderer );
        Sleep(10);
    }
}

void GameScreen::drawText(SDL_Renderer* gRenderer, string text, int x, int y, SDL_Color color, int sizeText) {
    TTF_Font* gFontGameScreen = TTF_OpenFont( "Font/score.ttf", sizeText);
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFontGameScreen, text.c_str(), color );
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );

    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = textSurface->w;
    dstRect.h = textSurface->h;

    SDL_RenderCopy(gRenderer, textTexture, NULL, &dstRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void GameScreen::drawPiece(SDL_Renderer* gRenderer, int mode, int x, int y, int w, int h, int value) {
    if (value == 0) {
        erasePiece(gRenderer, x, y, w, h);
        return;
    }

    SDL_Rect srcRect;
    srcRect.w = imageWidth / mode;
    srcRect.h = imageHeight / mode;
    srcRect.x = ((value-1) % mode) * srcRect.w;
    srcRect.y = ((value-1) / mode) * srcRect.h;

    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;

    //load image piece
    SDL_RenderCopy(gRenderer, imageTexture, &srcRect, &dstRect);

    //load border
    if (borderGameScreen == NULL) {
        SDL_Surface* surface = IMG_Load("Picture/border.png");
        borderGameScreen = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }
    SDL_RenderCopy(gRenderer, borderGameScreen, NULL, &dstRect);

    //draw number
    drawText(gRenderer, to_string(value), x+10, y+10, {0,0,0,255}, 25);
}

void GameScreen::erasePiece(SDL_Renderer* gRenderer, int x, int y, int w, int h) {
    SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_RenderCopy(gRenderer, backgroundGameScreen, &rect, &rect);
}

void GameScreen::shuffleGame(GAME& gameValue, int mode) {
    srand(time(NULL));
    for (int i = 0; i < 1000000; i++) {
        int o = rand() % 4;
        if (o == 0 && gameValue.pos0.X > 0) {
            swap(gameValue.table[gameValue.pos0.X][gameValue.pos0.Y], gameValue.table[gameValue.pos0.X-1][gameValue.pos0.Y]);
            gameValue.pos0.X--;
        }
        if (o == 1 && gameValue.pos0.X < mode-1) {
            swap(gameValue.table[gameValue.pos0.X][gameValue.pos0.Y], gameValue.table[gameValue.pos0.X+1][gameValue.pos0.Y]);
            gameValue.pos0.X++;
        }
        if (o == 2 && gameValue.pos0.Y > 0) {
            swap(gameValue.table[gameValue.pos0.X][gameValue.pos0.Y], gameValue.table[gameValue.pos0.X][gameValue.pos0.Y-1]);
            gameValue.pos0.Y--;
        }
        if (o == 3 && gameValue.pos0.Y < mode-1) {
            swap(gameValue.table[gameValue.pos0.X][gameValue.pos0.Y], gameValue.table[gameValue.pos0.X][gameValue.pos0.Y+1]);
            gameValue.pos0.Y++;
        }
    }
}


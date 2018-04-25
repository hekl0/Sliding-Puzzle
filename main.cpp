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
    SDL_SetWindowIcon(gWindow, IMG_Load("Picture/icon.png"));

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

    while (!quit) {
        if (!DataManager::haveLastGame()) MainScreen::start(gRenderer, quit);
        else MainScreenContinue::start(gRenderer, quit);
    }

    close();
    return 0;
}

#include <main.h>

SDL_Texture* backgroundInstructionScreen[3];

void InstructionScreen::start(SDL_Renderer* gRenderer, bool& quit) {
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
                    //back
                    if (86 <= x && x <= 139 && 95 <= y && y <= 131) backPressed = true;

                    //left
                    if (56 <= x && x <= 106 && 335 <= y && y <= 370) {
                        mode = (mode - 1 + 3) % 3;
                        loadBackground(gRenderer, mode);
                    }
                    //right
                    if (596 <= x && x <= 650 && 335 <= y && y <= 370) {
                        mode = (mode + 1) % 3;
                        loadBackground(gRenderer, mode);
                    }
                }
        }
    }
}

void InstructionScreen::loadBackground(SDL_Renderer* gRenderer, int mode) {
    //clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear(gRenderer);

    //load background
    if (backgroundInstructionScreen[mode] == NULL) {
        string src = "";
        src = src + "Picture/Instruction" + to_string(mode) + ".png";
        SDL_Surface* surface = IMG_Load(src.c_str());
        backgroundInstructionScreen[mode] = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
    }
    SDL_RenderCopy(gRenderer, backgroundInstructionScreen[mode], NULL, NULL);

    SDL_RenderPresent(gRenderer);
}

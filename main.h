#ifndef _MAIN_H_INCLUDED
#define _MAIN_H_INCLUDED

#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include <SDL_ttf.h>
#include <vector>

using namespace std;
////////////////////////////////////
struct GAME {
    int width, height;
    int table[4][4];
    COORD pos0;
    int score;
    int startTime;
    bool isSetStartTime;
};

class State {
    public:
        int sizeN;
        int cost;
        int table[4][4];
        COORD emptySlot;
        std::string lastMove;

        State() {}

        State(int sizeN, int cost, int table[4][4], COORD emptySlot, std::string lastMove) {
            this->sizeN = sizeN;
            this->cost = cost;
            for (short i = 0; i < 4; i++)
                for (short j = 0; j < 4; j++)
                    this->table[i][j] = table[i][j];
            this->emptySlot = emptySlot;
            this->lastMove = lastMove;
        }

        bool operator < (State node) const {
            for(int i = 0; i < sizeN; i++)
                for (int j = 0; j < sizeN; j++) {
                    if(node.table[i][j] < this->table[i][j]) return true;
                    if(node.table[i][j] > this->table[i][j]) return false;
                }
            return false;
        }

        bool operator > (State node) const {
            for(int i = 0; i < sizeN; i++)
                for (int j = 0; j < sizeN; j++) {
                    if(node.table[i][j] > this->table[i][j]) return true;
                    if(node.table[i][j] < this->table[i][j]) return false;
                }
            return false;
        }
};


class SolvingPuzzle {
    public:
        static string solvePuzzle(GAME gameValue);
        static void neighbor(State node);
        static int HeuristicCost(State node);
        static void restruct(State &node);
        static void moveBlock(string moves, State &node);
        static COORD locate(int value, State node);
};
////////////////////////////////////////////
const int SCREEN_WIDTH = 700, SCREEN_HEIGHT = 700;

class MainScreen {
    public:
        static void start(SDL_Renderer* gRenderer, bool& quit);
};

class MainScreenContinue {
    public:
        static void start(SDL_Renderer* gRenderer, bool& quit);
};

class OptionScreen {
    public:
        static void start(SDL_Renderer* gRenderer, bool& quit);
        static void loadBackground(SDL_Renderer* gRenderer, int mode, string src, bool isChangingImage, string text);
        static void loadImage(SDL_Renderer* gRenderer, string src);
        static void EasyOption(SDL_Renderer* gRenderer);
        static void MediumOption(SDL_Renderer* gRenderer);
        static void HardOption(SDL_Renderer* gRenderer);
        static void changeImageSource(SDL_Renderer* gRenderer, int mode, string& src, bool& quit);
};

class GameScreen {
    public:
        static void start(SDL_Renderer* gRenderer, bool& quit, int mode, string src, bool isContinuing);
        static void loadBackground(SDL_Renderer* gRenderer, int mode);
        static void loadData(SDL_Renderer* gRenderer, string& src, int& mode, bool isContinuing);
        static void drawText(SDL_Renderer* gRenderer, string text, int x, int y, SDL_Color color);
        static void drawPiece(SDL_Renderer* gRenderer, int mode, int x, int y, int w, int h, int value);
        static void erasePiece(SDL_Renderer* gRenderer, int x, int y, int w, int h);
        static void drawImage(SDL_Renderer* gRenderer, int mode);
        static void shuffleGame(GAME& gameValue, int mode);
        static void swapPiece(SDL_Renderer* gRenderer, GAME& gameValue, int mode, int fromI, int fromJ, int toI, int toJ);
        static void slidingAnimation(SDL_Renderer* gRenderer, int mode, int value, int fromI, int fromJ, int toI, int toJ);
        static bool checkFinished(int mode);
        static void showScore(SDL_Renderer* gRenderer);
};

class HighscoreScreen {
    public:
        static void start(SDL_Renderer* gRenderer, bool& quit);
        static void loadBackground(SDL_Renderer* gRenderer, int mode);
        static void easyOption(SDL_Renderer* gRenderer, int mode);
        static void mediumOption(SDL_Renderer* gRenderer, int mode);
        static void hardOption(SDL_Renderer* gRenderer, int mode);
        static void loadHighscore(SDL_Renderer* gRenderer, int mode);
        static void drawText(SDL_Renderer* gRenderer, string text, int x, int y);
};

class DataManager {
    public:
        static void addItem(int mode, string name, int score);
        static vector< pair<string, int> > getAllItem(int mode);
        static bool haveLastGame();
        static string dataLastGame(GAME& gameValue);
        static void saveLastGame(bool isUnfinish, GAME gameValue, string src);
};

class InstructionScreen {
    public:
        static void start(SDL_Renderer* gRenderer, bool& quit);
        static void loadBackground(SDL_Renderer* gRenderer, int mode);
};

class OutroScreen {
    public:
        static void loseScreen(SDL_Renderer* gRenderer, bool& quit);
        static void winScreen(SDL_Renderer* gRenderer, bool& quit, int mode, int score);
        static void drawName(SDL_Renderer* gRenderer, string text);
};
#endif



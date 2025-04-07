#pragma once
#ifndef _FONTS__H
#define _FONTS__H

#include "SDL_wrapper.h"
#include <vector>
#include <string>

struct TextItem {
    SDL_Texture* texture = nullptr;
    std::string text;
    SDL_Rect rect = { 0, 0, 0, 0 };

    void createText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const std::string& text, int x, int y, int state);
    void displayText(SDL_Renderer* renderer, int x = -1, int y = -1);
    void clearText();
};

struct Fonts {
private:   
    TTF_Font* fontEndGame;
    TTF_Font* fontStage;
    TextItem stageText;
    TextItem YouWin;
	TextItem PressRToRestart;
	TextItem PressArrowKeysToStart;
    TextItem CountText;

public:
    Fonts();
    ~Fonts();

    void logErrorAndExit(const char* msg, const char* error);
    void init(SDL_Renderer* renderer);
    void initTTF();

    void clearAllTexts();
    void quitTTF();

    TextItem getYouWin() const { return YouWin; }
    TextItem getPressRToRestart() const { return PressRToRestart; }
    TextItem getPressArrowKeysToStart() const { return PressArrowKeysToStart; }
    TextItem& getCountText() { return CountText; }
	TTF_Font* getFontEndGame() const { return fontEndGame; }
	TTF_Font* getFontStage() const { return fontStage; }
};

#endif // _FONTS__H
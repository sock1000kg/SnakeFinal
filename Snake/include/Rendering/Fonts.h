#pragma once
#ifndef _FONTS__H
#define _FONTS__H

#include "SDL_wrapper.h"
#include <vector>
#include <string>

struct TextItem {
    SDL_Texture* texture;
    std::string text;
    SDL_Rect rect;

    void createText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const std::string& text, int x, int y, int state);
    void displayText(SDL_Renderer* renderer, int x = -1, int y = -1);
    void clearText();
};

struct Fonts {
    TTF_Font* fontEndGame;
    TTF_Font* fontStage;
    TextItem stageText;
    TextItem YouWin;
	TextItem PressRToRestart;
	TextItem PressArrowKeysToStart;
    TextItem CountText;

    Fonts();
    ~Fonts();

    void logErrorAndExit(const char* msg, const char* error);
    void init(SDL_Renderer* renderer);
    void initTTF();

    void clearAllTexts();
    void quitTTF();
};

#endif // _FONTS__H
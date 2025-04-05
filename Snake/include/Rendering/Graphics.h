#pragma once
#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include "SDL_Wrapper.h"
#include <vector>
#include <string>

struct Graphics {
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Texture* backgroundTexture;
    SDL_Texture* grass;
    SDL_Texture* appleTexture;
    SDL_Texture* obstacleTexture;

    const char* WINDOW_TITLE = "HAHAHAHAHAHHAHAHAHAHAHHAHAHAHAHAHAHHAHAHA";

    Graphics();
    ~Graphics();

    void logErrorAndExit(const char* msg, const char* error);
    void init();
    void initSDL();

    void changeBackground(const char* filename);
    void drawApple(const SDL_Rect& rect);
    void drawObstacle(const SDL_Rect& rect);

    void renderScene();
    void clearScene();
    void presentScene();
    void quit();
};

#endif // _GRAPHICS__H
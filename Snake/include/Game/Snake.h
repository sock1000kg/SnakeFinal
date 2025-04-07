#pragma once
#ifndef GAME_H
#define GAME_H

#include "Rendering/Graphics.h"
#include "Rendering/Fonts.h"
#include "Rendering/Sound.h"
#include "Defs.h"
#include <vector>
#include <algorithm>
#include <deque>
#include <sstream>

//SDL
extern SDL_Event event;
extern SDL_Color white;

//FUNCTIONS
struct Game {
    //Snake head
    SDL_Rect head = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, HEAD_SIZE, HEAD_SIZE };

    //Snake Body container
    std::deque <SDL_Rect> snake;

    //Apples and obstacles container
    std::vector <SDL_Rect> apples;
    std::vector<SDL_Rect> obstacles;

    //Game variables
    bool running = true, ended = false, music = true;

    int direction = STOP, lastDirection = STOP; //Movement

    int size = INITIAL_SIZE, applesEaten = 0, stage = 1, deathCount = 0;

    int appleCount = INIT_APPLE_COUNT; //Apples per stage

    int frameCounter = 0;
    Uint32 frameStart, frameTime;

    Graphics& graphics;
    Sounds& sounds;
    Fonts& texts;

    Game(Graphics& g, Sounds& s, Fonts& f);
    ~Game();

    //Inputs
    void checkInput();
    bool isClickOnSnakeFace(int mouseX, int mouseY);

    //Game logic
    void run();
    void restartGame();
    void setupStage();
    void death();
    bool isOverlapping(const SDL_Rect& a, const SDL_Rect& b);
    void handleCollisions();
    void handleMovement();
    void frameCapping();
    void cleanup();
};


#endif
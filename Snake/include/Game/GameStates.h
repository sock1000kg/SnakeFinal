#pragma once
#ifndef GAMESTATES_H
#define GAMESTATES_H

#include "Rendering/Graphics.h"
#include "Rendering/Fonts.h"
#include "Defs.h"
#include <SDL.h>

struct Game;

struct GameStates {
    static void renderStartScreen(Game& game);
    static void waitForStart(Game& game);
    static void renderEndScreen(Game& game);
    static void endGame(Game& game);
    static void stageDisplay(Game& game);
};

#endif
#pragma once
#ifndef _DEFS__H
#define _DEFS__H

//GAME VARIABLES
const int HEAD_SIZE = 20;
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int INITIAL_SIZE = 2;
const int MAX_STAGE = 3;
const int INIT_APPLE_COUNT = 5;
const int BASE_SPEED = HEAD_SIZE-13;
const int FRAME_DELAY = 1000 / 60;

extern SDL_Color white;
extern SDL_Color black;

enum Directions { STOP, DOWN, LEFT, RIGHT, UP };
#endif
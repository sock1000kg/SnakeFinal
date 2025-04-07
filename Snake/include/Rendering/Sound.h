#pragma once
#ifndef _SOUNDS__H
#define _SOUNDS__H

#include "SDL_wrapper.h"

struct Sounds {
private:
    Mix_Chunk* eatSound, * dieSound, * yay;
    Mix_Music* bgMusic;
    int volume;

public:
    Sounds();
    ~Sounds();

    void logErrorAndExit(const char* msg, const char* error);
    void init();
    void initMIX();

    void setVolume(int newVolume, Mix_Chunk* sound = nullptr, Mix_Music* track = nullptr);
    void playMusic(int loops = -1);
    void stopMusic();
    void playSound(Mix_Chunk* sound, int channel = -1, int loops = 0);
    void quitMIX();

	Mix_Chunk* getEatSound() const { return eatSound; }
	Mix_Chunk* getDieSound() const { return dieSound; }
	Mix_Chunk* getYay() const { return yay; }
	Mix_Music* getBgMusic() const { return bgMusic; }
};

#endif // _SOUNDS__H
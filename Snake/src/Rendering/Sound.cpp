#include "Rendering/Sound.h"

Sounds::Sounds() : eatSound(nullptr), dieSound(nullptr), yay(nullptr), bgMusic(nullptr), volume(MIX_MAX_VOLUME/2){}

//INITIALIZATION
Sounds::~Sounds() {
    quitMIX();
}

void Sounds::logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    exit(1);
}

void Sounds::init() {
    initMIX();
    eatSound = Mix_LoadWAV("Assets\\sound\\eatsound.wav");
    if (!eatSound) logErrorAndExit("Load_eatSound_error", Mix_GetError());

    dieSound = Mix_LoadWAV("Assets\\sound\\diesound.wav");
    if (!dieSound) logErrorAndExit("Load_dieSound_error", Mix_GetError());

    yay = Mix_LoadWAV("Assets\\sound\\yay.wav");
    if (!yay) logErrorAndExit("Load_yay_error", Mix_GetError());

    bgMusic = Mix_LoadMUS("Assets\\sound\\background.mp3");
    if (!bgMusic) logErrorAndExit("Load_music_error", Mix_GetError());

    SDL_Log("Sound initialized successfully!!");
}

void Sounds::initMIX() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) logErrorAndExit("MIXER_Init", Mix_GetError());
}

//SOUND SETTINGS AND PLAYING
void Sounds::setVolume(int newVolume, Mix_Chunk* sound, Mix_Music* track) {
    if (newVolume < 0) newVolume = 0;
    if (newVolume > MIX_MAX_VOLUME) newVolume = MIX_MAX_VOLUME;

    volume = newVolume;

    // Apply new volume to one sound
    if (sound) { Mix_VolumeChunk(sound, volume); }
    else {
     // Apply volume to all sounds
        if (eatSound) Mix_VolumeChunk(eatSound, volume);
        if (dieSound) Mix_VolumeChunk(dieSound, volume);
        if (yay) Mix_VolumeChunk(yay, volume);
    }

    if (track) Mix_VolumeMusic(volume);
}

void Sounds::playMusic(int loops) {
    if (!bgMusic) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "No background music exist: %s", Mix_GetError());
        return;
    }
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(bgMusic, loops);
    }
}

void Sounds::stopMusic() {
    Mix_HaltMusic();
}

void Sounds::playSound(Mix_Chunk* sound, int channel, int loops) {
    if (!sound) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "No sound effect exist: %s", Mix_GetError());
        return;
    }
    Mix_PlayChannel(channel, sound, loops);
}

//QUITTING
void Sounds::quitMIX() {
    if (eatSound) {
        Mix_FreeChunk(eatSound);
        eatSound = nullptr;
        SDL_Log("eatSound destroyed");
    }
    if (dieSound) {
        Mix_FreeChunk(dieSound);
        dieSound = nullptr;
		SDL_Log("dieSound destroyed");
    }
    if (yay) {
        Mix_FreeChunk(yay);
        yay = nullptr;
		SDL_Log("yay destroyed");
    }
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
        bgMusic = nullptr;
		SDL_Log("bgMusic destroyed");
    }
    Mix_CloseAudio();
    Mix_Quit();
}

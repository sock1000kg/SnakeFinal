#define SDL_MAIN_HANDLED

#include "Game/Snake.h"
#include "Game/GameStates.h"
#include "Game/Defs.h"
#include <iostream>

SDL_Event event;

Graphics graphics;
Sounds sounds;
Fonts texts;

int main() {
	graphics.init();
	sounds.init();
	texts.init(graphics.getRenderer());

	Game game(graphics, sounds, texts);

	//Setting volume of sounds
	sounds.setVolume(MIX_MAX_VOLUME / 4, sounds.getEatSound());
	sounds.setVolume(MIX_MAX_VOLUME / 4, sounds.getDieSound());
	sounds.setVolume(20, sounds.getYay());

	game.setupStage();

	//Start screen
	GameStates::waitForStart(game);

	//Background music
	sounds.setVolume(70, nullptr, sounds.getBgMusic());
	sounds.playMusic();

	//GAME LOOP
	game.run();
	game.cleanup();
	return 0;
}

#define SDL_MAIN_HANDLED

#include "Snake.h"
#include "GameStates.h"
#include "Defs.h"
#include <iostream>

SDL_Event event;

Graphics graphics;
Sounds sounds;
Fonts texts;

int main() {
	graphics.init();
	sounds.init();
	texts.init(graphics.renderer);

	Game game(graphics, sounds, texts);

	//Setting volume of sounds
	sounds.setVolume(MIX_MAX_VOLUME / 4, sounds.eatSound);
	sounds.setVolume(MIX_MAX_VOLUME / 4, sounds.dieSound);
	sounds.setVolume(20, sounds.yay);

	game.setupStage();

	//Start screen
	GameStates::waitForStart(game);

	//Background music
	sounds.setVolume(70, nullptr, sounds.bgMusic);
	sounds.playMusic();

	//GAME LOOP
	game.run();
	game.cleanup();
	return 0;
}

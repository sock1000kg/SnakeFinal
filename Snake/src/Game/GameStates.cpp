#include "Game/GameStates.h"
#include "Game/Snake.h"

//START SCREEN
void GameStates::renderStartScreen(Game& game) {
    game.graphics.changeBackground("Assets\\image\\background.jpg");
    game.graphics.clearScene();
    game.graphics.renderScene();

    game.texts.PressArrowKeysToStart.displayText(game.graphics.renderer);

    game.graphics.presentScene();
}
void GameStates::waitForStart(Game& game) {
    bool waitingForInput = true;
    renderStartScreen(game);

    while (waitingForInput) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game.cleanup();
                exit(0);
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
                    game.texts.PressArrowKeysToStart.clearText();
                    game.graphics.changeBackground("Assets\\image\\grass.png");
                    waitingForInput = false;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (game.isClickOnSnakeFace(mouseX, mouseY)) {
                    game.sounds.playSound(game.sounds.yay, -1, 0);
                }
            }
        }
    }
}


//END GAME SCREEN
void GameStates::renderEndScreen(Game& game) {
    game.graphics.changeBackground("Assets\\image\\background.jpg");
    game.graphics.clearScene();
    game.graphics.renderScene();

    // Create the death count text
    std::string deathCountStr = "Deaths: " + std::to_string(game.deathCount);
    game.texts.CountText.createText(game.graphics.renderer, game.texts.fontStage, white, deathCountStr, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150, 1);

    game.texts.YouWin.displayText(game.graphics.renderer);
    game.texts.PressRToRestart.displayText(game.graphics.renderer);
    game.texts.CountText.displayText(game.graphics.renderer);
    game.graphics.presentScene();
}
void GameStates::endGame(Game& game) {
    // Yay sound
    game.sounds.playSound(game.sounds.yay, -1, 0);
    renderEndScreen(game);
    while (game.ended) {
        game.checkInput();
    }
}

void GameStates::stageDisplay(Game& game) {
    game.graphics.clearScene();
    // Keep printing head forward
    game.snake.push_front(game.head);

    // Pop the tail off snake
    while (game.snake.size() > game.size) {
        game.snake.pop_back();
    }

    game.graphics.renderScene();

    // Draw Body
    SDL_SetRenderDrawColor(game.graphics.renderer, 255, 255, 255, 255);
    for (const auto& segment : game.snake) {
        SDL_RenderFillRect(game.graphics.renderer, &segment);
    }

    // Draw Apples
    for (const auto& apple : game.apples) {
        game.graphics.drawApple(apple);
    }

    // Draw Obstacles
    for (const auto& obstacle : game.obstacles) {
        game.graphics.drawObstacle(obstacle);
    }

    game.graphics.presentScene();
}

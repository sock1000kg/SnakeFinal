#include "Game/Snake.h"
#include "Game/GameStates.h"
#include <cstdlib>


Game::Game(Graphics& g, Sounds& s, Fonts& f)
    : graphics(g), sounds(s), texts(f)  // Initialize references for game rendering
{
    deathCount = 0;
    restartGame();  // Initialize all game state
}

// Destructor
Game::~Game() = default;

//HANDLING INPUTS
void Game::checkInput() {
    // Check for input
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            restartGame();
            running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            if (!ended) {
                if (event.key.keysym.sym == SDLK_DOWN && lastDirection != UP) { direction = DOWN; }
                if (event.key.keysym.sym == SDLK_UP && lastDirection != DOWN) { direction = UP; }
                if (event.key.keysym.sym == SDLK_LEFT && lastDirection != RIGHT) { direction = LEFT; }
                if (event.key.keysym.sym == SDLK_RIGHT && lastDirection != LEFT) { direction = RIGHT; }
            }
            if (event.key.keysym.sym == SDLK_r) { restartGame(); break; } // Restart game with "r"
            if (event.key.keysym.sym == SDLK_m) { // Toggle music on and off with "m"
                if (music) {
                    sounds.stopMusic();
                    music = false;
                }
                else {
                    sounds.playMusic();
                    music = true;
                }
            }
        }
        if (ended) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (isClickOnSnakeFace(mouseX, mouseY)) {
                    sounds.playSound(sounds.getYay(), -1, 0); //PLAY YAY SOUND WHEN CLICK ON THE SNAKE HAHAHAHAHHAHHAHA
                }
            }
        }
    }
}
bool Game::isClickOnSnakeFace(int mouseX, int mouseY) {
    // Define the area above the snake's face (200 pixels above the center)
    SDL_Rect clickArea = { SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 300, 300, 300 };
    SDL_Point mousePoint = { mouseX, mouseY };
    return SDL_PointInRect(&mousePoint, &clickArea);
}

void Game::run() {
    while (running) {
        frameStart = SDL_GetTicks();
        checkInput();
        if (ended) {
            GameStates::endGame(*this);
        }
        else {
            if (stage <= MAX_STAGE) {
                handleMovement();
                handleCollisions();

                GameStates::stageDisplay(*this);
            }
            else ended = true;
        }
        frameCapping();
    }
}
//RESTARTING
void Game::restartGame() {
    //snake
    snake.clear();
    head = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, HEAD_SIZE, HEAD_SIZE };
    snake.push_front(head);

    //variables
    appleCount = INIT_APPLE_COUNT;
    apples.clear();
    obstacles.clear();
    stage = 1;
    deathCount = 0;
    direction = STOP;
    applesEaten = 0;
    size = INITIAL_SIZE;
    ended = false;

    graphics.changeBackground("Assets\\image\\grass.png");

    setupStage();
}

void Game::setupStage() {
    apples.clear();
    obstacles.clear();
    direction = STOP;

    if (stage > MAX_STAGE) return;

    // Update texts
    std::string stageCountStr = "Stage: " + std::to_string(stage) + "/" + std::to_string(MAX_STAGE);
    texts.getStageCountText().createText(graphics.getRenderer(), texts.getFontStage(), white, stageCountStr, SCREEN_WIDTH - 125, 20, 0);

    std::string deathCountStr = "Deaths: " + std::to_string(deathCount);
    texts.getDeathCountText().createText(graphics.getRenderer(), texts.getFontStage(), white, deathCountStr, 20, 20, 0);

    appleCount = INIT_APPLE_COUNT + (stage - 1) * (INIT_APPLE_COUNT);  // Apples per stage
    int obstacleCount = (stage - 1) * (INIT_APPLE_COUNT*2);   // Obstacles per stage

    // Obstacle generation
    for (int i = 0; i < obstacleCount; i++) {
        SDL_Rect obstacle;
        bool validPositionOb;
        do {
            validPositionOb = true;

            obstacle.x = (rand() % (SCREEN_WIDTH / HEAD_SIZE)) * HEAD_SIZE;
            obstacle.y = (rand() % (SCREEN_HEIGHT / HEAD_SIZE)) * HEAD_SIZE;
            obstacle.w = HEAD_SIZE;
            obstacle.h = HEAD_SIZE;

            // Restrict spawning on the top level
            if (obstacle.y < 50) {
                validPositionOb = false;
                continue;
            }

            for (const auto& snake_segment : snake) { // Check if overlap with snake
                if (isOverlapping(obstacle, snake_segment)) {
                    validPositionOb = false;
                    break;
                }
                if (isOverlapping(obstacle, head)) {
                    validPositionOb = false;
                    break;
                }
            }
        } while (!validPositionOb);

        obstacles.push_back(obstacle); // Push only after validation
    }

    // Apple generation
    for (int i = 0; i < appleCount; i++) {
        SDL_Rect apple;
        bool validPositionAp;
        do {
            validPositionAp = true;
            apple.x = (rand() % (SCREEN_WIDTH / HEAD_SIZE)) * HEAD_SIZE;
            apple.y = (rand() % (SCREEN_HEIGHT / HEAD_SIZE)) * HEAD_SIZE;
            apple.w = HEAD_SIZE;
            apple.h = HEAD_SIZE;

            if (apple.y < 50) { // Restrict spawning on the top level
                validPositionAp = false;
                continue;
            }

            // Check if it overlaps with any obstacles
            for (const auto& obstacle : obstacles) {
                if (isOverlapping(apple, obstacle)) {
                    validPositionAp = false;
                    break;
                }
            }

            // Check if it overlaps with any other apples
            for (const auto& other_apple : apples) {
                if (isOverlapping(apple, other_apple)) {
                    validPositionAp = false;
                    break;
                }
            }


            // Check if overlap with snake
            for (const auto& snake_segment : snake) {
                if (isOverlapping(apple, snake_segment)) {
                    validPositionAp = false;
                    break;
                }
            }

            // Check if the apple is surrounded by obstacles**
            int surroundingObstacles = 0;
            SDL_Rect neighbors[4] = {
                {apple.x - HEAD_SIZE, apple.y, HEAD_SIZE, HEAD_SIZE},  // Left
                {apple.x + HEAD_SIZE, apple.y, HEAD_SIZE, HEAD_SIZE},  // Right
                {apple.x, apple.y - HEAD_SIZE, HEAD_SIZE, HEAD_SIZE},  // Above
                {apple.x, apple.y + HEAD_SIZE, HEAD_SIZE, HEAD_SIZE}   // Below
            };
            for (const auto& neighbor : neighbors) {
                for (const auto& obstacle : obstacles) {
                    if (isOverlapping(neighbor, obstacle)) {
                        surroundingObstacles++;
                    }
                }
            }
            if (surroundingObstacles == 4) {
                validPositionAp = false;
            }

        } while (!validPositionAp);

        apples.push_back(apple); // Push only after validation
    }
}


//DEATH
void Game::death() {
    sounds.playSound(sounds.getDieSound());
    size = INITIAL_SIZE;  // Reset size on collision
    applesEaten = 0; // Reset apple count for the stage
    deathCount++;

    setupStage(); // Restart current stage
}

//GENERATION OF APPLES AND COLLISIONS
bool Game::isOverlapping(const SDL_Rect& a, const SDL_Rect& b) { // To prevent overlap in generation and checking collision
    return (SDL_HasIntersection(&a, &b));
}

//HANDLING COLLISIONS
void Game::handleCollisions() {
    // Apple Collision
    for (auto it = apples.begin(); it != apples.end(); ) {
        if (SDL_HasIntersection(&head, &*it)) {
            sounds.playSound(sounds.getEatSound());
            size += HEAD_SIZE / 4;
            applesEaten++;
            it = apples.erase(it); // Erase returns the next iterator
        }
        else {
            ++it;
        }
    }

    // Advance stage
    if (applesEaten == appleCount) {
        if (stage <= MAX_STAGE) {
            stage++;
            applesEaten = 0;
            setupStage();
        }
    }

    // Obstacle Collision
    std::for_each(obstacles.begin(), obstacles.end(), [&](auto& obstacle) {
        if (SDL_HasIntersection(&head, &obstacle)) {
            death();
        }
        });

    // Collision detection with body
    std::for_each(snake.begin(), snake.end(), [&](auto& snake_segment) {
        if (direction && head.x == snake_segment.x && head.y == snake_segment.y) {
            death();
        }
        });
}

void Game::handleMovement() {
    int Move_Speed = BASE_SPEED;
    // Movement
    switch (direction) {
    case DOWN:
        head.y += Move_Speed; break;
    case UP:
        head.y -= Move_Speed; break;
    case LEFT:
        head.x -= Move_Speed; break;
    case RIGHT:
        head.x += Move_Speed; break;
    case STOP:
        break;
    }
    lastDirection = direction;

    // Screen wrapping logic
    if (head.x < 0) head.x = SCREEN_WIDTH - HEAD_SIZE;  // Wrap left to right
    if (head.x >= SCREEN_WIDTH) head.x = 0;             // Wrap right to left
    if (head.y < 0) head.y = SCREEN_HEIGHT - HEAD_SIZE;   // Wrap top to bottom
    if (head.y >= SCREEN_HEIGHT) head.y = 0;              // Wrap bottom to top
}

//FRAME CAPPING (Currently 60 fps)
void Game::frameCapping() {
    frameTime = SDL_GetTicks() - frameStart;
    if (FRAME_DELAY > frameTime) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }
}

//CLEANUP
void Game::cleanup() {
    graphics.quit();
    sounds.quitMIX();
    texts.quitTTF();
}
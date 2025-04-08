// Graphics.cpp
#include "Rendering/Graphics.h"
#include "Game/Defs.h"

SDL_Color white = { 255, 255, 255, 255 };
SDL_Color black = { 0, 0, 0, 255 };

//INITIALIZATION
Graphics::Graphics() {
    renderer = nullptr; 
    window = nullptr; 
    backgroundTexture = nullptr;
	grass = nullptr; 
    appleTexture = nullptr;
	obstacleTexture = nullptr;
}

Graphics::~Graphics() {
    quit();
}

void Graphics::logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
    exit(1);
}

void Graphics::init() {
    initSDL();

    //Load background
    SDL_Surface* surface = IMG_Load("Assets\\image\\background.jpg");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!backgroundTexture) logErrorAndExit("Load_background_error", IMG_GetError());

    // Load apple texture
    surface = IMG_Load("Assets\\image\\apple.png");
    appleTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!appleTexture) logErrorAndExit("Load_apple_error", IMG_GetError());

	// Load obstacle texture 
	surface = IMG_Load("Assets\\image\\tile.png"); 
	obstacleTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!obstacleTexture) logErrorAndExit("Load_obstacle_error", IMG_GetError());
}

void Graphics::initSDL() {
    //SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logErrorAndExit("SDL_Init", SDL_GetError());

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    //SDL_IMAGE
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) logErrorAndExit("SDL_image error:", IMG_GetError());
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Log("SDL initialized successfully!");
}

//DRAWING OBJECTS
void Graphics::drawApple(const SDL_Rect& rect) {
    if (appleTexture) {
        SDL_Rect Rect = rect;

        //Scale the texture
        Rect.w = static_cast<int>(rect.w * 1.6);
        Rect.h = static_cast<int>(rect.h * 1.6);

        //Adjust x and y to keep the apple centered
        Rect.x = rect.x - (Rect.w - rect.w) / 2;
        Rect.y = rect.y - (Rect.h - rect.h) / 2; 

        SDL_RenderCopy(renderer, appleTexture, nullptr, &Rect);
    }
    else {
        SDL_Log("Apple texture is null, cannot draw apple");
    }
}

void Graphics::drawObstacle(const SDL_Rect& rect) {
	if (obstacleTexture) {
        SDL_Rect Rect = rect;

        //Scale the texture
        Rect.w = static_cast<int>(rect.w * 1.2);
        Rect.h = static_cast<int>(rect.h * 1.2);

        //Adjust x and y to keep the apple centered
        Rect.x = rect.x - (Rect.w - rect.w) / 2; 
        Rect.y = rect.y - (Rect.h - rect.h) / 2;

		SDL_RenderCopy(renderer, obstacleTexture, nullptr, &Rect);
	}
	else {
		SDL_Log("Obstacle texture is null, cannot draw obstacle");
	}
}

//DRAWING SCENES
void Graphics::changeBackground(const char* filename) {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    backgroundTexture = IMG_LoadTexture(renderer, filename);
    if (!backgroundTexture) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "No background texture exist: %s", IMG_GetError());
}

void Graphics::renderScene() {
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }
}

void Graphics::clearScene() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Set black background
    SDL_RenderClear(renderer);
}

void Graphics::presentScene() {
    SDL_RenderPresent(renderer);
}

//QUITTING
void Graphics::quit() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
        SDL_Log("backgroundTexture destroyed");
    }
    if (grass) {
        SDL_DestroyTexture(grass);
        grass = nullptr;
        SDL_Log("grass destroyed");
    }
    if (appleTexture) {
        SDL_DestroyTexture(appleTexture);
        appleTexture = nullptr;
		SDL_Log("appleTexture destroyed");
    }
    if (obstacleTexture) {
        SDL_DestroyTexture(obstacleTexture);
        obstacleTexture = nullptr;
        SDL_Log("obstacleTexture destroyed");
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
		SDL_Log("renderer destroyed");
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
		SDL_Log("window destroyed");
    }

    IMG_Quit();
    SDL_Quit();
}

#include "Rendering/Fonts.h"
#include "Game/Snake.h"
#include "Game/Defs.h"

//INTIALIZATION
Fonts::Fonts() {
    fontEndGame = nullptr;
    fontStage = nullptr;
    YouWin = { nullptr, "", {0, 0, 0, 0} };
    PressRToRestart = { nullptr, "", {0, 0, 0, 0} };
    PressArrowKeysToStart = { nullptr, "", {0, 0, 0, 0} };
	CountText = { nullptr, "", {0, 0, 0, 0} };
}
Fonts::~Fonts() {
    clearAllTexts();
    quitTTF();
}

void Fonts::logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    TTF_Quit();
    SDL_Quit();
    exit(1);
}


void Fonts::init(SDL_Renderer* renderer) {
    initTTF();
    fontEndGame = TTF_OpenFont("Assets\\font\\SavoryCurry.ttf", 100); // For titles
    fontStage = TTF_OpenFont("Assets\\font\\SavoryCurry.ttf", 20);    // For normal texts
    if (!fontEndGame || !fontStage) logErrorAndExit("Load_fonts_error", TTF_GetError());


    PressArrowKeysToStart.createText(renderer, fontStage, white, "Press Arrow Keys to Start", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1);
    YouWin.createText(renderer, fontEndGame, white, "You Win!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1);
    PressRToRestart.createText(renderer, fontStage, white, "Press R to Restart", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, 2);
	CountText.createText(renderer, fontStage, white, "Deaths: 0", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150, 1);

    SDL_Log("Fonts initialized successfully!!!");
}

void Fonts::initTTF() {
    if (TTF_Init() == -1) logErrorAndExit("TTF_Init", TTF_GetError());
}

//TEXT ITEM HANDLING
void TextItem::createText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const std::string& text, int x, int y, int state) {
    if (!font) {
        SDL_Log("Font nullptr in createText()");
        return;
    }

    // If the new text is the same as an existing text, do nothing
    if (this->text == text) {
        SDL_Log("Text is the same: %s", text.c_str());
        return;
    }

    clearText();

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        SDL_Log("Unable to create text surface: %s", TTF_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        SDL_Log("Unable to create text texture: %s", SDL_GetError());
        return;
    }

    this->texture = textTexture;
    this->text = text;
    this->rect = { x, y, textSurface->w, textSurface->h };

    if (state == 1 || state == 2) {
        this->rect.x = x - textSurface->w / 2;
    }
    if (state == 1 || state == 3) {
        this->rect.y = y - textSurface->h / 2;
    }

    SDL_FreeSurface(textSurface);
    SDL_Log("Text created: %s, Position: (%d, %d)", this->text.c_str(), this->rect.x, this->rect.y);
}

void TextItem::displayText(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect destRect = this->rect;
    if (x != -1 && y != -1) {
        destRect.x = x;
        destRect.y = y;
    }

    if (this->texture) {
        SDL_RenderCopy(renderer, this->texture, nullptr, &destRect);
        SDL_Log("Text displayed: %s, Position: (%d, %d)", this->text.c_str(), destRect.x, destRect.y);
    }
    else {
        SDL_Log("Text texture is null, cannot display text: %s", this->text.c_str());
    }
}

void TextItem::clearText() {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
    SDL_Log("Text cleared: %s", this->text.c_str());
    this->text.clear();
    this->rect = { 0, 0, 0, 0 };
}

//FONTS HANDLING
void Fonts::clearAllTexts() {
    if (YouWin.texture) {
        SDL_DestroyTexture(YouWin.texture);
        SDL_Log("YouWin destroyed");
    }
    if (PressRToRestart.texture) {
        SDL_DestroyTexture(PressRToRestart.texture);
        SDL_Log("PressRToRestart destroyed");
    }
    if (PressArrowKeysToStart.texture) {
        SDL_DestroyTexture(PressArrowKeysToStart.texture);
        SDL_Log("PressArrowKeysToStart destroyed");
    }
    if (CountText.texture) {
        SDL_DestroyTexture(CountText.texture);
        SDL_Log("CountText destroyed");
    }
}

void Fonts::quitTTF() {
    clearAllTexts();
    if (fontEndGame) {
        TTF_CloseFont(fontEndGame);
        fontEndGame = nullptr;
    }
    if (fontStage) {
        TTF_CloseFont(fontStage);
        fontStage = nullptr;
    }
    TTF_Quit();
}
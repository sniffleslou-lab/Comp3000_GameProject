//
// Created by Lou halpin on 05/04/2026.
//
#include "QTEManager.h"
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>

QTEManager::QTEManager(SDL_Renderer* renderer)
: renderer(renderer) {
    SDL_Surface* orb =IMG_Load("../assets/textures/ui/progress_orb.png");
    progressBallTexture = SDL_CreateTextureFromSurface(renderer, orb);
    SDL_FreeSurface(orb);
}

void QTEManager::resetState() {
    active = false;
    success = false;
    failed = false;

    timer = 0.0f;
    currentIndex = 0;

    successProgress = 0.0f;
    failProgress = 0.0f;
}
void QTEManager::start(const QTEEvents &event) {
    currentEvent = event;

    resetState();
    active = true;
    currentIndex = 0;
    timer = 0.0f;


    keyPromptTexture = loadKeyTexture(currentEvent.sequence[currentIndex]);
}

SDL_Texture *QTEManager::loadKeyTexture(SDL_Keycode key) {
    std::string text;

    switch (key) {
        case SDLK_w: text = "W"; break;
        case SDLK_a: text = "A"; break;
        case SDLK_s: text = "S"; break;
        case SDLK_d: text = "D"; break;
        case SDLK_SPACE: text = "SPACE"; break;
        default: text = "?"; break;
    }
    TTF_Font* font = TTF_OpenFont("../assets/font/SunLight Dreams.otf", 32);
    SDL_Color black = {0,0,0};

    SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), black);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);
    TTF_CloseFont(font);

    return tex;
}
void QTEManager::update(float dt) {
    if (!active) return;

    timer += dt;

    //time ran out
    if (timer > currentEvent.timePerKey) {
        completeFail();
        return;
    }
}

void QTEManager::handleEvents(const SDL_Event &e) {
    if (!active) return;
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode pressed = e.key.keysym.sym;
        if (pressed == currentEvent.sequence[currentIndex]) {
            //correct key
            successProgress += currentEvent.successGain;

            currentIndex++;
            timer = 0.0f;
            if (currentIndex >= currentEvent.sequence.size()) {
                completeSuccess();
                return;
            }
            //loading key prompt
            keyPromptTexture = loadKeyTexture(currentEvent.sequence[currentIndex]);
        }else {
            //wrong key
            failProgress += currentEvent.failPenalty;
            if (failProgress >= 1.0f) {
                completeFail();
                return;
            }
        }
    }
}

void QTEManager::completeSuccess() {
    success = true;
    active = false;
}
void QTEManager::completeFail() {
    failed = true;
    active = false;
}

void QTEManager::render(SDL_Renderer* renderer) {
    if (!active) return;

    //screen size
    int screenW = 800;
    int screenH = 600;

    // Key prompt
    SDL_Rect keyRect = {screenW/2 - 50, screenH/2 - 120, 100, 100};
    SDL_RenderCopy(renderer, keyPromptTexture, nullptr, &keyRect);

    // Orb
    SDL_Rect orbRect = {screenW/2 - 32, screenH/2, 64, 64};
    SDL_RenderCopy(renderer, progressBallTexture, nullptr, &orbRect);

    // Progress bar (always visible)
    SDL_SetRenderDrawColor(renderer, 0,255,0,255);
    int barWidth = (int)(successProgress * 200);
    if (barWidth < 5) barWidth = 5;

    SDL_Rect bar = {screenW/2 - 100, screenH/2 + 80, barWidth, 20};
    SDL_RenderFillRect(renderer, &bar);
}

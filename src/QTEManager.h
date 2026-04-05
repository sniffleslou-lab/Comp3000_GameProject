//
// Created by Lou halpin on 05/04/2026.
//

#ifndef AMIPRETTY_QTEMANAGER_H
#define AMIPRETTY_QTEMANAGER_H
#include <SDL.h>
#include <memory>
#include "QTEEvents.h"

class QTEManager {
    public:
    QTEManager(SDL_Renderer* renderer);

    void start(const QTEEvents& event);
    void update(float dt);
    void render(SDL_Renderer* renderer);

    void handleEvents(const SDL_Event& e);
    bool isActive() const { return active; }
    bool isSuccess() const { return success; }
    bool isFailure() const { return failure; }

    private:
    SDL_Renderer* renderer;

    QTEEvents currentEvent;
    int currentIndex = 0;

    float timer = 0.0f;
    float progress = 0.0f;

    bool active = false;
    bool success = false;
    bool failure = false;

    //ui
    SDL_Texture* keyPromptTexture = nullptr;
    SDL_Texture* progressBallTexture = nullptr;

    SDL_Texture* loadKeyTexture(SDL_Keycode key);
};

#endif //AMIPRETTY_QTEMANAGER_H
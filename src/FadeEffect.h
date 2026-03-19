//
// Created by Lou halpin on 19/03/2026.
//

#ifndef AMIPRETTY_FADEEFFECT_H
#define AMIPRETTY_FADEEFFECT_H

#include <SDL.h>
#include <__locale>

class FadeEffect {
    public:
    FadeEffect() : alpha(0),targetAlpha(0),speed(0),active(false) {}
    void start(Uint8 r, Uint8 g, Uint8 b, Uint8 target, float duration){
        color = {r,g,b,255};
        alpha = 0;
        targetAlpha = target;
        speed = target / duration;
        active = true;
    }
    void update(float dt) {
        if (active) return;

        alpha += speed * dt;
        if (alpha >= targetAlpha) {
            alpha = targetAlpha;
            active = false;
        }
    }
    void render(SDL_Renderer* renderer) {
        if (!active & alpha == 0) return;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, (Uint8)alpha);
        SDL_Rect full = {0,0,800,600};
        SDL_RenderFillRect(renderer, &full);
    }
    bool isActive() const {return active;}


    private:
    SDL_Color color;
    float targetAlpha;
    float speed;
    bool active;
};
#endif //AMIPRETTY_FADEEFFECT_H
#include "controls.h"
#include "character.h"

void Controls::handleInput(SDL_Event &event, Character &player) {
if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT: player.move(-5,0); break;
            case SDLK_RIGHT: player.move(5,0); break;
            case SDLK_UP: player.move(0,-5); break;
            case SDLK_DOWN: player.move(0, 5); break;

        }
    }
}
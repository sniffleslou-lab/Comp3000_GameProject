#include "controls.h"
#include "character.h"
#include "inspectionSystem.h"
#include "sceneManager.h"

void Controls::handleInput(SDL_Event &event, Character &player, inspectionSystem& inspector) {
if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT: player.move(-15, 0, inspector.getItems()); break;
            case SDLK_RIGHT: player.move(15,0, inspector.getItems()); break;
            case SDLK_UP: player.move(0,-15, inspector.getItems()); break;
            case SDLK_DOWN: player.move(0, 15, inspector.getItems()); break;



        }
    }
}
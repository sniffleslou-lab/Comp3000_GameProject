#pragma once
#include <SDL.h>
#include "character.h"

class Controls{
public:
    void handleInput(SDL_Event& event, Character& player);
};
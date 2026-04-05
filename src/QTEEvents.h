//
// Created by Lou halpin on 05/04/2026.
//

#ifndef AMIPRETTY_QTEEVENTS_H
#define AMIPRETTY_QTEEVENTS_H

#include <SDL.h>
#include <vector>

struct QTEEvents {
    std::vector<SDL_KeyCode> sequence; //key press order
    float timePerKey = 1.0f;
    float failPenalty = 0.25f;
    float successGain = 0.25f;
};

#endif //AMIPRETTY_QTEEVENTS_H
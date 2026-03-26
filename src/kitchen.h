//
// Created by Lou halpin on 03/12/2025.
//

#ifndef AMIPRETTY_KITCHEN_H
#define AMIPRETTY_KITCHEN_H
#include <SDL.h>
#include "scene.h"
#include <memory>
#include "controls.h"
#include "character.h"
#include "inspectionSystem.h"
#include "dialogueSystem.h"
#include "storyFlags.h"
#include "NPC.h"
#include <SDL_image.h>
#include  "FadeEffect.h"


class Kitchen : public Scene{
public:
    Kitchen(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~Kitchen();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer, bool debugMode) override;
    void exit() override;
    bool playerIsNearAnnaKitchen();
    bool playerIsNearGarret();
    void startPowerOutage();
    void drawCenteredText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int y);


private:
    SDL_Renderer* renderer;
    StoryFlags& storyFlags;
   //SceneManager* sceneManager = nullptr;
    std::unique_ptr<Character> player;
    Controls controls;
    std::unique_ptr<inspectionSystem> inspector;
    DialogueSystem* dialogueSystem;
    std::unique_ptr<NPC> garretNPC;
    std::unique_ptr<NPC> annaNPC;
    bool eWasDown = false;
    bool startDialogueNextFrame = false;

    FadeEffect screenFade;
    SDL_Texture* blackoutImage = nullptr;

    //chapter card
    bool showArc2Card = false;
    float arc2CardTimer = 0.0f;
    TTF_Font* arc2Font = nullptr;

    bool showCreditsCard = false;
    float creditsTimer = 0.0f;
    TTF_Font* creditsFont = nullptr;

    SDL_Texture* newsImage= nullptr;
    bool showNewsImage = false;
    float newsTimer = 0.0f;

};

#endif //AMIPRETTY_KITCHEN_H

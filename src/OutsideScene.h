//
// Created by Lou halpin on 25/03/2026.
//

#ifndef AMIPRETTY_OUTSIDESCENE_H
#define AMIPRETTY_OUTSIDESCENE_H
#include "scene.h"
#include "storyFlags.h"
#include "dialogueSystem.h"
#include "inspectionSystem.h"
#include "character.h"
#include "NPC.h"
#include "controls.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "QTEManager.h"

class OutsideScene : public Scene {
public:
    OutsideScene(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~OutsideScene();
    void enter() override;
    void handleEvents(SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer, bool debugMode) override;
    void exit() override;

    void setSceneManager(SceneManager* mgr) override { sceneManager = mgr; }

private:

    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;

    std::unique_ptr<Character> player;
    std::unique_ptr<inspectionSystem> inspector;
    std::unique_ptr<NPC> maxwellNPC;

    SDL_Renderer* renderer;
    SceneManager* sceneManager = nullptr;

    Controls controls;
    std::unique_ptr<QTEManager> qteManager;

    bool startDialogueNextFrame = false;

    //chapter 5 card
    bool showChapter5Card = false;
    float chapter5Timer = 0.0f;
    TTF_Font* chapterFont = nullptr;

    bool playerIsNearMaxwell();
    void drawCenteredText(SDL_Renderer* renderer, TTF_Font* font,
        const std::string& text, int y);
};
#endif //AMIPRETTY_OUTSIDESCENE_H
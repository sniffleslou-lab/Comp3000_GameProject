#include "MaxwellRoom.h"
#include "sceneManager.h"
#include <iostream>

MaxwellRoom::MaxwellRoom(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue)
: renderer(renderer), storyFlags(flags), dialogueSystem(dialogue) {
    inspector = std::make_unique<inspectionSystem>(renderer, storyFlags, dialogueSystem);
    qteManager = std::make_unique<QTEManager>(renderer);

    player = std::make_unique<Character>(
    renderer,"../assets/textures/Characters/playerChar.png",
    400, 300
);


}

MaxwellRoom::~MaxwellRoom() {}

void MaxwellRoom::enter() {
    std::cout << "Entering Maxwells room\n";
    maxwellNPC = std::make_unique<NPC>(
        renderer,"../assets/textures/Characters/MaxwellChar.png", 500, 200);
    //load room
    inspector->loadItems("../assets/data/MaxwellRoom.json", renderer);
    //trigger for the confrontation ARC4
    if (storyFlags.getFlag("StartMaxwellConfrontation") &&
        !storyFlags.getFlag("MaxwellStormedOut"))
    {
        dialogueSystem->startDialogue("MaxwellConfrontationScene");
        return;
    }


    // ARC2 soft intro
    if (storyFlags.getFlag("EnterMaxwellRoom") &&
        !storyFlags.getFlag("MaxwellRoomDialogueDone"))
    {
        dialogueSystem->startDialogue("MaxwellConversationScene");
        return;
    }

}
void MaxwellRoom::handleEvents(SDL_Event &e) {
//qte
    if (qteManager->isActive()) {
        qteManager->handleEvents(e);
        return;
    }

    if(dialogueSystem->choiceActive){
        if (e.type == SDL_KEYDOWN){
            if (e.key.repeat != 0) return;

            if (e.key.keysym.sym == SDLK_a){
                dialogueSystem->selectedChoice = 0;
            }
            if (e.key.keysym.sym == SDLK_d){
                dialogueSystem->selectedChoice = 1;
            }
            if (e.key.keysym.sym == SDLK_SPACE){
                Choice chosen = dialogueSystem->currentChoices[dialogueSystem->selectedChoice];
                storyFlags.setFlag(chosen.flag, true);
                dialogueSystem->choiceActive = false;
                dialogueSystem->justFinishedChoice = true;

                if(!chosen.next.empty()){
                    dialogueSystem->jumpToLine(chosen.next);
                } else {
                    dialogueSystem->nextLine();
                }
            }
        }

    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {

        if (dialogueSystem->justFinishedChoice) {
            dialogueSystem->justFinishedChoice = false;
            return;
        }

        if (!dialogueSystem->choiceActive) {
            dialogueSystem->nextLine();
        }
    }
    controls.handleInput(e, *player, *inspector);

}
void MaxwellRoom::update(float dt) {
    //qtw is active, update and pause everything
    if (qteManager->isActive()) {
        qteManager->update(dt);
        return;
    }
    //replay minigame
    if (storyFlags.getFlag("ReplayMaxwellMinigame") &&
        !qteManager->isActive()) {
        storyFlags.setFlag("ReplayMaxwellMinigame", false);
        storyFlags.setFlag("MaxwellQTEStarted", true);
        QTEEvents event;
        event.sequence = {SDLK_w, SDLK_a, SDLK_d};
        event.timePerKey = 2.0f;
        event.successGain = 0.5f;
        event.failPenalty = 0.1f;

        qteManager->start(event);
        return;
    }
    //when macwellroom dialogue ends -> it should start the qte
    if (storyFlags.getFlag("StartMaxwellMinigame")&&
        !storyFlags.getFlag("MaxwellQTEStarted")) {
        storyFlags.setFlag("MaxwellQTEStarted", true);

        QTEEvents event;
        event.sequence = {SDLK_w, SDLK_a, SDLK_d};
        event.timePerKey = 2.0f;
        event.successGain = 0.5f;
        event.failPenalty = 0.1f;

        qteManager->start(event);
        return;
    }
    //qte finished, when akward goodbye
    if (storyFlags.getFlag("MaxwellQTEStarted") &&
        qteManager->isSuccess() &&
        !storyFlags.getFlag("MaxwellPostQTEStarted"))
    {
        storyFlags.setFlag("MaxwellPostQTEStarted", true);
        dialogueSystem->startDialogue("MaxwellPostQTE");
        return;
    }

    // After awkward goodbye → return to hallway
    if (storyFlags.getFlag("MaxwellPostQTEDone") &&
        !dialogueSystem->isActive)
    {
        sceneManager->changeScene(SceneID::SCENE_HALLWAYA, renderer);
        return;
    }

    //after maxwell storms out, will auto transition back to the hallway
    if (storyFlags.getFlag("MaxwellStormedOut")&&
        !dialogueSystem->isActive) {
        sceneManager->changeScene(SceneID::SCENE_HALLWAYA, renderer);
    }
}
void MaxwellRoom::render(SDL_Renderer* renderer, bool debugMode) {

    inspector->render(renderer);
    if (maxwellNPC)maxwellNPC->draw(renderer);
    player->draw();
    dialogueSystem->render(renderer);
    qteManager->render(renderer);

}

void MaxwellRoom::exit() {
    std::cout << "Exiting Maxwells room\n";
}

//
// Created by Lou halpin on 03/12/2025.
//

#include "hallwayA.h"
#include <iostream>

HallwayA::HallwayA(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem* dialogue)
:storyFlags(flags), renderer(renderer), dialogueSystem(dialogue)
{
    player = std::make_unique<Character>(renderer,"../assets/textures/Characters/playerChar.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer, storyFlags, dialogueSystem);
    inspector->loadItems("../assets/data/hallwayA.json",renderer);


    bool unlocked = storyFlags.getFlag("AnnaUnlocked");
    bool moved = storyFlags.getFlag("AnnaMoved");
    std::cout << "DEBUG: annaUnlocked= " << unlocked << " | AnnaMoved=" << moved << "\n";

    if (unlocked && !moved){
        annaNPC = std::make_unique<NPC>(renderer, "../assets/textures/Characters/annaCha.png", 500,300);
        std::cout << "anna spawned in hallway\n";
    }

    //spawn maxwell in arc2
    if (storyFlags.getFlag("Arc2ArgumentSeen") &&
    !storyFlags.getFlag("Arc2_MaxwellSeen") &&
    !storyFlags.getFlag("Arc2Complete"))
    {
        maxwellNPC = std::make_unique<NPC>(renderer,
            "../assets/textures/Characters/maxwellChar.png", 600, 250);
        std::cout << "Maxwell spawned in hallway\n";
    }
    if (storyFlags.getFlag("Arc2Complete")) {
        maxwellNPC.reset();
    }



    //dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    //dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}

HallwayA::~HallwayA() {}

void HallwayA::enter() {
    std::cout<<"entered hallway scene\n";
    chapterFont = TTF_OpenFont("../assets/font/SunLight Dreams.otf", 48);
    //chapter 4 card
    if (storyFlags.getFlag("MaxwellStormedOut")&&
        !storyFlags.getFlag("Chapter4Started")) {
        storyFlags.setFlag("Chapter4Started", true);
        showChapter4Card = true;
        chapter4Timer = 0.0f;
    }
    //message after maxwell hang out and qte
    if (storyFlags.getFlag("MaxwellPostQTEDone") &&
    !storyFlags.getFlag("KitchenMessageSeen"))
    {
        dialogueSystem->startDialogue("PhoneMessageKitchen");
    }


    //rests diaglogue state
    queuedNPC = "";
    startDialogueNextFrame = false;
    //dialogueSystem->isActive = false;
    //dialogueSystem->choiceActive = false;
    //dialogueSystem->justFinishedChoice = false;

    inspector->doorCooldown = true;
    inspector->doorCooldownTimer = 0.0f;
}

void HallwayA::handleEvents(SDL_Event &e) {

    if(dialogueSystem->choiceActive){
        if (e.type == SDL_KEYDOWN){
            if (e.key.repeat != 0) return;
            if (e.key.keysym.sym == SDLK_a){
                dialogueSystem->selectedChoice=0;
            }

            if (e.key.keysym.sym == SDLK_d){
                dialogueSystem->selectedChoice=1;
            }
            if (e.key.keysym.sym == SDLK_SPACE){
                //NOW confirming
                Choice chosen =dialogueSystem->currentChoices[dialogueSystem->selectedChoice];
                storyFlags.setFlag(chosen.flag, true);
                dialogueSystem->choiceActive=false;
                dialogueSystem->justFinishedChoice= true;

                if(!chosen.next.empty()){
                    dialogueSystem->jumpToLine(chosen.next);
                } else{
                    dialogueSystem->nextLine();
                }
            }
        }
        return;
    }

    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_e){

        if (storyFlags.getFlag("StartMaxwellInvestigation") &&
            inspector->isNear("Spare Room Door", player->getPosition()))
        {
            std::cout << "Entering Spare Room...\n";
            sceneManager->changeScene(SceneID::SCENE_SPAREROOM, renderer);
            return;
        }
        if (inspector->isNear("Maxwells Door", player->getPosition()) &&
             storyFlags.getFlag("FoundMaxwellKey"))
        {
            storyFlags.setFlag("StartMaxwellConfrontation", true);
            std::cout << "Unlocking Maxwell's room...\n";
            sceneManager->changeScene(SceneID::SCENE_MAXWELLROOM, renderer);
            return;
        }

        inspector->inspect(player->getPosition(),*sceneManager, renderer);


    }


    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
        if (annaNPC && playerIsNearAnna()){
            queuedNPC = "Anna";
            startDialogueNextFrame = true;
            return;
        }
        if (maxwellNPC && playerIsNearMaxwell()) {
            queuedNPC = "MaxwellHall";
            startDialogueNextFrame = true;
            return;
        }
        //if(inspector->isNear("MaxwellDoor", player->getPosition())){
          // queuedNPC = "Maxwell";
            //startDialogueNextFrame = true;
            //return;
        //}
        if (inspector->isNear("Maxwells Door", player->getPosition()) &&
           !storyFlags.getFlag("FoundMaxwellKey"))
        {
            queuedNPC = "Maxwell";
            startDialogueNextFrame = true;
            return;
        }


        /*if(playerIsNearGarret()){
            queuedNPC = "Garret";
            startDialogueNextFrame = true;
            return;
        }*/
        return;
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
}

bool HallwayA::playerIsNearAnna() {
    if (!annaNPC) return false;
    SDL_Rect p = player->getPosition();
    SDL_Rect a = annaNPC->getRect();
    return SDL_HasIntersection(&p,&a);
}
bool HallwayA::playerIsNearMaxwell() {
    if (!maxwellNPC) return false;
    SDL_Rect p = player->getPosition();
    SDL_Rect m = maxwellNPC->getRect();
    return SDL_HasIntersection(&p, &m);
}

void HallwayA::update(float dt) {
    if (!inspector) {
        std::cerr << "ERROR: inspector is null in HallwayA::update\n";
        return;
    }
    if (!dialogueSystem) {
        std::cerr << "ERROR: dialogueSystem is null in HallwayA::update\n";
        return;
    }

    if (showChapter4Card) {
        chapter4Timer += dt;
        if (chapter4Timer > 3.0f) {
            showChapter4Card = false;
        }
        return;
    }

inspector->update(dt, player->getPosition());
    if (startDialogueNextFrame){
        dialogueSystem->startDialogue(queuedNPC);
        startDialogueNextFrame = false;
    }
    /*maxwell hallway moment in arc2
    if (storyFlags.getFlag("Arc2ArgumentSeen")&&
        !storyFlags.getFlag("Arc2_MaxwellSeen")&&
        inspector->isNear("Maxwell Trigger", player->getPosition())&&
        !dialogueSystem->isActive) {
        dialogueSystem->startDialogue("MaxwellHall");
        storyFlags.setFlag("Arc2_MaxwellSeen", true);
    }*/
    //max talk -> teleport into maxs room
    if (storyFlags.getFlag("MaxwellHallDone") &&
        !storyFlags.getFlag("TeleportedToMaxwellRoom"))
    {
        storyFlags.setFlag("TeleportedToMaxwellRoom", true);
        sceneManager->changeScene(SceneID::SCENE_MAXWELLROOM, renderer);

        return;
    }

    //max investigaiton in arc3

    if (storyFlags.getFlag("StartMaxwellInvestigation")&&
        !storyFlags.getFlag("ReachedMaxwellDoor")&&
        !dialogueSystem->isActive) {
        dialogueSystem->startDialogue("InvestigationScene");
    }

}
void HallwayA::render(SDL_Renderer *renderer, bool debugMode) {
    if (!inspector) {
        std::cerr << "ERROR: inspector is null in HallwayA::render\n";
        return;
    }
    if (!dialogueSystem) {
        std::cerr << "ERROR: dialogueSystem is null in HallwayA::render\n";
        return;
    }
    SDL_SetRenderDrawColor(renderer, 80,60,100,255);
    SDL_RenderClear(renderer);

    inspector->render(renderer);
    if (annaNPC) annaNPC->draw(renderer);
    if (maxwellNPC) maxwellNPC->draw(renderer);
    player->draw();
    dialogueSystem->render(renderer);


    //chapter card
    if (showChapter4Card) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0,0,0,180);
        SDL_Rect fullscreen = {0,0,1280,720};
        SDL_RenderFillRect(renderer, &fullscreen);

        drawCenteredText(renderer,chapterFont, "CHAPTER 4",300);
        drawCenteredText(renderer,chapterFont, "Outsiders",380);
    }
    //debug
    if (debugMode) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        for (auto& item : inspector->getItems()) {
            SDL_RenderDrawRect(renderer, &item.rect);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        SDL_Rect p = player->getPosition();
        SDL_RenderDrawRect(renderer, &p);
    }

}

void HallwayA::drawCenteredText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int y) {
    if (!font) return;
    SDL_Color white = {255,255,255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

    int screenW = 1280;
    int textW = surf->w;

    SDL_Rect dst;
    dst.x = (screenW - textW) / 2;
    dst.y = y;
    dst.w = surf->w;
    dst.h = surf->h;


    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

}

void HallwayA::exit() {
    std::cout<<"left hallway scene\n";
}


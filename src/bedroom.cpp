//
// Created by Lou halpin on 19/11/2025.
//
#include "bedroom.h"
#include <iostream>
#include "sceneManager.h"
#include "inspectionSystem.h"
#include <SDL_ttf.h>


bedroom::bedroom(SDL_Renderer *renderer, StoryFlags& flags, DialogueSystem* dialogue)
: storyFlags(flags), renderer(renderer), dialogueSystem(dialogue) {
    player = std::make_unique<Character>(renderer,"../assets/textures/Characters/playerChar.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer,storyFlags,dialogueSystem);
    inspector->loadItems("../assets/data/item.json",renderer);

    chapterFont = TTF_OpenFont("../assets/font/SunLight Dreams.otf", 48);
    if(!chapterFont){
        std::cout << "failed to load font: "<<TTF_GetError()<< std::endl;
    }
    qteDebug = std::make_unique<QTEManager>(renderer);

    //dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    //dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}
bedroom::~bedroom()  {}

void bedroom::enter() {


    std::cout<< "entered bedroom scene";
    inspector->doorCooldown = true;
    inspector->doorCooldownTimer = 0.0f;

    //title card
    if (!storyFlags.getFlag("SeenChapter1Card")){
        chapterNumber = 1;
        chapterSubtitle = "Unsettling";
        showChapterCard = true;
        chapterCardTimer = 0.0f;
        storyFlags.setFlag("SeenChapter1Card", true);
        return;
    }
    //chapter 3
    if (storyFlags.getFlag("Arc3Start") &&
     !storyFlags.getFlag("SeenChapter3Card")) {
        chapterNumber = 3;
        chapterSubtitle = "Reflections";
        showChapterCard = true;
        chapterCardTimer = 0.0f;
        storyFlags.setFlag("SeenChapter3Card", true);
        return;
     }
    if (storyFlags.getFlag("Arc3Start") &&
     !storyFlags.getFlag("MirrorSceneDone") &&
     !dialogueSystem->isActive)
    {
        dialogueSystem->startDialogue("PlayerThoughts_Mirror");
        return;
    }

    if (storyFlags.getFlag("MirrorSceneDone") &&
     !storyFlags.getFlag("PhoneMessageSeen") &&
     !dialogueSystem->isActive)
    {
        dialogueSystem->startDialogue("Phone");
        storyFlags.setFlag("PhoneMessageSeen", true);
    }



}
void bedroom::drawText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y) {
    SDL_Color white = {255,255,255,255};

    SDL_Surface* surf = TTF_RenderText_Blended(font,text.c_str(),white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect dst = {x,y,surf->w,surf->h};
    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}
void bedroom::drawCenteredText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int y) {
    SDL_Color white = {255,255,255,255};

    SDL_Surface* surf = TTF_RenderText_Blended(font,text.c_str(),white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

    int screenW = 1280;
    int textW = surf->w;

    SDL_Rect dst;
    dst.x = ( screenW - textW)/2;
    dst.y=y;
    dst.w = surf->w;
    dst.h = surf->h;
    SDL_RenderCopy(renderer, tex, NULL,&dst);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}
void bedroom::handleEvents(SDL_Event &e) {
    // DEBUG: Press F6 to start a random QTE test
    if (qteDebug->isActive()) {
        qteDebug->handleEvents(e);
        return;
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F6) {

        QTEEvents testEvent;

        // Random sequence length 3–5
        int len = 3 + (rand() % 3);

        for (int i = 0; i < len; i++) {
            int r = rand() % 5;
            switch (r) {
                case 0: testEvent.sequence.push_back(SDLK_w); break;
                case 1: testEvent.sequence.push_back(SDLK_a); break;
                case 2: testEvent.sequence.push_back(SDLK_s); break;
                case 3: testEvent.sequence.push_back(SDLK_d); break;
                case 4: testEvent.sequence.push_back(SDLK_SPACE); break;
            }
        }

        testEvent.timePerKey = 5.0f;
        testEvent.successGain = 0.5f;
        testEvent.failPenalty = 0.1f;

        qteDebug->start(testEvent);
        std::cout << "DEBUG: Started random QTE in bedroom\n";
    }

    if(dialogueSystem->choiceActive){
        if (e.type == SDL_KEYDOWN){
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
                dialogueSystem->currentChoices.clear();
                dialogueSystem->nextLine();
            }
        }
        return;
    }

    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_e){
        inspector->inspect(player->getPosition(),*sceneManager, renderer);
    }

    //if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
      //  dialogueSystem->startDialogue("Garret");
    //}
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE){
        dialogueSystem->nextLine();
    }
}
void bedroom::update(float dt) {

    //QTE debug s active update
    if (qteDebug->isActive()) {
        qteDebug->update(dt);
        return;
    }
    //after qte
    if (qteDebug->isSuccess()) {
        std::cout << "DEBUG: QTE success\n";
    }

    if (qteDebug->isFailure()) {
        std::cout << "DEBUG: QTE fail\n";
    }

    inspector->update(dt, player->getPosition());

    if(showChapterCard){
        chapterCardTimer += dt;
        if(chapterCardTimer > 5.0f){
            showChapterCard = false;

            if (storyFlags.getFlag("Arc3Start") &&
                !storyFlags.getFlag("MirrorSceneDone") &&
                !dialogueSystem->choiceActive)
            {
                dialogueSystem->startDialogue("PlayerThoughts_Mirror");
            }
        }
    }
}

void bedroom::render(SDL_Renderer *renderer, bool debugMode) {
    inspector->render(renderer);
    player->draw();

    dialogueSystem->render(renderer);

    //title card
    if(showChapterCard){
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,0,0,0,180);
        SDL_Rect fullscreen = {0,0,1280,720};
        SDL_RenderFillRect(renderer, &fullscreen);

        drawCenteredText(renderer, chapterFont, "CHAPTER " + std::to_string(chapterNumber), 500);
        drawCenteredText(renderer, chapterFont, chapterSubtitle, 450);

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
    qteDebug->render(renderer);
}
void bedroom::exit() {
    std::cout<<"exited bedroom scene";
}
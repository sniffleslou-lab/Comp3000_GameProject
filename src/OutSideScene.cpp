#include "OutsideScene.h"
#include <iostream>

OutsideScene::OutsideScene(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue)
: renderer(renderer), storyFlags(flags), dialogueSystem(dialogue)
{
    player = std::make_unique<Character>(
            renderer,
            "../assets/textures/Characters/playerChar.png",
            350, 400
        );
    inspector = std::make_unique<inspectionSystem>(renderer, storyFlags,dialogueSystem);
    inspector->loadItems("../assets/data/outsideScene.json", renderer);

    qteManager = std::make_unique<QTEManager>(renderer);

    //for arc4 when max appears outside storming out
    if (!storyFlags.getFlag("Chapter4_MaxwellGone")) {
        maxwellNPC = std::make_unique<NPC>(renderer,
            "../assets/textures/Characters/MaxwellChar.png",650,350);
    }
}

OutsideScene::~OutsideScene() {}

void OutsideScene::enter() {
    std::cout<<" entered outside scene\n";

    chapterFont = TTF_OpenFont("../assets/font/SunLight Dreams.otf", 48);
    //Axiety attack intro for the first time
    if (!storyFlags.getFlag("OutsideAnxietySeen")) {
        storyFlags.setFlag("OutsideAnxietySeen", true);
        dialogueSystem->startDialogue("OutsideAnxietyIntro");
        return;
    }
    //findmaxwel json
    if (!storyFlags.getFlag("Chapter4_FindMaxwellDone")) {
        storyFlags.setFlag("Chapter4_FindMaxwellDone", true);
        dialogueSystem->startDialogue("FindMaxwellScene");
    }
}

void OutsideScene::handleEvents(SDL_Event &e) {
    if (qteManager->isActive()) {
        qteManager->handleEvents(e);
        return;
    }

    if (showChapter5Card) {
        return;
    }
    if (dialogueSystem->choiceActive) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.repeat != 0) return;

            if (e.key.keysym.sym == SDLK_a)
                dialogueSystem->selectedChoice = 0;

            if (e.key.keysym.sym == SDLK_d)
                dialogueSystem->selectedChoice = 1;

            if (e.key.keysym.sym == SDLK_SPACE) {
                Choice chosen = dialogueSystem->currentChoices[dialogueSystem->selectedChoice];
                storyFlags.setFlag(chosen.flag, true);

                dialogueSystem->choiceActive = false;
                dialogueSystem->justFinishedChoice = true;

                if (!chosen.next.empty())
                    dialogueSystem->jumpToLine(chosen.next);
                else
                    dialogueSystem->nextLine();
            }
        }
        return;
    }

    // SPACE to advance dialogue
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
        if (dialogueSystem->justFinishedChoice) {
            dialogueSystem->justFinishedChoice = false;
            return;
        }
        if (!dialogueSystem->choiceActive)
            dialogueSystem->nextLine();
    }

    // Player movement + inspector
    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e) {
        inspector->inspect(player->getPosition(), *sceneManager, renderer);
    }

    // Talk to Maxwell
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
        if (maxwellNPC && playerIsNearMaxwell()) {
            dialogueSystem->startDialogue("MaxwellConversationSceneOut");
            return;
        }
    }
}

void OutsideScene::update(float dt) {
    //chapter card
    if (showChapter5Card) {
        chapter5Timer += dt;

        if (chapter5Timer > 3.0f) {
            showChapter5Card = false;
        }
        return;
    }
inspector->update(dt, player->getPosition());



    if (storyFlags.getFlag("StartAnxietyQTE") &&
       !storyFlags.getFlag("AnxietyQTE_Started"))
    {
        storyFlags.setFlag("AnxietyQTE_Started", true);

        QTEEvents event;
        event.sequence = {SDLK_a, SDLK_s};
        event.timePerKey = 3.5f;
        event.successGain = 1.4f;
        event.failPenalty = 0.0f;

        qteManager->start(event);
        return;
    }
    //aftert axienty qte
    if (storyFlags.getFlag("AnxietyQTE_Started") &&
       qteManager->isSuccess() &&
       !storyFlags.getFlag("Chapter4_FindMaxwellDone"))
    {
        storyFlags.setFlag("Chapter4_FindMaxwellDone", true);
        dialogueSystem->startDialogue("FindMaxwellScene");
        return;
    }
    //after findmax scene converstation scene start
    if (storyFlags.getFlag("Chapter4_FindMaxwellDone")&&
    !storyFlags.getFlag("Chapter4_ConversationDone") &&
    !dialogueSystem->isActive)
        {
        storyFlags.setFlag("Chapter4_ConversationDone", true);
        dialogueSystem->startDialogue("MaxwellConversationSceneOut");
        return;
        }

    //chapter 5 card
    if (storyFlags.getFlag("Chapter4_ConversationDone")&&
        !showChapter5Card &&
        !dialogueSystem->isActive) {
        showChapter5Card = true;
        chapter5Timer = 0.0f;
        }
}

void OutsideScene::exit() {
    std::cout<<" exited OutsideScene\n";
}

bool OutsideScene::playerIsNearMaxwell() {
    if (!maxwellNPC) return false;
    SDL_Rect p = player->getPosition();
    SDL_Rect m = maxwellNPC->getRect();
    return SDL_HasIntersection(&p, &m);
}

void OutsideScene::drawCenteredText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int y) {
    SDL_Color white = {255,255,255,255};
    SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

    int w = surf->w;
    int h = surf->h;
    SDL_Rect dst = {(800 - w )/ 2, y, w, h};

    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void OutsideScene::render(SDL_Renderer *renderer, bool debugMode) {
    SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
    SDL_RenderClear(renderer);
    inspector->render(renderer);

    if (maxwellNPC) maxwellNPC->draw(renderer);
    player->draw();
    dialogueSystem->render(renderer);
    qteManager->render(renderer);

    if (showChapter5Card) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_Rect fullscreen = {0,0,800,600};
        SDL_RenderFillRect(renderer, &fullscreen);

        drawCenteredText(renderer, chapterFont, "CHAPTER 5",300);
        drawCenteredText(renderer, chapterFont, "Honestly",380);
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

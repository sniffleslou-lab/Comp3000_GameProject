//
// Created by Lou halpin on 03/12/2025.
//
#include "kitchen.h"
#include <iostream>

Kitchen::Kitchen(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem* dialogue)
:storyFlags(flags), renderer(renderer), dialogueSystem(dialogue) {
    player = std::make_unique<Character>(renderer,"../assets/textures/Characters/playerChar.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer,storyFlags,dialogueSystem);
    inspector->loadItems("../assets/data/kitchen.json",renderer);

    garretNPC = std::make_unique<NPC>(renderer, "../assets/textures/Characters/garretCha.png",300,200);

    if(storyFlags.getFlag("AnnaMoved")){
        std::cout << "Anna moved flag detected — spawning Anna in kitchen.\n";
        annaNPC = std::make_unique<NPC>(renderer, "../assets/textures/Characters/annaCha.png",350,200);
    }


    //dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    //dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}

Kitchen::~Kitchen() {}
void Kitchen::enter() {
    std::cout<< "entered kitchen scene";
   // dialogueSystem->startDialogue("Garret");
    inspector->doorCooldown = true;
    inspector->doorCooldownTimer = 0.0f;

    //find couch
    SDL_Rect couchRect= {0,0,0,0};
    for (auto& item : inspector->getItems()) {
        if (item.name == "Couch") {
            couchRect = item.rect;
            break;
        }
    }
    if (garretNPC) {
        int garretX = couchRect.x + couchRect.w/2-32;
        int garretY = couchRect.y - 30;
        garretNPC->setPosition(garretX,garretY);
    }

    if (storyFlags.getFlag("AnnaMoved")&& !annaNPC) {
        std::cout << "anna spawned in the room\n";
        int annaX = couchRect.x + couchRect.w - 40;
        int annaY = couchRect.y - 40;

        annaNPC = std::make_unique<NPC>(
            renderer,
            "../assets/textures/Characters/annaCha.png",
            annaX,
            annaY
        );    }
    if (!storyFlags.getFlag("AnnaMoved")) {
        startDialogueNextFrame = true;
    } else {
        startDialogueNextFrame = false;
    }

   // startDialogueNextFrame = true;
    arc2Font = TTF_OpenFont("../assets/font/SunLight Dreams.otf", 48);
    creditsFont = TTF_OpenFont("../assets/font/SunLight Dreams.otf", 48);


}

bool Kitchen::playerIsNearGarret() {
    SDL_Rect p = player->getPosition();
    SDL_Rect g = garretNPC->getRect();
    return SDL_HasIntersection(&p,&g);
}
bool Kitchen::playerIsNearAnnaKitchen() {
    if(!annaNPC) return false;
    SDL_Rect p = player->getPosition();
    SDL_Rect g = annaNPC->getRect();
    return SDL_HasIntersection(&p,&g);

}

void Kitchen::handleEvents(SDL_Event &e) {


    if (dialogueSystem->choiceActive) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.repeat != 0) return;

            if (e.key.keysym.sym == SDLK_a) {
                dialogueSystem->selectedChoice = 0;
            }

            if (e.key.keysym.sym == SDLK_d) {
                dialogueSystem->selectedChoice = 1;
            }
            if (e.key.keysym.sym == SDLK_SPACE) {
                std::cout << "SPACE pressed while choiceActive = "
                          << dialogueSystem->choiceActive << "\n";
                Choice chosen = dialogueSystem->currentChoices[dialogueSystem->selectedChoice];

                storyFlags.setFlag(chosen.flag, true);

                dialogueSystem->choiceActive = false;
                dialogueSystem->justFinishedChoice = true;
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

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e) {

        if(playerIsNearGarret()){
            dialogueSystem->startDialogue("Garret");
            return;
        }
        if(playerIsNearAnnaKitchen()){
            dialogueSystem->startDialogue("AnnaKitchen");
            return;
        }
        inspector->inspect(player->getPosition(), *sceneManager, renderer);
        return;
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
        dialogueSystem->startDialogue("Garret");
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
void Kitchen::drawCenteredText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int y) {
    SDL_Color white = {255,255,255,255};

    SDL_Surface* surf = TTF_RenderText_Blended(font,text.c_str(),white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    int w = surf->w;
    int h = surf->h;

    SDL_Rect dst = {(1280-w)/2,y,w,h};
    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}


void Kitchen::update(float dt) {
inspector->update(dt, player->getPosition());
   // inspector->inspect(player->getPosition(), *sceneManager, renderer);

    //garrets dialogue start when the player walks into the kitchen
/*
    if (startDialogueNextFrame){
        dialogueSystem->choiceActive = false;
        dialogueSystem->justFinishedChoice = false;
        dialogueSystem-> selectedChoice = 0;
       dialogueSystem->startDialogue("Garret");
        startDialogueNextFrame = false;
    }*/
    if (startDialogueNextFrame &&
    !storyFlags.getFlag("Arc1_Done") &&
    !storyFlags.getFlag("AnnaMoved"))
    {
        dialogueSystem->choiceActive = false;
        dialogueSystem->justFinishedChoice = false;
        dialogueSystem->selectedChoice = 0;
        dialogueSystem->startDialogue("Garret");
        startDialogueNextFrame = false;
    }

    //arc 1  ending
    if (storyFlags.getFlag("Arc1_Done")&&
        !storyFlags.getFlag("Arc1_TvTransition")&&
        !dialogueSystem->isActive)
        {
        dialogueSystem->startDialogue("LivingRoom_Arc1End");
        }

    //arc2 start trigger
    if (storyFlags.getFlag("Arc2_PowerOutage_Start") &&
        !storyFlags.getFlag("Arc2_PowerOutage_Triggered"))
    {
        startPowerOutage();
        storyFlags.setFlag("Arc2_PowerOutage_Triggered", true);
    }
    //arc2 post maxwell hallway moment//
    if (storyFlags.getFlag("Arc2_MaxwellSeen")&&
        !storyFlags.getFlag("Arc2Complete")&&
        !dialogueSystem->isActive) {
        dialogueSystem->startDialogue("PostMaxGroupArc2");

    }
    //tranisition to chapter 3
    if (storyFlags.getFlag("Arc2Complete")&&
        !dialogueSystem->isActive) {
        sceneManager->changeScene(SceneID::SCENE_BEDROOM, renderer);
    }
    /*
    if (storyFlags.getFlag("Arc2_PowerOutage_Start")&& !storyFlags.getFlag("Arc2_PowerOutage_Triggered"))
        {
        startPowerOutage();
        storyFlags.setFlag("Arc2_PowerOutage_Triggered", true);
    }*/
    //arc2 scene featuring the argument
    if (storyFlags.getFlag("BreakerFixed")&&
        !storyFlags.getFlag("Arc2ArgumentSeen")&&
        !dialogueSystem->isActive) {

        dialogueSystem->startDialogue("GroupArc2");
    }
    screenFade.update(dt);
    if (storyFlags.getFlag("Arc2_PowerOutage_Triggered")&& !showArc2Card) {
        showArc2Card = true;
        arc2CardTimer = 0.0f;
    }
    if (showArc2Card) {
        arc2CardTimer += dt;

        if (arc2CardTimer > 3.0f && !showNewsImage) {
            newsImage = IMG_LoadTexture(renderer, "../assets/textures/breakingnews.png");
            showNewsImage = true;
            newsTimer = 0.0f;
        }
    }
    if (showNewsImage) {
        newsTimer += dt;
        if (newsTimer > 3.0f) {
            showNewsImage = false;
            SDL_DestroyTexture(newsImage);
            newsImage = nullptr;
        }
    }
    //arc3 garret living room talk
    if (storyFlags.getFlag("CalledToLivingRoom") &&
        !storyFlags.getFlag("GarretTalkDone") &&
            !dialogueSystem->isActive) {
        dialogueSystem->startDialogue("CalledToLivingRoom");
    }
    //arc3 anna storms in
    if (storyFlags.getFlag("GarretTalkDone")&&
        !storyFlags.getFlag("StartMaxwellInvestigation")&&
        !dialogueSystem->isActive) {
        dialogueSystem->startDialogue("AnnaInterruptScene");
    }
    //arc 5
    if (storyFlags.getFlag("Arc5_FinalTalk")&&
        !storyFlags.getFlag("Arc5_FinalTalk_Done") &&
        !dialogueSystem->isActive)
        {
        dialogueSystem->startDialogue("StartArc5_FinalTalk");
        storyFlags.setFlag("Arc5_FinalTalk_Done", true);
        }
    if (storyFlags.getFlag("StartCredits")&&
        !showCreditsCard)
        {
        showCreditsCard = true;
        creditsTimer = 0.0f;
        }
    if (showCreditsCard) {
        creditsTimer += dt;
        if (creditsTimer > 3.0f) {
            sceneManager->changeScene(SceneID::SCENE_START, renderer);
        }
        return;
    }
    if (storyFlags.getFlag("Arc2Complete") &&
    !dialogueSystem->isActive) {

        storyFlags.setFlag("Arc3Start", true);
        sceneManager->changeScene(SceneID::SCENE_BEDROOM, renderer);
    }

/*
    if(!dialogueSystem->choiceActive) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        bool eDown = keys[SDL_SCANCODE_E];
        if (eDown && !eWasDown) {
            dialogueSystem->nextLine();
        }
        eWasDown = eDown;
    }*/

}

void Kitchen::startPowerOutage() {
    screenFade.start(0,0,0,255,2.0f);
    blackoutImage = IMG_LoadTexture(renderer,"../assets/textures/wall.png");
    //add sound
}

void Kitchen::render(SDL_Renderer *renderer, bool debugMode) {


    SDL_SetRenderDrawColor(renderer, 253,253,100,255);
    SDL_RenderClear(renderer);

    inspector->render(renderer);
    garretNPC->draw(renderer);

    if (annaNPC) annaNPC->draw(renderer);

    player->draw();
    dialogueSystem->render(renderer);

    screenFade.render(renderer);

    if (storyFlags.getFlag("Arc2_PowerOutage_Triggered")) {
        SDL_RenderCopy(renderer,blackoutImage, NULL, NULL);
    }

    if (showArc2Card ) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0,0,0,180);
        SDL_Rect fullscreen = {0,0,1280,720};
        SDL_RenderFillRect(renderer, &fullscreen);

        drawCenteredText(renderer, arc2Font, "CHAPTER 2", 500);
        drawCenteredText(renderer,arc2Font, "Static", 450);
    }
    if (showNewsImage && newsImage) {
        SDL_RenderCopy(renderer, newsImage,NULL,NULL);
    }

    if (showCreditsCard) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_Rect fullscreen = {0,0,1280,720};
        SDL_RenderFillRect(renderer, &fullscreen);

        drawCenteredText(renderer, creditsFont, "Made by Lou", 300);
        drawCenteredText(renderer, creditsFont, "Thanks for playing!", 380);
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

void Kitchen::exit() {
    std::cout<< "left kitchen scene";

}
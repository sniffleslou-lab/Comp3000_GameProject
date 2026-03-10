//
// Created by Lou halpin on 17/11/2025.
//
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "dialogueSystem.h"
#include "filesystem"
#include "sstream"


DialogueSystem::DialogueSystem(StoryFlags &flags, SDL_Renderer* renderer)
: storyFlags(flags), renderer(renderer) {
    font = TTF_OpenFont("../assets/font/Sunlight Dreams.otf",24);
    if(!font){
        std::cerr << "failed to load font: " << TTF_GetError()<<std::endl;
    }

    //portrait loader
    portraitMap["Garret"] = IMG_LoadTexture(renderer,"../assets/textures/portraits/garret.png");
    portraitMap["Anna"] = IMG_LoadTexture(renderer,"../assets/textures/portraits/anna.png");
    portraitMap["Maxwell"] = IMG_LoadTexture(renderer,"../assets/textures/portraits/maxwell.png");
    portraitMap["player"] = IMG_LoadTexture(renderer,"../assets/textures/portraits/player.png");

    for (auto& p : portraitMap){
        if(!p.second){
            std::cerr << "failed to load portrait for " << p.first<<std::endl;
        }
    }


}

void DialogueSystem::loadDialogueFile(const std::string &jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()){
        std::cerr << "could not open " << jsonPath;
        return;
    }
    nlohmann::json data;
    file >> data;
    std::cout<<"JSON File \" " << jsonPath << "\" is loaded. \n";

    NPCDialogue npc;
    npc.npcId = data["npc"];


    for (auto& lineJson : data["dialogue"]){
        dialogueLine line;
        line.id = lineJson["id"];
        line.text = lineJson["text"];
        if (lineJson.contains("condition")){
            line.condition = lineJson["condition"];
        }

        if(lineJson.contains("choices")){
            for (auto& c: lineJson["choices"]){
                Choice choice;
                choice.text = c["text"];
                choice.flag = c["flag"];
                if(c.contains("next")) choice.next = c["next"];
                line.choices.push_back(choice);
            }
        }
        npc.lines.push_back(line);
    }
    npcs.push_back(npc);
}
//this checks that it can load the file. going to load the files in main
void DialogueSystem::loadAllDialogue(const std::string &folderPath) {
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)){
        if (entry.path().extension()==".json"){
            loadDialogueFile(entry.path().string());
        }
    }
}
void DialogueSystem::startDialogue(const std::string &npcId) {
    currentLines.clear();
    currentIndex = 0;
    isActive = true;

    for (auto& npc : npcs) {
        if(npc.npcId == npcId){
            ///portrait
            if(portraitMap.find(npcId)!= portraitMap.end()){
                currentPortrait = portraitMap[npcId];
            }else  {
                currentPortrait = nullptr;
            }
            ///lines
            for (auto& line : npc.lines){
                if (evaluteCondition(line.condition)){
                    currentLines.push_back(line);
                }
            }
            break;
        }

    }
    std::cout << "Loaded lines for Garret:\n";
    for (auto& line : currentLines) { std::cout << " " << line.id << " (choices=" << line.choices.size() << ")\n";}

    if (currentLines.empty()){
        isActive = false;
    }
}
void DialogueSystem::nextLine() {
    if (!isActive) return;

    currentChoices.clear();

    if(justFinishedChoice){
        justFinishedChoice = false;
        if (currentIndex + 1 < currentLines.size()){
            currentIndex++;
        }else{
            endDialogue();
            return;
        }
    }
    if (currentIndex >= currentLines.size()){
        endDialogue();
        return;
    }
   const dialogueLine& line = currentLines[currentIndex];

    //quest trigger for garret
    if (line.id == "g5"){
        storyFlags.setFlag("Quest_Batteries", true);
        std::cout << "Quest started: Batteries\n";

    }
    if(!line.choices.empty()){
        choiceActive = true;
        selectedChoice = 0;
        currentChoices = line.choices;
        return;
    }

    if (currentIndex + 1 < currentLines.size()){
        currentIndex++;
    } else{
        endDialogue();
    }
}
void DialogueSystem::endDialogue() {
    isActive = false;
    currentLines.clear();
    currentIndex = 0;
}
void DialogueSystem::jumpToLine(const std::string &id) {
    for (int i = 0; i < currentLines.size(); i++){
        if (currentLines[i].id == id ){
            currentIndex = i;
            return;
        }
    }
}

std::vector<std::string> DialogueSystem::wrapText(const std::string &text, int maxWidth) {
    std::vector<std::string>lines;
    std::string current;
    std::istringstream words(text);
    std::string word;

    while (words >> word) {
        std::string test = current + (current.empty() ? "" : " ")+word;

        int w;
        TTF_SizeText(font, test.c_str(), &w, nullptr);
        if(w > maxWidth){
            lines.push_back(current);
            current = word;
        }else {
            current = test;
        }
    }
    if (!current.empty()){
        lines.push_back(current);
    }
    return lines;
}
void DialogueSystem::renderText(SDL_Renderer *renderer, const std::string &text, int x, int y) {
    SDL_Color white = {255,255,255,255};

    SDL_Surface* surface = TTF_RenderText_Blended(font,text.c_str(), white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x,y,surface->w, surface->h};
    SDL_RenderCopy(renderer,texture, nullptr,&dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
bool  DialogueSystem::evaluteCondition(const std::string &cond) {
    if (cond.empty())return true;

    std::string flag;
    std::string op;
    std::string value;

    std::stringstream ss(cond);
    ss >> flag >> op >> value;

    if (op != "==") return true;

    bool flagValue = storyFlags.getFlag(flag);
    bool expected = (value == "true");

    return flagValue == expected;
}


void DialogueSystem::render(SDL_Renderer *renderer) {

    if (!isActive || currentLines.empty())
        return;

    if(choiceActive) {

        SDL_Rect left = {250,500,200,60};
        SDL_Rect right = {450,500,200,60};

        //left button
        SDL_SetRenderDrawColor(renderer,
                               selectedChoice == 0 ? 200 : 80,
                               selectedChoice == 0 ? 200 : 80,
                               selectedChoice == 0 ? 200 : 80,
                               255);
        SDL_RenderFillRect(renderer, &left);

        //right
        SDL_SetRenderDrawColor(renderer,
                               selectedChoice == 1 ? 200 : 80,
                               selectedChoice == 1 ? 200 : 80,
                               selectedChoice == 1 ? 200 : 80,
                               255);
        SDL_RenderFillRect(renderer, &right);

        renderText(renderer, currentChoices[0].text,left.x + 20, left.y + 15);
        renderText(renderer, currentChoices[1].text,right.x + 20, right.y + 15);
        return;
    }
    //portrait
    if (currentPortrait){
        SDL_Rect portraitRect = {800,350,512,512};
        SDL_RenderCopy(renderer, currentPortrait, NULL, &portraitRect);
    }
    //dialogue
    SDL_Rect box = {50,400,700,150};
    SDL_SetRenderDrawColor(renderer, 0,0,0,200);
    SDL_RenderFillRect(renderer, &box);

    //wrap text
    const dialogueLine& line = currentLines[currentIndex];
    SDL_Color white = {255,255,255,255};

    auto wrappedLines = wrapText(line.text, 650);
    int y = 420;

    for (const auto& l : wrappedLines){
        SDL_Surface* surface = TTF_RenderText_Blended(font, l.c_str(),white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = {70,y,surface->w,surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dst);

        y += surface->h + 5;

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

}


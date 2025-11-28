//
// Created by Lou halpin on 28/11/2025.
//

#ifndef AMIPRETTY_STORYFLAGS_H
#define AMIPRETTY_STORYFLAGS_H
#include <string>
#include <unordered_map>


//for flags that will progress the game

class StoryFlags{
public:
    StoryFlags() = default;

    //sets
    void setFlag(const std::string& flag, bool value);
    //checks if true/false
    bool getFlag(const std::string& flag) const;
    //removes
    void clearFlag(const std::string& flag);
    //testing
    void debugPrint()const;
private:
    std::unordered_map<std::string, bool> flags;
};
#endif //AMIPRETTY_STORYFLAGS_H

//
// Created by Lou halpin on 28/11/2025.
//
#include <iostream>

#include "storyFlags.h"
#include <iostream>
void StoryFlags::setFlag(const std::string &flag, bool value) {
    flags[flag] = value;
}
bool StoryFlags::getFlag(const std::string &flag) const {
    auto it = flags.find(flag);
    return (it != flags.end()) ? it->second : false;
}
void StoryFlags::clearFlag(const std::string &flag) {
    flags.erase(flag);
}
void StoryFlags::debugPrint() const {
    std::cout<<"Story Flags:\n";
    for (const auto& [key, value] : flags) {
        std::cout << " " << key << " = " << (value ? "true" : "false");

    }
}
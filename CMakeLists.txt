cmake_minimum_required(VERSION 3.15)

#project
project(amIpretty LANGUAGES CXX)
#c++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/controls.cpp
        src/game.h
        src/game.cpp
        src/controls.h
        src/character.h
        src/character.cpp
        src/SceneManager.cpp
        src/InspectionSystem.cpp
        src/DialogueSystem.cpp
        src/sceneManager.h
        src/startScene.cpp
        src/StartScene.h
        src/scene.h
        src/bedroom.h
        src/bedroom.cpp)

#packages and links
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(amIpretty PRIVATE
        SDL2::SDL2
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
        nlohmann_json::nlohmann_json
)

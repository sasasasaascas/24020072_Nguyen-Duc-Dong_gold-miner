#include "constants.h"
#include "globals.h"
#include "game.h"
#include "utils.h"
#include <iostream>

int score = 0;
float angle = 0.0f;
float hookDirectionX = 0.0f;
float hookDirectionY = 0.0f;
bool isShooting = false;
bool isRetracting = false;
bool gameEnded = false;
bool replayScreenActive = false;
float rotationDirection = 1.0f;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
SDL_Texture* background = nullptr;
SDL_Texture* gold = nullptr;
SDL_Texture* rock = nullptr;
SDL_Texture* hook = nullptr;
SDL_Texture* character = nullptr;
SDL_Texture* startScreen = nullptr;
SDL_Texture* endScreen = nullptr;
SDL_Texture* replayScreen = nullptr;
TTF_Font* font = nullptr;
SDL_Color textColor = {255, 255, 255};
double currentFrame = 0.0;
Uint32 lastFrameTime = 0;
std::vector<Object> golds(Golds);
std::vector<Object> rocks(Rocks);
Object hookObj;
Object* caughtObj = nullptr;

int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Failed to initialize!\n";
        return -1;
    }
    background = loadTexture("image/background.png");
    character = loadTexture("image/character1_2.png");
    hook = loadTexture("image/hook.png");
    gold = loadTexture("image/gold.png");
    rock = loadTexture("image/rock.png");
    startScreen = loadTexture("image/startscreen.png");
    endScreen = loadTexture("image/endscreen.png");
    replayScreen = loadTexture("image/replayscreen.png");
    if (!background || !character || !hook || !gold || !rock || !startScreen || !endScreen || !replayScreen) {
        std::cerr << "Failed to load textures!\n";
        close();
        return -1;
    }
    showStartScreen();
    setupObjects();
    runGame();
    close();
    return 0;
}

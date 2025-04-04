#include "game.h"
#include "globals.h"
#include "utils.h"
#include "constants.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! SDL ERROR: ", TTF_GetError());
        return false;
    }
    window = SDL_CreateWindow("Gold Miner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! IMG Error: " << IMG_GetError() << std::endl;
        return false;
    }
    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 28);
    if (!font) {
        SDL_Log("Cannot upload the font", TTF_GetError());
        return false;
    }
    return true;
}

void close() {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(gold);
    SDL_DestroyTexture(rock);
    SDL_DestroyTexture(hook);
    SDL_DestroyTexture(character);
    SDL_DestroyTexture(endScreen);
    SDL_DestroyTexture(replayScreen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(startScreen);
    renderer = nullptr;
    window = nullptr;
    IMG_Quit();
    SDL_Quit();
}

void setupObjects() {
    srand(time(0));
    for (int i = 0; i < Golds; ++i) {
        bool valid = false;
        while (!valid) {
            int k = rand() % 40;
            int t = 80 + k;
            int m = (t < 90) ? 58 : (t < 100) ? 79 : (t < 110) ? 91 : 104;
            golds[i] = {rand() % (int)(SCREEN_WIDTH - 64.0), rand() % (int)(SCREEN_HEIGHT - 164.0) + 200.0, 80.0 + k, 80.0 + k, false, m};
            valid = true;
            if (golds[i].x < 0.0 || golds[i].x + golds[i].width > SCREEN_WIDTH || golds[i].y < 200.0 || golds[i].y + golds[i].height > SCREEN_HEIGHT) {
                valid = false;
                continue;
            }
            for (int j = 0; j < i; ++j) {
                if (CheckCollision(golds[i], golds[j]) || MinDistance(golds[i], golds[j])) {
                    valid = false;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < Rocks; ++i) {
        bool valid = false;
        while (!valid) {
            int m = rand() % 40;
            rocks[i] = {rand() % (int)(SCREEN_WIDTH - 64.0), rand() % (int)(SCREEN_HEIGHT - 164.0) + 200.0, 64.0 + m, 64.0 + m, false, rand() % 4 + 2};
            valid = true;
            if (rocks[i].x < 0.0 || rocks[i].x + rocks[i].width > SCREEN_WIDTH || rocks[i].y < 200.0 || rocks[i].y + rocks[i].height > SCREEN_HEIGHT) {
                valid = false;
                continue;
            }
            for (int j = 0; j < Golds; ++j) {
                if (CheckCollision(rocks[i], golds[j]) || MinDistance(rocks[i], golds[j])) {
                    valid = false;
                    break;
                }
            }
            for (int j = 0; j < i; ++j) {
                if (CheckCollision(rocks[i], rocks[j]) || MinDistance(rocks[i], rocks[j])) {
                    valid = false;
                    break;
                }
            }
        }
    }
    hookObj = {SCREEN_WIDTH / 2.0, 100.0, 32.0, 32.0, false};
}

void renderCharacter() {
    SDL_Rect dstRect = {(int)(SCREEN_WIDTH / 2.0 - HOOK_WIDTH / 2.0), 0, (int)HOOK_WIDTH - 15, (int)HOOK_HEIGHT - 15};
    SDL_RenderCopy(renderer, character, nullptr, &dstRect);
}

void renderObjects() {
    if (!gameEnded) {
        renderCharacter();
        for (const auto& goldObj : golds) {
            if (!goldObj.isCaught) {
                SDL_Rect goldRect = {(int)goldObj.x, (int)goldObj.y, (int)goldObj.width, (int)goldObj.height};
                SDL_RenderCopy(renderer, gold, nullptr, &goldRect);
            }
        }
        for (const auto& rockObj : rocks) {
            if (!rockObj.isCaught) {
                SDL_Rect rockRect = {(int)rockObj.x, (int)rockObj.y, (int)rockObj.width, (int)rockObj.height};
                SDL_RenderCopy(renderer, rock, nullptr, &rockRect);
            }
        }
    }
}

void renderHook() {
    if (!gameEnded) {
        SDL_Rect hookRect = {(int)hookObj.x, (int)hookObj.y, (int)hookObj.width + 7, (int)hookObj.height + 7};
        SDL_RenderCopy(renderer, hook, nullptr, &hookRect);
    }
}

void drawLineToHook() {
    if (!gameEnded) {
        int centerX = (int)(SCREEN_WIDTH / 2.0);
        int centerY = 50 + (int)(HOOK_HEIGHT / 2.0);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, centerX, centerY, (int)(hookObj.x + hookObj.width / 2.0), (int)(hookObj.y + hookObj.height / 2.0));
    }
}

bool allObjectsHooked() {
    for (const auto& goldObj : golds) {
        if (!goldObj.isCaught) return false;
    }
    for (const auto& rockObj : rocks) {
        if (!rockObj.isCaught) return false;
    }
    return true;
}

void retractHook() {
    double targetX = SCREEN_WIDTH / 2.0;
    double targetY = 100.0;
    double deltaX = targetX - hookObj.x;
    double deltaY = targetY - hookObj.y;
    double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    if (distance > HOOK_SPEED) {
        hookObj.x += deltaX / distance * HOOK_SPEED;
        hookObj.y += deltaY / distance * HOOK_SPEED;
        if (caughtObj) {
            caughtObj->x -= deltaX / distance * HOOK_SPEED;
            caughtObj->y -= deltaY / distance * HOOK_SPEED;
        }
    } else {
        hookObj.x = targetX;
        hookObj.y = targetY;
        isRetracting = false;
        isShooting = false;
        if (caughtObj) {
            score += caughtObj->scores;
            caughtObj->isCaught = true;
            caughtObj = nullptr;
        }
        if (allObjectsHooked()) {
            gameEnded = true;
            replayScreenActive = true;
        }
    }
}

void updateHook() {
    if (!isShooting) {
        hookObj.x = SCREEN_WIDTH / 2.0 + cos(angle) * 50.0;
        hookObj.y = 50.0 + HOOK_HEIGHT / 2.0 + sin(angle) * 50.0;
        angle += rotationDirection * ROTATION_SPEED;
        if (angle > MAX_ANGLE || angle < MIN_ANGLE) {
            rotationDirection = -rotationDirection;
        }
    } else if (isRetracting) {
        retractHook();
    } else {
        hookObj.x += hookDirectionX * HOOK_SPEED;
        hookObj.y += hookDirectionY * HOOK_SPEED;
        for (auto& goldObj : golds) {
            SDL_Rect hookRect = {(int)hookObj.x, (int)hookObj.y, (int)hookObj.width, (int)hookObj.height};
            SDL_Rect goldRect = {(int)goldObj.x, (int)goldObj.y, (int)goldObj.width, (int)goldObj.height};
            if (!goldObj.isCaught && SDL_HasIntersection(&hookRect, &goldRect)) {
                goldObj.isCaught = true;
                caughtObj = &goldObj;
                isRetracting = true;
                return;
            }
        }
        for (auto& rockObj : rocks) {
            SDL_Rect hookRect = {(int)hookObj.x, (int)hookObj.y, (int)hookObj.width, (int)hookObj.height};
            SDL_Rect rockRect = {(int)rockObj.x, (int)rockObj.y, (int)rockObj.width, (int)rockObj.height};
            if (!rockObj.isCaught && SDL_HasIntersection(&hookRect, &rockRect)) {
                rockObj.isCaught = true;
                caughtObj = &rockObj;
                isRetracting = true;
                return;
            }
        }
        if (hookObj.x > SCREEN_WIDTH || hookObj.x < 0.0 || hookObj.y > SCREEN_HEIGHT || hookObj.y < 0.0) {
            isRetracting = true;
        }
    }
}

void runGame() {
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE && !isShooting && !gameEnded) {
                    isShooting = true;
                    hookDirectionX = cos(angle);
                    hookDirectionY = sin(angle);
                }
                if (gameEnded && e.key.keysym.sym == SDLK_r) {
                    setupObjects();
                    isShooting = false;
                    isRetracting = false;
                    gameEnded = false;
                    replayScreenActive = false;
                }
                else if (gameEnded) {
                    SDL_RenderCopy(renderer, endScreen, nullptr, nullptr);
                    SDL_RenderPresent(renderer);
                    waitUntilKeyPressed();
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, nullptr, nullptr);
        if (gameEnded) {
            if (replayScreenActive) {
                SDL_RenderCopy(renderer, replayScreen, nullptr, nullptr);
            } else {
                SDL_RenderCopy(renderer, endScreen, nullptr, nullptr);
            }
        } else {
            updateHook();
            drawLineToHook();
            renderHook();
            renderScore(score);
            renderObjects();
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }
}

void showStartScreen() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, startScreen, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    waitUntilKeyPressed();
}

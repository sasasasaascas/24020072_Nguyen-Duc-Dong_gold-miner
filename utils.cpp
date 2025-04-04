#include "constants.h"
#include "utils.h"
#include "globals.h"
#include <iostream>
#include <cmath>
void waitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}

SDL_Texture* renderText(const std::string& text) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!surface) {
        SDL_Log("Lỗi tạo surface từ text! Lỗi: %s", TTF_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Lỗi tạo texture từ surface! Lỗi: %s", SDL_GetError());
    }
    return texture;
}

void renderScore(int score) {
    SDL_Texture* textTexture = renderText("Score: " + std::to_string(score));
    if (textTexture) {
        SDL_Rect textRect = {10, 10, 100, 50};
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }
}

SDL_Texture* loadTexture(const std::string& path) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (!newTexture) {
        std::cerr << "Failed to load texture! SDL Error: " << SDL_GetError() << std::endl;
    }
    return newTexture;
}

bool CheckCollision(const Object& object1, const Object& object2) {
    return (object1.x + object1.width > object2.x &&
            object1.x < object2.x + object2.width &&
            object1.y + object1.height > object2.y &&
            object1.y < object2.y + object2.height);
}

bool MinDistance(const Object& a, const Object& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double distance = sqrt(dx * dx + dy * dy);
    return distance < MIN_DISTANCE;
}

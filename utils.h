#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "object.h"

void waitUntilKeyPressed();
SDL_Texture* renderText(const std::string& text);
void renderScore(int score);
SDL_Texture* loadTexture(const std::string& path);
bool CheckCollision(const Object& object1, const Object& object2);
bool MinDistance(const Object& a, const Object& b);

#endif

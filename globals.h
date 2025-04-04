
#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "object.h"

extern int score;
extern float angle;
extern float hookDirectionX;
extern float hookDirectionY;
extern bool isShooting;
extern bool isRetracting;
extern bool gameEnded;
extern bool replayScreenActive;
extern float rotationDirection;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* texture;
extern SDL_Texture* background;
extern SDL_Texture* gold;
extern SDL_Texture* rock;
extern SDL_Texture* hook;
extern SDL_Texture* character;
extern SDL_Texture* startScreen;
extern SDL_Texture* endScreen;
extern SDL_Texture* replayScreen;
extern TTF_Font* font;
extern SDL_Color textColor;
extern double currentFrame;
extern Uint32 lastFrameTime;
extern std::vector<Object> golds;
extern std::vector<Object> rocks;
extern Object hookObj;
extern Object* caughtObj;

#endif

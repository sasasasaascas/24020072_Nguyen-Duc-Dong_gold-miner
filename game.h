
#ifndef GAME_H
#define GAME_H

#include "object.h"

bool init();
void close();
void setupObjects();
void renderCharacter();
void renderObjects();
void renderHook();
void drawLineToHook();
bool allObjectsHooked();
void retractHook();
void updateHook();
void runGame();
void showStartScreen();

#endif

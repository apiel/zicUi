#ifndef _UI_STATE_H_
#define _UI_STATE_H_

#include "def.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

bool needToDraw = false;

// TODO make this configurable 
uint8_t encoderCount = 4;
int8_t group = 0;

void setGroup(int8_t index)
{
    group = index;
}

void resetGroup()
{
    group = 0;
}

#endif
#ifndef _UI_STATE_H_
#define _UI_STATE_H_

#include "def.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

bool needToDraw = false;

// TODO make this configurable 
uint8_t encoderCount = 4;
uint8_t encoderRootIndex = 0;

void setEncoderRootIndex(uint8_t index)
{
    encoderRootIndex = index;
}

void resetEncoderRootIndex()
{
    encoderRootIndex = 0;
}

#endif
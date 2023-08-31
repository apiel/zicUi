#ifndef _UI_EVENT_H_
#define _UI_EVENT_H_

#include "def.h"
#include "state.h"
#include "viewMain.h"

int lastLog = 0;
void handleMotion(int x, int y, int id)
{
    if (SDL_GetTicks() - lastLog > 100)
    {
        lastLog = SDL_GetTicks();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Motion %d, %d (id: %d)", x, y, id);
    }
}

void handleMotionUp(int x, int y, int id)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "MotionUp %d, %d (id: %d)", x, y, id);
}

void handleMotionDown(int x, int y, int id)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "MotionDown %d, %d (id: %d)", x, y, id);
}

int encoderScreenQuater = SCREEN_W / ENCODER_COUNT;
void emulateEncoder(SDL_MouseWheelEvent wheel)
{
    uint8_t encoderId = wheel.mouseX / encoderScreenQuater;
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Encoder %d: %d", encoderId, wheel.y);
}

bool handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Quit");
            return false;

        case SDL_MOUSEMOTION:
            handleMotion(event.motion.x, event.motion.y, event.motion.which);
            return true;

        case SDL_MOUSEBUTTONDOWN:
            handleMotionDown(event.motion.x, event.motion.y, event.motion.which);
            return true;

        case SDL_MOUSEBUTTONUP:
            handleMotionUp(event.motion.x, event.motion.y, event.motion.which);
            return true;

        case SDL_MOUSEWHEEL:
            emulateEncoder(event.wheel);
            return true;

        case SDL_FINGERMOTION:
            handleMotion(event.tfinger.x * SCREEN_W, event.tfinger.y * SCREEN_H, event.tfinger.fingerId);
            return true;

        case SDL_FINGERDOWN:
            handleMotionDown(event.tfinger.x * SCREEN_W, event.tfinger.y * SCREEN_H, event.tfinger.fingerId);
            return true;

        case SDL_FINGERUP:
            handleMotionUp(event.tfinger.x * SCREEN_W, event.tfinger.y * SCREEN_H, event.tfinger.fingerId);
            return true;
        }
    }
    return true;
}

#endif
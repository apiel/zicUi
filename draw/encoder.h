#ifndef _UI_DRAW_ENCODER_H_
#define _UI_DRAW_ENCODER_H_

#include <string>

#include "../def.h"
#include "../state.h"
#include "../draw.h"

void drawEncoderBase(Point position, const char *name, const char *valueStr, int marginRight = 40)
{
    drawText({position.x + 10, position.y + 5}, name, colors.encoder.title, 12);
    drawText({position.x + dimensions.encoder.w - marginRight, position.y + 5}, valueStr,
             colors.encoder.value, 20, APP_FONT_BOLD);

    SDL_SetRenderDrawColor(renderer, colors.encoder.title.r, colors.encoder.title.g, colors.encoder.title.b, 255);
    SDL_RenderDrawLine(renderer,
                       position.x + 10, position.y + dimensions.encoder.h - 10,
                       position.x + dimensions.encoder.w - 10, position.y + dimensions.encoder.h - 10);

    SDL_SetRenderDrawColor(renderer, colors.encoder.value.r, colors.encoder.value.g, colors.encoder.value.b, 255);
}

void drawEncoder(Point position, const char *name, uint8_t value, const char *valueStr, uint8_t max = 127, int marginRight = 40)
{
    drawEncoderBase(position, name, valueStr, marginRight);

    int x = position.x + 10;
    int y = position.y + dimensions.encoder.h - 10;
    int x2 = x + ((dimensions.encoder.w - 20) * (value / (float)max));
    SDL_RenderDrawLine(renderer,
                       x, y,
                       x2, y);
    SDL_RenderDrawLine(renderer,
                       x, y - 1,
                       x2, y - 1);
}

void drawEncoder(Point position, const char *name, uint8_t value)
{
    drawEncoder(position, name, value, std::to_string(value).c_str());
}

void drawEncoderPercentage(Point position, const char *name, float value)
{
    char valueStr[6];
    sprintf(valueStr, "%d%%", (int)(value * 100));
    drawEncoder(position, name, value * 100, valueStr, 100, 60);
}

void drawCenteredEncoder(Point position, const char *name, uint8_t value)
{
    int8_t val = value - 64;
    drawEncoderBase(position, name, std::to_string(val).c_str());

    int x = position.x + 10 + ((dimensions.encoder.w - 20) * 0.5);
    int y = position.y + dimensions.encoder.h - 10;
    int x2 = position.x + 10 + ((dimensions.encoder.w - 20) * (value / 127.0));
    SDL_RenderDrawLine(renderer,
                       x, y,
                       x2, y);
    SDL_RenderDrawLine(renderer,
                       x, y - 1,
                       x2, y - 1);
}

#endif

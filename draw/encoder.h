#ifndef _UI_DRAW_ENCODER_H_
#define _UI_DRAW_ENCODER_H_

#include <string>

#include "../def.h"
#include "../state.h"
#include "../draw.h"

void drawEncoder(Point position, const char *name, uint8_t value)
{
    drawText({position.x + 10, position.y + 5}, name, colors.encoder.title, 12);
    drawText({position.x + dimensions.encoder.w - 40, position.y + 5}, std::to_string(value).c_str(),
             colors.encoder.value, 20, APP_FONT_BOLD);

    SDL_SetRenderDrawColor(renderer, colors.encoder.title.r, colors.encoder.title.g, colors.encoder.title.b, 255);
    SDL_RenderDrawLine(renderer,
                       position.x + 10, position.y + dimensions.encoder.h - 10,
                       position.x + dimensions.encoder.w - 10, position.y + dimensions.encoder.h - 10);

    SDL_SetRenderDrawColor(renderer, colors.encoder.value.r, colors.encoder.value.g, colors.encoder.value.b, 255);
    SDL_RenderDrawLine(renderer,
                       position.x + 10, position.y + dimensions.encoder.h - 10,
                       position.x + ((dimensions.encoder.w - 10) * (value / 127.0f)), position.y + dimensions.encoder.h - 10);
    SDL_RenderDrawLine(renderer,
                       position.x + 11, position.y + dimensions.encoder.h - 11,
                       position.x + ((dimensions.encoder.w - 11) * (value / 127.0f)), position.y + dimensions.encoder.h - 11);
}

#endif

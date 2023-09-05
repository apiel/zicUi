#ifndef _UI_DRAW_ENCODER_H_
#define _UI_DRAW_ENCODER_H_

#include <string>

#include "../def.h"
#include "../state.h"
#include "../draw.h"

void drawEncoderBase(Point position, const char *name, const char *valueStr, int marginRight = 40)
{
    drawText({position.x + 10, position.y + 5}, name, colors.encoder.title, 12);
    drawTextRight({position.x + dimensions.encoder.w - 10, position.y + 5}, valueStr,
                  colors.encoder.value, 20, APP_FONT_BOLD);

    drawLine({position.x + 10, position.y + dimensions.encoder.h - 10},
             {position.x + dimensions.encoder.w - 10, position.y + dimensions.encoder.h - 10},
             colors.encoder.title);
}

void drawEncoder(Point position, const char *name, int value, const char *valueStr, int stepCount, int marginRight = 40)
{
    drawEncoderBase(position, name, valueStr, marginRight);

    int x = position.x + 10;
    int y = position.y + dimensions.encoder.h - 10;
    int x2 = x + ((dimensions.encoder.w - 20) * (value / (float)stepCount));
    drawLine({x, y}, {x2, y}, colors.encoder.value);
    drawLine({x, y - 1}, {x2, y - 1}, colors.encoder.value);
}

void drawEncoder(Point position, const char *name, float value, int stepCount)
{
    int val = value * stepCount;
    drawEncoder(position, name, val, std::to_string(val).c_str(), stepCount);
}

void drawEncoderPercentage(Point position, const char *name, float value)
{
    char valueStr[6];
    sprintf(valueStr, "%d%%", (int)(value * 100));
    drawEncoder(position, name, value * 100, valueStr, 100, 60);
}

void drawCenteredEncoder(Point position, const char *name, float value)
{
    drawTextCentered({(int)(position.x + (dimensions.encoder.w * 0.5)), position.y + 5}, name, colors.encoder.title, 12);

    int margin = 10;
    int val = value * 100;
    drawTextRight({position.x + dimensions.encoder.w - margin, position.y + 5}, std::to_string(val).c_str(),
                  colors.encoder.value, 20, APP_FONT_BOLD);
    drawText({position.x + margin, position.y + 5}, std::to_string(100 - val).c_str(),
             colors.encoder.value, 20, APP_FONT_BOLD);

    int x = position.x + 10 + ((dimensions.encoder.w - 20) * 0.5);
    int y = position.y + dimensions.encoder.h - 10;
    int x2 = position.x + 10 + ((dimensions.encoder.w - 20) * value);
    drawLine({x, y}, {x2, y}, colors.encoder.value);
    drawLine({x, y - 1}, {x2, y - 1}, colors.encoder.value);
}

#endif

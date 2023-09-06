#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"
#include "../host.h"

struct EncoderProps
{
    const char *pluginName;
    const char *key;
};

class ComponentEncoder : public View
{
protected:
    Value *value = NULL;
    const char *name = NULL;

    void drawEncoderBase(Point position, const char *name, const char *valueStr, int marginRight = 10)
    {
        drawText({position.x + 10, position.y + 5}, name, colors.encoder.title, 12);
        drawTextRight({position.x + dimensions.encoder.w - marginRight, position.y + 5}, valueStr,
                      colors.encoder.value, 20, APP_FONT_BOLD);

        drawLine({position.x + 10, position.y + dimensions.encoder.h - 10},
                 {position.x + dimensions.encoder.w - 10, position.y + dimensions.encoder.h - 10},
                 colors.encoder.title);
    }

    void drawEncoder(Point position, const char *name, int value, const char *valueStr, int stepCount, int marginRight = 10)
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
        sprintf(valueStr, "%d", (int)(value * 100));
        drawEncoder(position, name, value * 100, valueStr, 100, 20);
        drawText({position.x + dimensions.encoder.w - 18, position.y + 14}, "%", colors.encoder.title, 10);
    }

    void drawCenteredEncoder(Point position, const char *name, float value, int stepCount)
    {
        drawTextCentered({(int)(position.x + (dimensions.encoder.w * 0.5)), position.y + 5}, name, colors.encoder.title, 12);

        int margin = 10;
        int val = value * stepCount;
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

public:
    Point position;
    const Size size = dimensions.encoder;
    const int margin = 1;

    ComponentEncoder(Point _position)
        : position(_position)
    {
    }

    void set(EncoderProps props)
    {
        if (props.pluginName != NULL && props.key != NULL)
        {
            value = hostValue(props.pluginName, props.key);
        }
        else
        {
            value = NULL;
        }
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.encoder.background);

        if (value != NULL)
        {
            if (value->type() == VALUE_CENTERED)
            {
                drawCenteredEncoder(position, value->label(), value->get(), value->stepCount());
            }
            else if (value->type() == VALUE_PERCENTAGE)
            {
                drawEncoderPercentage(position, value->label(), value->get());
            }
            else
            {
                drawEncoder(position, value->label(), value->get(), value->stepCount());
            }
        }

        drawNext();
    }

    void onEncoder(int id, int8_t direction)
    {
        value->set(value->get() + (direction / (float)value->stepCount()));
        render();
    }
};

#endif

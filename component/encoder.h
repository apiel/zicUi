#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../def.h"
#include "../component.h"
#include "../draw.h"
#include "../host.h"

class ComponentEncoder : public Component
{
protected:
    const char *name = NULL;

    void drawEncoderBase(Point position, const char *name, const char *valueStr, int marginRight = 10)
    {
        drawText({position.x + 10, position.y + 5}, name, colors.encoder.title, 12);
        drawTextRight({position.x + styles.encoder.w - marginRight, position.y + 5}, valueStr,
                      colors.encoder.value, 20, APP_FONT_BOLD);

        drawLine({position.x + 10, position.y + styles.encoder.h - 10},
                 {position.x + styles.encoder.w - 10, position.y + styles.encoder.h - 10},
                 colors.encoder.title);
    }

    void drawEncoder(Point position, const char *name, int value, const char *valueStr, int stepCount, int marginRight = 10)
    {
        drawEncoderBase(position, name, valueStr, marginRight);

        int x = position.x + 10;
        int y = position.y + styles.encoder.h - 10;
        int x2 = x + ((styles.encoder.w - 20) * (value / (float)stepCount));
        drawLine({x, y}, {x2, y}, colors.encoder.value);
        drawLine({x, y - 1}, {x2, y - 1}, colors.encoder.value);
    }

    void drawEncoder(Point position, const char *name, float value, int stepCount, const char *unit)
    {
        int val = value * stepCount;
        int marginRight = 10;
        if (unit != NULL)
        {
            int x = position.x + styles.encoder.w - marginRight;
            marginRight += 3 + (x - drawTextRight({x, position.y + 14}, unit, colors.encoder.title, 10));
        }

        drawEncoder(position, name, val, std::to_string(val).c_str(), stepCount, marginRight);
    }

    void drawCenteredEncoder(Point position, const char *name, float value, int stepCount)
    {
        drawTextCentered({(int)(position.x + (styles.encoder.w * 0.5)), position.y + 5}, name, colors.encoder.title, 12);

        int margin = 10;
        int val = value * stepCount;
        drawTextRight({position.x + styles.encoder.w - margin, position.y + 5}, std::to_string(val).c_str(),
                      colors.encoder.value, 20, APP_FONT_BOLD);
        drawText({position.x + margin, position.y + 5}, std::to_string(100 - val).c_str(),
                 colors.encoder.value, 20, APP_FONT_BOLD);

        int x = position.x + 10 + ((styles.encoder.w - 20) * 0.5);
        int y = position.y + styles.encoder.h - 10;
        int x2 = position.x + 10 + ((styles.encoder.w - 20) * value);
        drawLine({x, y}, {x2, y}, colors.encoder.value);
        drawLine({x, y - 1}, {x2, y - 1}, colors.encoder.value);
    }

public:
    int8_t id = -1;
    const int margin = styles.margin;

    Value *value = NULL;

    ComponentEncoder(Point position, Size size)
        : Component(position, size)
    {
    }

    void config(char *key, char *value)
    {
        if (strcmp(key, "VALUE") == 0)
        {
            printf("value: %s\n", value);
            char *pluginName = strtok(value, " ");
            char *keyValue = strtok(NULL, " ");
            set({pluginName, keyValue});
        } else  if (strcmp(key, "ID") == 0)
        {
            id = atoi(value);
        }
    }

    void set(ValueProps props)
    {
        value = hostValue(props);
        if (value != NULL)
        {
            value->onUpdate([](float, void *data)
                            { ((ComponentEncoder *)data)->render(); },
                            this);
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
            else
            {
                drawEncoder(position, value->label(), value->get(), value->stepCount(), value->unit());
            }
        }

        drawNext();
    }

    void onEncoder(int _id, int8_t direction)
    {
        if (id == _id)
        {
            value->set(value->get() + (direction / (float)value->stepCount()));
        }
    }
};

#endif

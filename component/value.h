#ifndef _UI_COMPONENT_VALUE_H_
#define _UI_COMPONENT_VALUE_H_

#include "../def.h"
#include "../component.h"
#include "../draw.h"
#include "../host.h"

class ComponentValue : public Component
{
protected:
    const char *name = NULL;

    void drawBar()
    {
        drawLine({position.x + 10, position.y + size.h - 10},
                 {position.x + size.w - 10, position.y + size.h - 10},
                 colors.encoder.title);

        int x = position.x + 10;
        int y = position.y + size.h - 10;
        int x2 = x + ((size.w - 20) * value->get());
        drawLine({x, y}, {x2, y}, colors.encoder.value);
        drawLine({x, y - 1}, {x2, y - 1}, colors.encoder.value);
    }

    void drawEncoder()
    {
        int marginRight = 10;
        if (value->props->unit != NULL)
        {
            int x = position.x + size.w - marginRight;
            marginRight += 3 + (x - drawTextRight({x, position.y + 14}, value->props->unit, colors.encoder.title, 10));
        }

        drawText({position.x + 10, position.y + 5}, value->label(), colors.encoder.title, 12);

        int valInt = (value->get() * value->props->stepCount) + value->props->stepStart;
        drawTextRight({position.x + size.w - marginRight, position.y + 5}, std::to_string(valInt).c_str(),
                      colors.encoder.value, 20, {APP_FONT_BOLD});

        drawBar();
    }

    void drawCenteredBar()
    {
        int x = position.x + 10 + ((size.w - 20) * 0.5);
        int y = position.y + size.h - 10;
        int x2 = position.x + 10 + ((size.w - 20) * value->get());
        drawLine({x, y}, {x2, y}, colors.encoder.value);
        drawLine({x, y - 1}, {x2, y - 1}, colors.encoder.value);
    }

    void drawCenteredEncoderOneSided()
    {
        drawText({position.x + 10, position.y + 5}, value->label(), colors.encoder.title, 12);

        int margin = 10;
        int val = (value->get() * value->props->stepCount) + value->props->stepStart;
        drawTextRight({position.x + size.w - margin, position.y + 5}, std::to_string(val).c_str(),
                      colors.encoder.value, 20, {APP_FONT_BOLD});

        drawCenteredBar();
    }

    void drawCenteredEncoder()
    {
        drawTextCentered({(int)(position.x + (size.w * 0.5)), position.y + 5}, value->label(), colors.encoder.title, 12);

        int margin = 10;
        int val = (value->get() * value->props->stepCount) + value->props->stepStart;
        drawTextRight({position.x + size.w - margin, position.y + 5}, std::to_string(val).c_str(),
                      colors.encoder.value, 20, {APP_FONT_BOLD});
        drawText({position.x + margin, position.y + 5}, std::to_string(value->props->stepCount - val).c_str(),
                 colors.encoder.value, 20, {APP_FONT_BOLD});

        drawCenteredBar();
    }

    void drawStringEncoder()
    {
        drawText({position.x + 10, position.y + 5}, value->string(), colors.encoder.value, 12, {.maxWidth = size.w - 20});
        char valueStr[20];
        sprintf(valueStr, "%d / %d", (int)((value->get() * value->props->stepCount) + 1), value->props->stepCount + 1);
        drawTextRight({position.x + size.w - 10, position.y + 25}, valueStr, colors.encoder.title, 10);
        drawBar();
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.encoder.background);

        if (value != NULL)
        {
            if (value->props->type == VALUE_CENTERED)
            {
                drawCenteredEncoder();
            }
            else if (value->props->type == VALUE_CENTERED_ONE_SIDED)
            {
                drawCenteredEncoderOneSided();
            }
            else if (value->props->type == VALUE_STRING)
            {
                drawStringEncoder();
            }
            else
            {
                drawEncoder();
            }
        }
    }

public:
    int8_t enocderId = -1;
    const int margin = styles.margin;

    Value *value = NULL;

    ComponentValue(Point position, Size size)
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
        }
        else if (strcmp(key, "ENCODER_ID") == 0)
        {
            enocderId = atoi(value);
        }
    }

    void set(ValueProps props)
    {
        value = hostValue(props);
        if (value != NULL)
        {
            value->onUpdate([](float, void *data)
                            { ((ComponentValue *)data)->renderNext(); },
                            this);
        }
    }

    void onEncoder(int id, int8_t direction)
    {
        if (id == enocderId)
        {
            value->set(value->get() + (direction / (float)value->props->stepCount));
        }
    }
};

#endif

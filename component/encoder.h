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

    int xDraw;
    int8_t drawId = -1;

    void drawBar()
    {
        drawLine({xDraw, position.y + size.h - 10},
                 {position.x + size.w - 10, position.y + size.h - 10},
                 colors.encoder.title);

        int x = xDraw;
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

        drawText({xDraw, position.y + 5}, value->label(), colors.encoder.title, 12);

        int valInt = (value->get() * value->props->stepCount) + value->props->stepStart;
        drawTextRight({position.x + size.w - marginRight, position.y + 5}, std::to_string(valInt).c_str(),
                      colors.encoder.value, 20, {APP_FONT_BOLD});

        drawBar();
    }

    void drawCenteredBar()
    {
        int x = xDraw + ((size.w - 20) * 0.5);
        int y = position.y + size.h - 10;
        int x2 = xDraw + ((size.w - 20) * value->get());
        drawLine({x, y}, {x2, y}, colors.encoder.value);
        drawLine({x, y - 1}, {x2, y - 1}, colors.encoder.value);
    }

    void drawCenteredEncoderOneSided()
    {
        drawText({xDraw, position.y + 5}, value->label(), colors.encoder.title, 12);

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
        drawText({xDraw, position.y + 5}, std::to_string(value->props->stepCount - val).c_str(),
                 colors.encoder.value, 20, {APP_FONT_BOLD});

        drawCenteredBar();
    }

    void drawStringEncoder()
    {
        drawText({xDraw, position.y + 5}, value->string(), colors.encoder.value, 12, {.maxWidth = size.w - 20});
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

        if (drawId > 0)
        {
            drawFilledRect(
                {position.x + margin, position.y + margin},
                {12, 12},
                colors.encoder.id);
            drawTextCentered({position.x + margin + 6, position.y + margin}, std::to_string(drawId).c_str(), colors.encoder.background, 8);
        }

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
    int8_t encoderId = -1;
    const int margin = styles.margin;

    Value *value = NULL;

    ComponentEncoder(Point position, Size size)
        : Component(position, size), xDraw(position.x + 15)
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
            encoderId = atoi(value);
        }
    }

    void set(ValueProps props)
    {
        value = hostValue(props);
        if (value != NULL)
        {
            value->onUpdate([](float, void *data)
                            { ((ComponentEncoder *)data)->renderNext(); },
                            this);
        }
    }

    void onEncoder(int id, int8_t direction)
    {
        if (id == encoderId)
        {
            value->set(value->get() + (direction / (float)value->props->stepCount));
        }
    }

    void onEncoderRootIndexChanged(uint8_t index, uint8_t encoderCount)
    {
        int8_t shouldDrawId = -1;
        if (encoderId >= index  && encoderId <  index + encoderCount)
        {
            shouldDrawId = (encoderId % encoderCount) + 1;
        }
        if (shouldDrawId != drawId)
        {
            drawId = shouldDrawId;
            renderNext();
        }
        // printf("%d onEncoderRootIndexChanged %d %d => %d %d\n", encoderId, index, encoderCount, encoderId % encoderCount, drawId);
    }
};

#endif

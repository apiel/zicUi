#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../def.h"
#include "../component.h"
#include "../host.h"

class ComponentEncoder : public Component
{
protected:
    const char *name = NULL;

    struct DrawArea
    {
        int x;
        int xCenter;
        int y;
        int w;
        int h;
    } area;

    const int valueMarginTop = 15;

    bool encoderActive = false;

    void drawLabel()
    {
        draw.text({area.x, area.y}, value->label(), colors.encoder.title, 12);
    }

    void drawBar()
    {
        draw.line({area.x, area.y + size.h - 10},
                  {position.x + size.w - 10, area.y + size.h - 10},
                  colors.encoder.title);

        int x = area.x;
        int y = area.y + size.h - 10;
        int x2 = x + ((size.w - 20) * value->get());
        draw.line({x, y}, {x2, y}, colors.encoder.value);
        draw.line({x, y - 1}, {x2, y - 1}, colors.encoder.value);
    }

    void drawValue()
    {
        int val = (value->get() * value->props->stepCount) + value->props->stepStart;
        int x = draw.textCentered({area.xCenter, area.y + valueMarginTop}, std::to_string(val).c_str(),
                                  colors.encoder.value, 20, {APP_FONT_BOLD});

        if (value->props->unit != NULL)
        {
            draw.text({x + 2, area.y + valueMarginTop + 8}, value->props->unit, colors.encoder.title, 10);
        }
    }

    void drawEncoder()
    {
        drawLabel();
        drawValue();
        drawBar();
    }

    void drawCenteredBar()
    {
        int x = area.x + (area.w * 0.5);
        int y = area.y + area.h - 10;
        int x2 = area.x + (area.w * value->get());
        draw.line({x, y}, {x2, y}, colors.encoder.value);
        draw.line({x, y - 1}, {x2, y - 1}, colors.encoder.value);
    }

    void drawCenteredEncoderOneSided()
    {
        drawLabel();
        drawValue();
        drawCenteredBar();
    }

    void drawCenteredEncoder()
    {
        draw.textCentered({area.xCenter, area.y}, value->label(), colors.encoder.title, 12);

        int val = (value->get() * value->props->stepCount) + value->props->stepStart;
        draw.textRight({area.x + area.w, area.y + valueMarginTop}, std::to_string(val).c_str(),
                       colors.encoder.value, 20, {APP_FONT_BOLD});
        draw.text({area.x, area.y + valueMarginTop}, std::to_string(value->props->stepCount - val).c_str(),
                  colors.encoder.value, 20, {APP_FONT_BOLD});

        drawCenteredBar();
    }

    void drawStringEncoder()
    {
        draw.text({area.x, area.y + 5}, value->string(), colors.encoder.value, 12, {.maxWidth = area.w});
        char valueStr[20];
        sprintf(valueStr, "%d / %d", (int)((value->get() * value->props->stepCount) + 1), value->props->stepCount + 1);
        draw.textRight({area.x + area.w, area.y + 25}, valueStr, colors.encoder.title, 10);
        drawBar();
    }

    void render()
    {
        draw.filledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.encoder.background);

        if (encoderActive)
        {
            draw.filledRect(
                {position.x + margin, position.y + margin},
                {12, 12},
                colors.encoder.id);
            draw.textCentered({position.x + margin + 6, position.y + margin}, std::to_string(encoderId + 1).c_str(), colors.encoder.background, 8);
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

public:
    int8_t encoderId = -1;
    const int margin = styles.margin;

    Value *value = NULL;

    // margin left 15
    // margin right 10
    ComponentEncoder(Point position, Size size, Draw &draw)
        : Component(position, size, draw), area({position.x + 15, 0, position.y, size.w - (15 + 10), size.h})
    {
        area.xCenter = (int)(area.x + (area.w * 0.5));
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

    void onEncoder(int id, int8_t direction)
    {
        if (encoderActive && id == encoderId)
        {
            value->set(value->get() + (direction / (float)value->props->stepCount));
        }
    }

    void onGroupChanged(int8_t index) override
    {
        int8_t shouldActivate = false;
        if (group == index || group == -1)
        {
            shouldActivate = true;
        }
        if (shouldActivate != encoderActive)
        {
            encoderActive = shouldActivate;
            renderNext();
        }
        // printf("current group: %d inccoming group: %d drawId: %d\n", group, index, drawId);
    }
};

#endif

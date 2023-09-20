#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../component.h"
#include "../host.h"
#include "../plugins/valueInterface.h"

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
    int8_t encoderId = -1;

    ValueInterface *value = NULL;

    void drawLabel()
    {
        draw.text({area.x, area.y}, value->label(), colors.title, 12);
    }

    void drawBar()
    {
        draw.line({area.x, area.y + size.h - 10},
                  {position.x + size.w - 10, area.y + size.h - 10},
                  colors.title);

        int x = area.x;
        int y = area.y + size.h - 10;
        int x2 = x + ((size.w - 20) * value->get());
        draw.line({x, y}, {x2, y}, colors.value);
        draw.line({x, y - 1}, {x2, y - 1}, colors.value);
    }

    void drawValue()
    {
        int val = (value->get() * value->props->stepCount) + value->props->stepStart;
        int x = draw.textCentered({area.xCenter, area.y + valueMarginTop}, std::to_string(val).c_str(),
                                  colors.value, 20, {styles.font.bold});

        if (value->props->unit != NULL)
        {
            draw.text({x + 2, area.y + valueMarginTop + 8}, value->props->unit, colors.title, 10);
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
        draw.line({x, y}, {x2, y}, colors.value);
        draw.line({x, y - 1}, {x2, y - 1}, colors.value);
    }

    void drawCenteredEncoderOneSided()
    {
        drawLabel();
        drawValue();
        drawCenteredBar();
    }

    void drawCenteredEncoder()
    {
        draw.textCentered({area.xCenter, area.y}, value->label(), colors.title, 12);

        int val = (value->get() * value->props->stepCount) + value->props->stepStart;
        draw.textRight({area.x + area.w, area.y + valueMarginTop}, std::to_string(val).c_str(),
                       colors.value, 20, {styles.font.bold});
        draw.text({area.x, area.y + valueMarginTop}, std::to_string(value->props->stepCount - val).c_str(),
                  colors.value, 20, {styles.font.bold});

        drawCenteredBar();
    }

    void drawStringEncoder()
    {
        draw.text({area.x, area.y + 5}, value->string(), colors.value, 12, {.maxWidth = area.w});
        char valueStr[20];
        sprintf(valueStr, "%d / %d", (int)((value->get() * value->props->stepCount) + 1), value->props->stepCount + 1);
        draw.textRight({area.x + area.w, area.y + 25}, valueStr, colors.title, 10);
        drawBar();
    }

    void render()
    {
        draw.filledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.background);

        if (encoderActive)
        {
            draw.filledRect(
                {position.x + margin, position.y + margin},
                {12, 12},
                colors.id);
            draw.textCentered({position.x + margin + 6, position.y + margin}, std::to_string(encoderId + 1).c_str(), colors.background, 8);
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

    struct Colors
    {
        Color background;
        Color id;
        Color title;
        Color value;
    } colors;

    const int margin;

public:
    // margin left 15
    // margin right 10
    ComponentEncoder(Point position, Size size, Draw &draw)
        : Component(position, size, draw),
          colors({styles.colors.foreground2, styles.colors.foreground3, styles.colors.textDark, styles.colors.text}),
          margin(styles.margin),
          area({position.x + 15, 0, position.y, size.w - (15 + 10), size.h})
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

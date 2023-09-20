#ifndef _UI_COMPONENT_PAD_H_
#define _UI_COMPONENT_PAD_H_

#include "../component.h"
#include "../styles.h"

class ComponentPad : public Component
{
protected:
    struct Colors
    {
        Color background = styles.colors.foreground;
        Color value = styles.colors.text;
        Color title = styles.colors.textDark;
    } colors;

    int drawValue(const char *c, Value *value, Point position)
    {
        int x = position.x;
        x = draw.text({x, position.y}, c, colors.title, 12);
        int val = value->get() * value->props->stepCount;
        if (value->props->type == VALUE_CENTERED)
        {
            x = draw.text({x + 3, position.y}, std::to_string(100 - val).c_str(), colors.value, 12, {styles.font.bold});
        }
        x = draw.text({x + 3, position.y}, value->label(), colors.title, 12);
        x = draw.text({x + 3, position.y}, std::to_string(val).c_str(), colors.value, 12, {styles.font.bold});
        if (value->props->unit != NULL)
        {
            x = draw.text({x + 2, position.y}, value->props->unit, colors.title, 10);
        }
        return x;
    }

    void render()
    {
        draw.filledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.background);

        if (valueX != NULL && valueY != NULL)
        {
            int y = position.y + size.h - 20;
            drawValue("X:", valueX, {position.x + 10, y});
            drawValue("Y:", valueY, {(int)(position.x + size.w * 0.5), y});

            // NOTE if it is not a centered value, should draw 0 in the middle
            // and both side from the center being positive value to 1.0 ?

            draw.filledRect(
                {position.x + (margin * 2) + (int)((size.w - (pointerSize * 2)) * valueX->get()),
                 position.y + (margin * 2) + (int)((size.h - (pointerSize * 2)) * (1.0 - valueY->get()))},
                {pointerSize, pointerSize},
                colors.value);
        }
    }

    void setValueX(ValueProps value)
    {
        valueX = hostValue(value);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Pad X value: %s %s", value.pluginName, value.key);
        if (valueX != NULL)
        {
            valueX->onUpdate([](float, void *data)
                             { ((ComponentPad *)data)->renderNext(); },
                             this);
        }
    }

    void setValueY(ValueProps value)
    {
        valueY = hostValue(value);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Pad Y value: %s %s", value.pluginName, value.key);
        if (valueY != NULL)
        {
            valueY->onUpdate([](float, void *data)
                             { ((ComponentPad *)data)->renderNext(); },
                             this);
        }
    }

public:
    int pointerSize = 4;
    Value *valueX = NULL;
    Value *valueY = NULL;
    bool holdValue = true;
    float releaseX = 0.0f;
    float releaseY = 0.0f;

    const int margin = styles.margin;

    ComponentPad(Point position, Size size, Draw &draw)
        : Component(position, size, draw)
    {
    }

    void config(char *key, char *value)
    {
        if (strcmp(key, "VALUE_X") == 0)
        {
            char *pluginName = strtok(value, " ");
            char *keyValue = strtok(NULL, " ");
            setValueX({pluginName, keyValue});
        }
        else if (strcmp(key, "VALUE_Y") == 0)
        {
            char *pluginName = strtok(value, " ");
            char *keyValue = strtok(NULL, " ");
            setValueY({pluginName, keyValue});
        }
        else if (strcmp(key, "HOLD_VALUE") == 0)
        {
            holdValue = strcmp(value, "true") == 0;
        }
        else if (strcmp(key, "RELEASE_X") == 0)
        {
            releaseX = atof(value);
        }
        else if (strcmp(key, "RELEASE_Y") == 0)
        {
            releaseY = atof(value);
        }
    }

    void onMotion(Motion &motion)
    {
        if (valueX == NULL || valueY == NULL)
        {
            return;
        }

        float x = (motion.position.x - position.x - margin) / (float)(size.w - 2 * margin);
        if (x - valueX->get() > 0.01 || valueX->get() - x > 0.01)
        {
            valueX->set(x);
        }
        float y = 1.0 - (motion.position.y - position.y - margin) / (float)(size.h - 2 * margin);
        if (y - valueY->get() > 0.01 || valueY->get() - y > 0.01)
        {
            valueY->set(y);
        }
    }

    void onMotionRelease(Motion &motion)
    {
        Component::onMotionRelease(motion);
        if (holdValue == false && valueX != NULL && valueY != NULL && (valueX->get() != releaseX || valueY->get() != releaseY))
        {
            valueX->set(releaseX);
            valueY->set(releaseY);
        }
    }
};

#endif

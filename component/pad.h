#ifndef _UI_COMPONENT_PAD_H_
#define _UI_COMPONENT_PAD_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"

struct PadOptions
{
    bool holdValue = true;
    float releaseX = 0.0f;
    float releaseY = 0.0f;
};

class ComponentPad : public View
{
protected:
    int drawValue(const char *c, Value *value, Point position)
    {
        int x = position.x;
        x = drawText({x, position.y}, c, colors.encoder.title, 12);
        int val = value->get() * value->stepCount();
        if (value->type() == VALUE_CENTERED)
        {
            x = drawText({x + 3, position.y}, std::to_string(100 - val).c_str(), colors.encoder.value, 12, APP_FONT_BOLD);
        }
        x = drawText({x + 3, position.y}, value->label(), colors.encoder.title, 12);
        x = drawText({x + 3, position.y}, std::to_string(val).c_str(), colors.encoder.value, 12, APP_FONT_BOLD);
        if (value->unit() != NULL)
        {
            x = drawText({x + 2, position.y}, value->unit(), colors.encoder.title, 10);
        }
        return x;
    }

public:
    int pointerSize = 4;
    Value *valueX = NULL;
    Value *valueY = NULL;
    PadOptions options;

    const int margin = styles.margin;

    ComponentPad(Point position, Size size, ValueProps valueXProps, ValueProps valueYProps, PadOptions options = {})
        : View(position, size), valueX(hostValue(valueXProps)), valueY(hostValue(valueYProps)), options(options)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Pad X value: %s %s %s", valueXProps.pluginName, valueXProps.key, valueX == NULL ? "NULL" : "NOT NULL");
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Pad Y value: %s %s %s", valueYProps.pluginName, valueYProps.key, valueY == NULL ? "NULL" : "NOT NULL");
        if (valueY != NULL && valueX != NULL)
        {
            valueX->onUpdate([](float, void *data)
                             { ((ComponentPad *)data)->render(); },
                             this);

            valueY->onUpdate([](float, void *data)
                             { ((ComponentPad *)data)->render(); },
                             this);
        }
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.pad.background);

        if (valueX != NULL && valueY != NULL)
        {
            int y = position.y + size.h - 20;
            drawValue("X:", valueX, {position.x + 10, y});
            drawValue("Y:", valueY, {(int)(position.x + size.w * 0.5), y});

            // NOTE if it is not a centered value, should draw 0 in the middle
            // and both side from the center being positive value to 1.0 ?

            drawFilledRect(
                {position.x + (margin * 2) + (int)((size.w - (pointerSize * 2)) * valueX->get()),
                 position.y + (margin * 2) + (int)((size.h - (pointerSize * 2)) * (1.0 - valueY->get()))},
                {pointerSize, pointerSize},
                colors.pad.value);
        }
        drawNext();
    }

    void onMotion(Motion &motion)
    {
        if (!motion.in({position, size}) || !motion.originIn({position, size}))
        {
            return;
        }

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
        if (options.holdValue == false && valueX != NULL && valueY != NULL)
        {
            valueX->set(options.releaseX);
            valueY->set(options.releaseY);
        }
    }
};

#endif

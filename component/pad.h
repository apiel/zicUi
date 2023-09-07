#ifndef _UI_COMPONENT_PAD_H_
#define _UI_COMPONENT_PAD_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"

#ifndef PAD_COUNT
#define PAD_COUNT 2
#endif

class ComponentPad : public View
{
public:
    Point position;
    const Size size;

    const int margin = 1;

    float xValue = 0.5f;
    float yValue = 0.5f;

    ComponentPad(Point position, Size size) : position(position), size(size)
    {
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.pad.background);

        drawFilledRect(
            {position.x + margin + (int)((size.w - 4) * xValue),
             position.y + margin + (int)((size.h - 4) * yValue)},
            {4, 4},
            colors.pad.value);
    }

    void onMotion(Motion &motion)
    {
        if (!motion.in({position, size}) || !motion.originIn({position, size}))
        {
            return;
        }

        xValue = (motion.position.x - position.x - margin) / (float)(size.w - 2 * margin);
        yValue = (motion.position.y - position.y - margin) / (float)(size.h - 2 * margin);
        // printf("val %f %f\n", xValue, yValue);
        render();
        drawNext();
    }
};

#endif

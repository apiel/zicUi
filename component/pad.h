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
    int pointerSize = 4;
    Point position;
    const Size size;
    Value *valueX = NULL;
    Value *valueY = NULL;

    const int margin = 1;

    ComponentPad(Point position, Size size, ValueProps valueXProps, ValueProps valueYProps)
        : position(position), size(size), valueX(hostValue(valueXProps)), valueY(hostValue(valueYProps))
    {
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.pad.background);

        if (valueX != NULL && valueY != NULL)
        {

            // NOTE if it is not a centered value, should draw 0 in the middle
            // and both side from the center being positive value to 1.0 ?

            drawFilledRect(
                {position.x + (margin * 2) + (int)((size.w - (pointerSize * 2)) * valueX->get()),
                 position.y + (margin * 2) + (int)((size.h - (pointerSize * 2)) * valueY->get())},
                {pointerSize, pointerSize},
                colors.pad.value);
        }
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

        // should this be debounced
        // valueX->set((motion.position.x - position.x - margin) / (float)(size.w - 2 * margin));
        // valueY->set((motion.position.y - position.y - margin) / (float)(size.h - 2 * margin));
        float x = (motion.position.x - position.x - margin) / (float)(size.w - 2 * margin);
        if (x - valueX->get() > 0.01 || valueX->get() - x > 0.01)
        {
            valueX->set(x);
        }
        float y = (motion.position.y - position.y - margin) / (float)(size.h - 2 * margin);
        if (y - valueY->get() > 0.01 || valueY->get() - y > 0.01)
        {
            valueY->set(y);
        }
        render();
        drawNext();
    }
};

#endif

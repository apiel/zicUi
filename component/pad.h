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
    Value *valueX = NULL;
    Value *valueY = NULL;

    const int margin = 1;

    ComponentPad(Point position, Size size, ValueProps valueXProps, ValueProps valueYProps)
        : position(position)
        , size(size)
        , valueX(hostValue(valueXProps))
        , valueY(hostValue(valueYProps))
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
            drawFilledRect(
                {position.x + margin + (int)((size.w - 4) * valueX->get()),
                 position.y + margin + (int)((size.h - 4) * valueY->get())},
                {4, 4},
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
        valueX->set((motion.position.x - position.x - margin) / (float)(size.w - 2 * margin));
        valueY->set((motion.position.y - position.y - margin) / (float)(size.h - 2 * margin));
        render();
        drawNext();
    }
};

#endif

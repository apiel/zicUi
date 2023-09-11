#ifndef _UI_COMPONENT_GRANULAR_H_
#define _UI_COMPONENT_GRANULAR_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"

class ComponentGranular : public View
{
protected:

public:
    Point position;
    const Size size;

    const int margin = styles.margin;

    ComponentGranular(Point position, Size size)
        : position(position), size(size)
    {
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.pad.background);

        drawNext();
    }

    void onMotion(Motion &motion)
    {
        
    }

    void onMotionRelease(Motion &motion)
    {
    }
};

#endif

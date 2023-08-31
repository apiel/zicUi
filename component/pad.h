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
    Point position = {0, dimensions.pad_y};
    const Size size = {SCREEN_W / PAD_COUNT, dimensions.pad_h};
    const int margin = 1;

    ComponentPad(uint8_t id)
    {
        position.x = id * size.w;
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.pad.background);
    }
};

#endif

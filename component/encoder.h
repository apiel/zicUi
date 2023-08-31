#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"

class ComponentEncoder : public View
{
public:
    Point position;
    const Size size = {SCREEN_W / ENCODER_COUNT, 50};
    const int margin = 1;

    void (*callback)(int8_t) = [](int8_t) {};

    ComponentEncoder(Point _position)
        : position(_position)
    {
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.encoder.background);
    }

    void onEncoder(int id, int8_t direction)
    {
        callback(direction);
    }
};

#endif

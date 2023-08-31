#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../def.h"
#include "../view.h"

class ComponentEncoder : public View
{
public:
    Point position;
    const Size size = {SCREEN_W / ENCODER_COUNT, 50};

    ComponentEncoder(Point _position)
        : position(_position)
    {
    }

    void render()
    {
    }
};

#endif

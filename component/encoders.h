#ifndef _UI_COMPONENT_ENCODERS_H_
#define _UI_COMPONENT_ENCODERS_H_

#include "../def.h"
#include "../view.h"
#include "encoder.h"

class ComponentEncoders : public View
{
public:
    ComponentEncoder encoders[ENCODER_COUNT] = {
        {{0, 0}},
        {{SCREEN_W / ENCODER_COUNT, 0}},
        {{SCREEN_W / ENCODER_COUNT * 2, 0}},
        {{SCREEN_W / ENCODER_COUNT * 3, 0}},
    };

    void render()
    {
        for (int i = 0; i < ENCODER_COUNT; ++i)
        {
            encoders[i].render();
        }
    }

    void onEncoder(int id, int8_t direction)
    {
        if (id < ENCODER_COUNT)
        {
            encoders[id].onEncoder(id, direction);
        }
    }
};

#endif

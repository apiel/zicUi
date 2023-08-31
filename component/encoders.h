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

    ComponentEncoders()
    {
        encoders[0].callback = [](int8_t direction)
        { printf("Encoder 0 %d\n", direction); };
        encoders[1].callback = [](int8_t direction)
        { printf("Encoder oui 1 %d\n", direction); };
        encoders[2].callback = [](int8_t direction)
        { printf("Encoder YO 2 %d\n", direction); };
        encoders[3].callback = [](int8_t direction)
        { printf("Encoder yeah 3 %d\n", direction); };
    }

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

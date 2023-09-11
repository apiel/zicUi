#ifndef _UI_COMPONENT_ENCODERS_H_
#define _UI_COMPONENT_ENCODERS_H_

#include "../def.h"
#include "../component.h"
#include "encoder.h"

struct EncodersProps
{
    ValueProps props[ENCODER_COUNT];
};

class ComponentEncoders : public Component
{
protected:
    static ComponentEncoders *instance;

    ComponentEncoders(): Component({0, 0}, {SCREEN_W, SCREEN_H}) {}

public:
    ComponentEncoder encoders[ENCODER_COUNT] = {
        {{0, 0}, styles.encoder},
        {{SCREEN_W / ENCODER_COUNT, 0}, styles.encoder},
        {{SCREEN_W / ENCODER_COUNT * 2, 0}, styles.encoder},
        {{SCREEN_W / ENCODER_COUNT * 3, 0}, styles.encoder},
    };

    static ComponentEncoders &get()
    {
        if (!instance)
        {
            instance = new ComponentEncoders();
        }
        return *instance;
    }

    void set(EncodersProps props)
    {
        for (int i = 0; i < ENCODER_COUNT; ++i)
        {
            encoders[i].set(props.props[i]);
            // if (encoders[i].value != NULL) {
            //     encoders[i].value->onUpdate([] (float) {
            //         ComponentEncoders::get().render(i);
            //     }, i);
            // }
        }
    }

    void render(uint8_t id)
    {
        encoders[id].render();
    }

    void render()
    {
        for (int i = 0; i < ENCODER_COUNT; ++i)
        {
            render(i);
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

ComponentEncoders *ComponentEncoders::instance = NULL;

#endif

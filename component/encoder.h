#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"
#include "../host.h"
#include "../draw/encoder.h"

struct EncoderProps
{
    const char *pluginName;
    const char *key;
};

class ComponentEncoder : public View
{
protected:
    Value *value = NULL;
    const char *name = NULL;

public:
    Point position;
    const Size size = dimensions.encoder;
    const int margin = 1;

    ComponentEncoder(Point _position)
        : position(_position)
    {
    }

    void set(EncoderProps props)
    {
        if (props.pluginName != NULL && props.key != NULL)
        {
            value = hostValue(props.pluginName, props.key);
        }
        else
        {
            value = NULL;
        }
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.encoder.background);

        if (value != NULL)
        {
            if (value->type() == VALUE_CENTERED)
            {
                drawCenteredEncoder(position, value->label(), value->get(), value->stepCount());
            }
            else
            {
                drawEncoder(position, value->label(), value->get(), value->stepCount());
            }
        }

        drawNext();
    }

    void onEncoder(int id, int8_t direction)
    {
        value->set(value->get() + (direction / (float)value->stepCount()));
        render();
    }
};

#endif

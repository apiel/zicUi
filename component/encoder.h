#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"
#include "../host.h"
#include "../draw/encoder.h"

enum EncoderType
{
    ENCODER_NONE,
    ENCODER_BASIC,
    ENCODER_CENTERED,
};

struct EncoderProps
{
    const char *pluginName;
    const char *key;
    const char *name = NULL;
    EncoderType type = ENCODER_BASIC;
};

class ComponentEncoder : public View
{
protected:
    Value *value = NULL;
    EncoderType type;
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
        type = props.type;
        if (type != ENCODER_NONE)
        {
            value = hostValue(props.pluginName, props.key);
            name = props.name;
        }
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.encoder.background);

        if (type == ENCODER_BASIC)
        {
            drawEncoder(position, name ? name : value->name(), value->get(), value->stepCount());
        }
        else if (type == ENCODER_CENTERED)
        {
            drawCenteredEncoder(position, name ? name : value->name(), value->get(), value->stepCount());
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

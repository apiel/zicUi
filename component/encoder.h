#ifndef _UI_COMPONENT_ENCODER_H_
#define _UI_COMPONENT_ENCODER_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"

struct EncoderProps
{
    void (*callback)(int8_t) = [](int8_t) {};
    void (*render)(Point) = [](Point) {};
};

class ComponentEncoder : public View
{
public:
    Point position;
    const Size size = dimensions.encoder;
    const int margin = 1;

    EncoderProps props;

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

        props.render(position);
    }

    void onEncoder(int id, int8_t direction)
    {
        props.callback(direction);
    }
};

#endif

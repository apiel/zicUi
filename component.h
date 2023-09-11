#ifndef _UI_COMPONENT_H_
#define _UI_COMPONENT_H_

#include "def.h"
#include "motion.h"

class Component
{
public:
    Point position;
    const Size size;

    Component(Point position, Size size)
        : position(position), size(size)
    {
    }

    virtual void render() = 0;

    virtual void onMotion(Motion &motion)
    {
    }

    virtual void onMotionRelease(Motion &motion)
    {
    }

    virtual void onEncoder(int id, int8_t direction)
    {
    }
};

#endif

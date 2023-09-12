#ifndef _UI_COMPONENT_H_
#define _UI_COMPONENT_H_

#include "draw.h"
#include "def.h"
#include "motion.h"

class Component
{
protected:
    virtual void render() = 0;

public:
    Point position;
    const Size size;
    bool needRendering = true;

    Component(Point position, Size size)
        : position(position), size(size)
    {
    }

    virtual void triggerRenderer()
    {
        if (needRendering)
        {
            render();
            needRendering = false;
            drawNext();
        }
    }

    virtual void renderNext()
    {
        needRendering = true;
    }

    virtual void onMotion(Motion &motion)
    {
    }

    virtual void onMotionRelease(Motion &motion)
    {
    }

    virtual void onEncoder(int id, int8_t direction)
    {
    }

    virtual void config(char *key, char *value)
    {
    }
};

#endif

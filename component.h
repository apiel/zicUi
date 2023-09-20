#ifndef _UI_COMPONENT_H_
#define _UI_COMPONENT_H_

#include "draw.h"
#include "motion.h"

class Component
{
protected:
    Draw &draw;
    Styles &styles;
    virtual void render() = 0;

public:
    Point position;
    const Size size;
    bool needRendering = true;
    int8_t group = -1;

    Component(Point position, Size size, Draw &draw)
        : draw(draw), styles(draw.styles), position(position), size(size)
    {
    }

    virtual void triggerRenderer()
    {
        if (needRendering)
        {
            render();
            needRendering = false;
            draw.next();
        }
    }

    virtual void renderNext()
    {
        needRendering = true;
    }

    virtual void onMotion(Motion &motion)
    {
    }

    virtual void handleMotion(Motion &motion)
    {
        if (!motion.originIn({position, size}))
        {
            return;
        }

        if (motion.in({position, size}))
        {
            if (motion.isStarting())
            {
                setGroup(group);
            }
            onMotion(motion);
        }
    }

    virtual void onMotionRelease(Motion &motion)
    {
    }

    virtual void handleMotionRelease(Motion &motion)
    {
        resetGroup();
        onMotionRelease(motion);
    }

    virtual void onEncoder(int id, int8_t direction)
    {
    }

    virtual void config(char *key, char *value)
    {
    }
    virtual void baseConfig(char *key, char *value)
    {
        if (strcmp(key, "GROUP") == 0)
        {
            group = atoi(value);
        }
        else
        {
            config(key, value);
        }
    }

    virtual void onGroupChanged(int8_t index)
    {
    }
};

#endif

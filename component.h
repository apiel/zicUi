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
    uint8_t encoderRootIndex = 0;

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

    virtual void handleMotion(Motion &motion)
    {
        if (motion.in({position, size}))
        {
            if (motion.isStarting())
            {
                // printf("Motion Starting, set encoderRootIndex %d\n", encoderRootIndex);
                setEncoderRootIndex(encoderRootIndex);
            }
            onMotion(motion);
        }
    }

    virtual void onMotionRelease(Motion &motion)
    {
    }

    virtual void handleMotionRelease(Motion &motion)
    {
        resetEncoderRootIndex();
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
        if (strcmp(key, "ENCODER_ROOT_INDEX") == 0)
        {
            encoderRootIndex = atoi(value);
        }
        else
        {
            config(key, value);
        }
    }
};

#endif

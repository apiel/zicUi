#ifndef _UI_VIEW_H_
#define _UI_VIEW_H_

#include "def.h"
#include "motion.h"

class View
{
public:
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

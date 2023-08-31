#ifndef _UI_VIEW_H_
#define _UI_VIEW_H_

#include "def.h"
#include "motion.h"

class View
{
public:
    virtual void render() = 0;

    void onMotion(Motion &motion)
    {
    }

    void onMotionRelease(Motion &motion)
    {
    }

    void onEncoder(int id, int8_t direction)
    {
        printf("Encoder component [%d] %d\n", id, direction);
    }
};

#endif

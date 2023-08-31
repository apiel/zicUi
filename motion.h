#ifndef _UI_MOTION_H_
#define _UI_MOTION_H_

#include "def.h"

// Touch and mouse motion
class Motion
{
public:
    int id;
    Point position;
    Point origin;

    Motion(int id, int x, int y)
        : id(id), position({x, y}), origin({x, y})
    {
    }

    void move(int x, int y)
    {
        position.x = x;
        position.y = y;
    }
};

#endif

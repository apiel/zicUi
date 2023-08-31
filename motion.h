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

    Motion() : id(-1), position({0, 0}), origin({0, 0})
    {
    }

    Motion(int id, int x, int y)
        : id(id), position({x, y}), origin({x, y})
    {
    }


    void setId(int id)
    {
        this->id = id;
    }

    void init(int id, int x, int y)
    {
        this->id = id;
        position.x = x;
        position.y = y;
        origin.x = x;
        origin.y = y;
    }

    void move(int x, int y)
    {
        position.x = x;
        position.y = y;
    }

    bool in(Rect rect)
    {
        return (position.x >= rect.position.x &&
                position.x <= rect.position.x + rect.size.w &&
                position.y >= rect.position.y &&
                position.y <= rect.position.y + rect.size.h);
    }
};

#endif

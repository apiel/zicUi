#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "view.h"
#include "draw.h"
#include "component/encoders.h"
#include "viewPad.h"

class ViewMain : public View
{
protected:
    ComponentEncoders& encoders = ComponentEncoders::get();
    ViewPad& viewPad = ViewPad::get();

    View& view = viewPad;

    static ViewMain *instance;

    ViewMain() {}

public:
    static ViewMain &get()
    {
        if (!instance)
        {
            instance = new ViewMain();
        }
        return *instance;
    }

    void render()
    {
        drawClear();
        view.render();
        drawNext();
    }

    void onMotion(Motion &motion)
    {
        // printf("Motion yo [%d] %d, %d (origin: %d, %d)\n", motion.id, motion.position.x, motion.position.y, motion.origin.x, motion.origin.y);
        view.onMotion(motion);
    }

    void onMotionRelease(Motion &motion)
    {
        // printf("Motion released [%d] %d, %d (origin: %d, %d)\n", motion.id, motion.position.x, motion.position.y, motion.origin.x, motion.origin.y);
        view.onMotionRelease(motion);
    }

    void onEncoder(int id, int8_t direction)
    {
        encoders.onEncoder(id, direction);
    }
};

ViewMain *ViewMain::instance = NULL;

#endif
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
        viewPad.render();
        drawText({10, 110}, "Hello World! 12345 # $ +");
        drawRect({10, 140}, {100, 100}, {255, 0, 0, 255});
        draw();
    }

    void onMotion(Motion &motion)
    {
        printf("Motion [%d] %d, %d (origin: %d, %d)\n", motion.id, motion.position.x, motion.position.y, motion.origin.x, motion.origin.y);
    }

    void onMotionRelease(Motion &motion)
    {
        printf("Motion released [%d] %d, %d (origin: %d, %d)\n", motion.id, motion.position.x, motion.position.y, motion.origin.x, motion.origin.y);
    }

    void onEncoder(int id, int8_t direction)
    {
        encoders.onEncoder(id, direction);
    }
};

ViewMain *ViewMain::instance = NULL;

#endif
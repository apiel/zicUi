#ifndef _UI_VIEW_PAD_H_
#define _UI_VIEW_PAD_H_

#include "def.h"
#include "view.h"
#include "draw.h"
#include "component/encoders.h"

class ViewPad : public View
{
protected:
    ComponentEncoders &encoders = ComponentEncoders::get();

    static ViewPad *instance;

    ViewPad()
    {
        encoders.set(EncodersProps{
            {{[](int8_t direction)
              { printf("Filter %d\n", direction); }},
             {[](int8_t direction)
              { printf("Resonance %d\n", direction); }},
             {[](int8_t direction)
              { printf("Sample reducer %d\n", direction); }},
             {[](int8_t direction)
              { printf("Distortion %d\n", direction); }}},
        });
    }

public:
    static ViewPad &get()
    {
        if (!instance)
        {
            instance = new ViewPad();
        }
        return *instance;
    }

    void render()
    {
        encoders.render();
    }
};

ViewPad *ViewPad::instance = NULL;

#endif
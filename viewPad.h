#ifndef _UI_VIEW_PAD_H_
#define _UI_VIEW_PAD_H_

#include "def.h"
#include "view.h"
#include "draw.h"
#include "component/encoders.h"
#include "component/pad.h"
#include "host.h"

class ViewPad : public View
{
protected:
    ComponentPad padA = {0};
    ComponentPad padB = {1};

    static ViewPad *instance;

    ViewPad()
    {
        encoders.set({{{"MultiModeFilter", "CUTOFF"},
                       {"MultiModeFilter", "RESONANCE"},
                       {"SampleRateReducer", "SAMPLE_STEP"},
                       {"Distortion", "DRIVE"}}});
    }

public:
    ComponentEncoders &encoders = ComponentEncoders::get();

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
        padA.render();
        padB.render();
    }

    void onMotion(Motion &motion) override
    {
        padA.onMotion(motion);
        padB.onMotion(motion);
    }
};

ViewPad *ViewPad::instance = NULL;

#endif
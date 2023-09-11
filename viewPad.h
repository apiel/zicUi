#ifndef _UI_VIEW_PAD_H_
#define _UI_VIEW_PAD_H_

#include "def.h"
#include "view.h"
#include "draw.h"
#include "component/encoders.h"
#include "component/pad.h"
#include "component/granular.h"
#include "host.h"

class ViewPad : public View
{
protected:
    ComponentEncoders &encoders = ComponentEncoders::get();

    int pad_h = SCREEN_H - encoders.encoders[0].size.h;
    int pad_y = encoders.encoders[0].position.x + encoders.encoders[0].size.h;
    int halfScreen_w = SCREEN_W / 2;

    ComponentPad padA = {{0, pad_y}, {halfScreen_w, pad_h}, {"Delay", "CUTOFF"}, {"Delay", "MASTER_AMPLITUDE"}};
    ComponentGranular granular = {{halfScreen_w, pad_y}, {halfScreen_w, 300}};

    static ViewPad *instance;

    ViewPad()
    : View({0, 0}, {SCREEN_W, SCREEN_H})
    {
        encoders.set({{{"MultiModeFilter", "CUTOFF"},
                       {"MultiModeFilter", "RESONANCE"},
                       {"SampleRateReducer", "SAMPLE_STEP"},
                       {"Distortion", "DRIVE"}}});
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
        padA.render();
        granular.render();
    }

    void onMotion(Motion &motion) override
    {
        padA.onMotion(motion);
        granular.onMotion(motion);
    }

    void onMotionRelease(Motion &motion)
    {
        padA.onMotionRelease(motion);
        granular.onMotionRelease(motion);
    }
};

ViewPad *ViewPad::instance = NULL;

#endif
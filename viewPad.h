#ifndef _UI_VIEW_PAD_H_
#define _UI_VIEW_PAD_H_

#include "def.h"
#include "view.h"
#include "draw.h"
#include "component/encoders.h"
#include "component/pad.h"
#include "draw/encoder.h"
#include "host.h"

class ViewPad : public View
{
protected:
    ComponentPad padA = {0};
    ComponentPad padB = {1};

    static ViewPad *instance;

    ViewPad()
    {
        // encoders.set(EncodersProps{
        //     {{[](int8_t direction)
        //       {
        //           ViewPad &pad = ViewPad::get();
        //           pad.cutoff.set(pad.cutoff.get() + (direction / (float)pad.cutoff.stepCount()));
        //           pad.encoders.render(0);
        //           drawNext();
        //       },
        //       [](Point position)
        //       {
        //           ViewPad &pad = ViewPad::get();
        //           drawCenteredEncoder(position, "LPF | HPF", pad.cutoff.get(), pad.cutoff.stepCount());
        //       }},
        //      {[](int8_t direction)
        //       {
        //           ViewPad &pad = ViewPad::get();
        //           pad.resonance.set(pad.resonance.get() + (direction / (float)pad.resonance.stepCount()));
        //           pad.encoders.render(1);
        //           drawNext();
        //       },
        //       [](Point position)
        //       {
        //           ViewPad &pad = ViewPad::get();
        //           drawEncoder(position, "Resonance", pad.resonance.get(), pad.resonance.stepCount());
        //       }},
        //      {[](int8_t direction)
        //       {
        //           ViewPad &pad = ViewPad::get();
        //           pad.sampleReducer.set(pad.sampleReducer.get() + (direction / (float)pad.sampleReducer.stepCount()));
        //           pad.encoders.render(2);
        //           drawNext();
        //       },
        //       [](Point position)
        //       {
        //           ViewPad &pad = ViewPad::get();
        //           drawEncoder(position, "Sample rate reducer", pad.sampleReducer.get(), pad.sampleReducer.stepCount());
        //       }},
        //      {[](int8_t direction)
        //       {
        //           ViewPad &pad = ViewPad::get();
        //           pad.distortion.set(pad.distortion.get() + (direction / (float)pad.distortion.stepCount()));
        //           pad.encoders.render(3);
        //           drawNext();
        //       },
        //       [](Point position)
        //       {
        //           ViewPad &pad = ViewPad::get();
        //           drawEncoder(position, "Distortion", pad.distortion.get(), pad.distortion.stepCount());
        //       }}},
        // });

        encoders.set({{{"MultiModeFilter", "CUTOFF"},
                       {"MultiModeFilter", "RESONANCE"},
                       {"SampleRateReducer", "SAMPLE_STEP"},
                       {"Distortion", "DRIVE"}}});
    }

public:
    ComponentEncoders &encoders = ComponentEncoders::get();

    // AudioPlugin &effectPlugin = getPlugin("MultiModeFilter");
    // Value cutoff = Value(effectPlugin, "CUTOFF");
    // Value cutoff = Value("MultiModeFilter", "CUTOFF");

    // Value &cutoff = hostValue("MultiModeFilter", "CUTOFF");
    // Value &resonance = hostValue("MultiModeFilter", "RESONANCE");
    // Value &sampleReducer = hostValue("SampleRateReducer", "SAMPLE_STEP");
    // Value &distortion = hostValue("Distortion", "DRIVE");

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
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
        encoders.set(EncodersProps{
            {{[](int8_t direction)
              {
                  ViewPad &pad = ViewPad::get();
                  uint8_t cutoff = rangeMidi((pad.cutoff.get() * 128.0) + direction);
                  pad.cutoff.set(cutoff / 128.0);
                  pad.encoders.render(0);
                  drawNext();
              },
              [](Point position)
              {
                  ViewPad &pad = ViewPad::get();
                  uint8_t cutoff = pad.cutoff.get() * 128;
                  drawCenteredEncoder(position, cutoff > 63 ? "High pass filter" : "Low pass filter", cutoff);
              }},
             {[](int8_t direction)
              {
                  ViewPad &pad = ViewPad::get();
                  pad.resonance = rangeMidi(pad.resonance + direction);
                  pad.encoders.render(1);
                  drawNext();
              },
              [](Point position)
              {
                  ViewPad &pad = ViewPad::get();
                  drawEncoder(position, "Resonance", pad.resonance);
              }},
             {[](int8_t direction)
              {
                  ViewPad &pad = ViewPad::get();
                  pad.sampleReducer = rangeMidi(pad.sampleReducer + direction);
                  printf("SampleReducer %d\n", pad.sampleReducer);
              }},
             {[](int8_t direction)
              {
                  ViewPad &pad = ViewPad::get();
                  pad.distortion = rangeMidi(pad.distortion + direction);
                  printf("Distortion %d\n", pad.distortion);
              }}},
        });
    }

public:
    ComponentEncoders &encoders = ComponentEncoders::get();

    uint8_t resonance = 0;
    uint8_t sampleReducer = 0;
    uint8_t distortion = 0;

    // AudioPlugin &effectPlugin = getPlugin("MultiModeFilter");
    // Value cutoff = Value(effectPlugin, "CUTOFF");
    Value cutoff = Value("MultiModeFilter", "CUTOFF");

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
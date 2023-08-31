#ifndef _UI_VIEW_PAD_H_
#define _UI_VIEW_PAD_H_

#include "def.h"
#include "view.h"
#include "draw.h"
#include "component/encoders.h"
#include "component/pad.h"
#include "draw/encoder.h"

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
                  pad.cutoff = rangeMidi(pad.cutoff + direction);
                  pad.encoders.render(0);
                  draw();
              },
              [](Point position)
              {
                  ViewPad &pad = ViewPad::get();
                  drawEncoder(position, "Cutoff", pad.cutoff);
              }},
             {[](int8_t direction)
              {
                  ViewPad &pad = ViewPad::get();
                  pad.resonance = rangeMidi(pad.resonance + direction);
                  printf("Resonance %d\n", pad.resonance);
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

    // temp values
    uint8_t cutoff = 64;
    uint8_t resonance = 0;
    uint8_t sampleReducer = 0;
    uint8_t distortion = 0;

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
};

ViewPad *ViewPad::instance = NULL;

#endif
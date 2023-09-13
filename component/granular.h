#ifndef _UI_COMPONENT_GRANULAR_H_
#define _UI_COMPONENT_GRANULAR_H_

#include "../def.h"
#include "../component.h"
#include "../draw.h"
#include "../host.h"

class ComponentGranular : public Component
{
protected:
    AudioPlugin &plugin;

    bool noteIsOn = false;

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.pad.background);

        uint64_t *samplesCount = (uint64_t *)plugin.data(0);
        float *bufferSamples = (float *)plugin.data(1);
        // printf("-------------------> samplesCount: %lu\n", *samplesCount);

        // loop through all samples and creat a graph
        int h = (size.h - 2 * margin) * 0.5f;
        for (int i = 0; i < *samplesCount; i++)
        {
            // drawBar({position.x + margin, position.y + margin},
            //         bufferSamples[i], *samplesCount);

            int x = position.x + margin + (i * (size.w - 2 * margin) / *samplesCount);
            float sample = bufferSamples[i];
            int sampleHeight = (int)(sample * h);
            int y1 = position.y + margin + (h - sampleHeight);
            int y2 = position.y + margin + (h + sampleHeight);
            drawLine({x, y1}, {x, y2}, colors.encoder.title);
        }
    }

public:
    const int margin = styles.margin;

    ComponentGranular(Point position, Size size)
        : Component(position, size), plugin(getPlugin("Granular"))
    {
    }

    void onMotion(Motion &motion)
    {
        if (!noteIsOn)
        {
            plugin.noteOn(48, 127);
            noteIsOn = true;
        }
    }

    void onMotionRelease(Motion &motion)
    {
        Component::onMotionRelease(motion);
        if (noteIsOn)
        {
            plugin.noteOff(48, 0);
            noteIsOn = false;
        }
    }
};

#endif

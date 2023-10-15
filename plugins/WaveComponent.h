#ifndef _UI_COMPONENT_WAVE_H_
#define _UI_COMPONENT_WAVE_H_

#include "../plugins/component.h"
#include <string>

class WaveComponent : public Component
{
protected:
    AudioPlugin &plugin;

    Size waveSize;
    Point wavePosition;
    void *textureWave = NULL;

    void render()
    {
        printf("Rendering wave\n");
        draw.filledRect(wavePosition, size, colors.background);

        uint64_t samplesCount = 1000;
        float *bufferSamples = (float *)plugin.data(0);
        int h = waveSize.h * 0.5f;
        for (int xIndex = 0; xIndex < waveSize.w; xIndex++)
        {
            int x = wavePosition.x + xIndex;
            int i = xIndex * samplesCount / waveSize.w;
            int y1 = wavePosition.y + (h - (int)(bufferSamples[i] * h));
            int y2 = wavePosition.y + (h + (int)(bufferSamples[i] * h));
            draw.line({x, y1}, {x, y2}, colors.samples);
            // draw.line({x, y1}, {x, y1}, colors.samples);
        }
    }

    struct Colors
    {
        Color background;
        Color samples;
    } colors;

    const int margin;

    int lastUpdateUi = -1;

public:
    WaveComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground, styles.colors.textDark}),
          margin(styles.margin),
          plugin(getPlugin("Kick23"))
    {
        waveSize = {size.w - 2 * margin, size.h - 2 * margin};
        wavePosition = {position.x + margin, position.y + margin};
    }

    virtual void triggerRenderer() override
    {
        int *last = (int *)plugin.data(1);
        if (*last != lastUpdateUi)
        {
            lastUpdateUi = *last + 0;
            needRendering = true;
        }
        Component::triggerRenderer();
    }
};

#endif

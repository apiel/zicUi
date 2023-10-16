#ifndef _UI_COMPONENT_WAVE_H_
#define _UI_COMPONENT_WAVE_H_

#include "../plugins/component.h"
#include <string>

#include <vector>

class WaveComponent : public Component
{
protected:
    AudioPlugin &plugin;

    Size waveSize;
    Point wavePosition;
    void *textureWave = NULL;

    struct Data
    {
        float modulation;
        float time;
    };

    void render()
    {
        draw.filledRect(wavePosition, size, colors.background);

        uint64_t samplesCount = 1000;
        float *bufferSamples = (float *)plugin.data(1);

        int h = waveSize.h * 0.5f;
        float yRatio = samplesCount / waveSize.w;
        for (int xIndex = 0; xIndex < waveSize.w - 1; xIndex++)
        {
            int x = wavePosition.x + xIndex;
            int i = xIndex * yRatio;
            int y1 = wavePosition.y + (h - (int)(bufferSamples[i] * h));
            int i2 = (xIndex + 1) * yRatio;
            int y2 = wavePosition.y + (h - (int)(bufferSamples[i2] * h));
            draw.line({x, y1}, {x + 1, y2}, colors.samples);
        }

        std::vector<Data> *dataAmp = (std::vector<Data> *)plugin.data(2);
        for (int i = 0; i < dataAmp->size() - 1; i++)
        {
            Data &data1 = dataAmp->at(i);
            Data &data2 = dataAmp->at(i + 1);
            draw.line({(int)(wavePosition.x + waveSize.w * data1.time),
                       (int)(wavePosition.y + waveSize.h - waveSize.h * data1.modulation)},
                      {(int)(wavePosition.x + waveSize.w * data2.time),
                       (int)(wavePosition.y + waveSize.h - waveSize.h * data2.modulation)},
                      colors.amp);
        }

        std::vector<Data> *dataFreq = (std::vector<Data> *)plugin.data(3);
        for (int i = 0; i < dataFreq->size() - 1; i++)
        {
            Data &data1 = dataFreq->at(i);
            Data &data2 = dataFreq->at(i + 1);
            draw.line({(int)(wavePosition.x + waveSize.w * data1.time),
                       (int)(wavePosition.y + waveSize.h - waveSize.h * data1.modulation)},
                      {(int)(wavePosition.x + waveSize.w * data2.time),
                       (int)(wavePosition.y + waveSize.h - waveSize.h * data2.modulation)},
                      colors.freq);
        }
    }

    struct Colors
    {
        Color background;
        Color samples;
        Color amp;
        Color freq;
    } colors;

    const int margin;

    int lastUpdateUi = -1;

public:
    WaveComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground, styles.colors.text, styles.colors.foreground2, styles.colors.foreground3}),
          margin(styles.margin),
          plugin(getPlugin("Kick23"))
    {
        waveSize = {size.w - 2 * margin, size.h - 2 * margin};
        wavePosition = {position.x + margin, position.y + margin};

        // colors.samples.a = 200;
    }

    virtual void triggerRenderer() override
    {
        int *last = (int *)plugin.data(0);
        if (*last != lastUpdateUi)
        {
            lastUpdateUi = *last + 0;
            needRendering = true;
        }
        Component::triggerRenderer();
    }
};

#endif

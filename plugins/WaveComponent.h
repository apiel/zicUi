#ifndef _UI_COMPONENT_WAVE_H_
#define _UI_COMPONENT_WAVE_H_

#include "../plugins/component.h"
#include <string>

class WaveComponent : public Component
{
protected:
    AudioPlugin &plugin;

    Size textureSize;
    void *textureWave = NULL;

    void render()
    {
        if (textureWave == NULL)
        {
            textureWave = draw.setTextureRenderer(textureSize);

            draw.filledRect({0, 0}, {textureSize.w, textureSize.h}, colors.background);

            uint64_t samplesCount = 1000;
            float *bufferSamples = (float *)plugin.data(0);
            int h = textureSize.h;
            for (int xIndex = 0; xIndex < textureSize.w; xIndex++)
            {
                int x = margin + xIndex;
                int i = xIndex * samplesCount / textureSize.w;
                int y1 = margin + (h - (int)(bufferSamples[i] * h));
                int y2 = margin + (h + (int)(bufferSamples[i] * h));
                draw.line({x, y1}, {x, y2}, colors.samples);
            }

            draw.setMainRenderer();
        }
        draw.applyTexture(textureWave, {{position.x + margin, position.y + margin}, textureSize});
    }

    struct Colors
    {
        Color background;
        Color samples;
    } colors;

    const int margin;

public:
    WaveComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground, styles.colors.textDark}),
          margin(styles.margin),
          plugin(getPlugin("Kick23"))
    {
        textureSize = {size.w - 2 * margin, size.h - 2 * margin};
    }

    virtual void triggerRenderer() override
    {
        // if (lastBrowser != browser->get())
        // {
        //     if (textureSampleWaveform != NULL)
        //     {
        //         draw.destroyTexture(textureSampleWaveform);
        //         textureSampleWaveform = NULL;
        //     }
        //     needRendering = true;
        // }
        Component::triggerRenderer();
    }
};

#endif

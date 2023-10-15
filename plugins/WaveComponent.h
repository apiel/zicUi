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

    ValueInterface *val1 = val(getPlugin("Kick23").getValue("ENVELOP_AMP_MOD_1"));
    ValueInterface *val2 = val(getPlugin("Kick23").getValue("ENVELOP_AMP_MOD_2"));
    ValueInterface *val3 = val(getPlugin("Kick23").getValue("ENVELOP_AMP_MOD_3"));
    ValueInterface *val4 = val(getPlugin("Kick23").getValue("ENVELOP_AMP_MOD_4"));
    ValueInterface *val5 = val(getPlugin("Kick23").getValue("ENVELOP_AMP_TIME_1"));
    ValueInterface *val6 = val(getPlugin("Kick23").getValue("ENVELOP_AMP_TIME_2"));
    ValueInterface *val7 = val(getPlugin("Kick23").getValue("ENVELOP_AMP_TIME_3"));
    ValueInterface *val8 = val(getPlugin("Kick23").getValue("ENVELOP_AMP_TIME_4"));
    ValueInterface *val9 = val(getPlugin("Kick23").getValue("ENVELOP_FREQ_MOD_1"));
    ValueInterface *val10 = val(getPlugin("Kick23").getValue("ENVELOP_FREQ_MOD_2"));
    ValueInterface *val11 = val(getPlugin("Kick23").getValue("ENVELOP_FREQ_MOD_3"));
    ValueInterface *val12 = val(getPlugin("Kick23").getValue("ENVELOP_FREQ_MOD_4"));
    ValueInterface *val13 = val(getPlugin("Kick23").getValue("ENVELOP_FREQ_TIME_1"));
    ValueInterface *val14 = val(getPlugin("Kick23").getValue("ENVELOP_FREQ_TIME_2"));
    ValueInterface *val15 = val(getPlugin("Kick23").getValue("ENVELOP_FREQ_TIME_3"));
    ValueInterface *val16 = val(getPlugin("Kick23").getValue("ENVELOP_FREQ_TIME_4"));

    void render()
    {
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

    // virtual void triggerRenderer() override
    // {
    //     // if (lastBrowser != browser->get())
    //     // {
    //         if (textureWave != NULL)
    //         {
    //             draw.destroyTexture(textureWave);
    //             textureWave = NULL;
    //         }
    //         needRendering = true;
    //     // }
    //     Component::triggerRenderer();
    // }
};

#endif

#ifndef _UI_COMPONENT_GRANULAR_H_
#define _UI_COMPONENT_GRANULAR_H_

#include "../plugins/component.h"
#include <string>

class GranularComponent : public Component
{
protected:
    AudioPlugin &plugin;
    ValueInterface *browser = hostValue("Granular", "BROWSER");
    float lastBrowser = -1.0f;
    ValueInterface *start = hostValue("Granular", "START");
    float lastStart = -1.0f;
    ValueInterface *grainSize = hostValue("Granular", "GRAIN_SIZE");
    float lastGrainSize = -1.0f;
    ValueInterface *spray = hostValue("Granular", "SPRAY");
    float lastSpray = -1.0f;
    ValueInterface *density = hostValue("Granular", "DENSITY");
    float lastDensity = -1.0f;

    MotionInterface *motion1 = NULL;
    MotionInterface *motion2 = NULL;

    Size textureSize;
    void *textureSampleWaveform = NULL;

    // FIXME
    // TODO This how to handle texture
    void renderSampleWaveform()
    {
        if (textureSampleWaveform == NULL)
        {
            lastBrowser = browser->get();
            textureSampleWaveform = draw.setTextureRenderer(textureSize);

            draw.filledRect({0, 0}, {textureSize.w, textureSize.h}, colors.background);

            uint64_t *samplesCount = (uint64_t *)plugin.data(0);
            float *bufferSamples = (float *)plugin.data(1);
            int h = textureSize.h * 0.5f;
            for (int i = 0; i < *samplesCount; i++)
            {
                int x = margin + (i * textureSize.w / *samplesCount);
                int y1 = margin + (h - (int)(bufferSamples[i] * h));
                int y2 = margin + (h + (int)(bufferSamples[i] * h));
                draw.line({x, y1}, {x, y2}, colors.samples);
            }

            draw.text({10, 5}, browser->string(), colors.info, 12);
            draw.setMainRenderer();
        }
        draw.applyTexture(textureSampleWaveform, {{position.x + margin, position.y + margin}, textureSize});
    }

    void renderStartRange()
    {
        int x = position.x + margin + (start->get() * (textureSize.w));
        int w = (grainSize->get() * (textureSize.w));
        if (x + w > position.x + textureSize.w)
        {
            w -= (x + w) - (position.x + textureSize.w);
        }
        draw.filledRect({x, position.y + margin}, {w, textureSize.h}, colors.start);
    }

    void renderInfo()
    {
        char info[256];
        snprintf(info, sizeof(info), "Start %d%% Size %d%% Spray %d%% Density %d",
                 (int)(lastStart * 100), (int)(lastGrainSize * 100), (int)(lastSpray * 100), (int)(lastDensity * density->props().stepCount + density->props().stepStart));
        draw.text({position.x + margin + 10, position.y + size.h - 20 - margin}, info, colors.info, 12);
    }

    void render()
    {
        renderSampleWaveform();
        renderInfo();
        renderStartRange();
    }

    struct Colors
    {
        Color background;
        Color info;
        Color samples;
        Color start;
    } colors;

    const int margin;

public:
    GranularComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground, styles.colors.foreground2, styles.colors.textDark, styles.colors.overlay}),
          margin(styles.margin),
          plugin(getPlugin("Granular"))
    {
        textureSize = {size.w - 2 * margin, size.h - 2 * margin};
    }

    virtual void triggerRenderer() override
    {
        if (lastBrowser != browser->get())
        {
            if (textureSampleWaveform != NULL)
            {
                draw.destroyTexture(textureSampleWaveform);
                textureSampleWaveform = NULL;
            }
            needRendering = true;
        }
        if (lastStart != start->get() || lastGrainSize != grainSize->get() || lastSpray != spray->get() || lastDensity != density->get())
        {
            lastStart = start->get();
            lastGrainSize = grainSize->get();
            lastSpray = spray->get();
            lastDensity = density->get();
            needRendering = true;
        }
        Component::triggerRenderer();
    }

    float startOrigin = 0.0;
    float sizeOrigin = 0.0;
    void onMotion(MotionInterface &motion)
    {
        if (motion1 == NULL)
        {
            plugin.noteOn(48, 127);
            motion1 = &motion;
            startOrigin = start->get();
        }

        if (motion1 == &motion)
        {
            float x = startOrigin + (motion.position.x - motion.origin.x) / (float)(textureSize.w);
            if (x - start->get() > 0.01 || start->get() - x > 0.01)
            {
                start->set(x);
            }

            float rangeMargin = 40;
            float y = 1.0 - (motion.position.y - position.y - margin - rangeMargin) / (float)(textureSize.h - (rangeMargin * 2));
            if (y - spray->get() > 0.01 || spray->get() - y > 0.01)
            {
                spray->set(y);
            }
        }
        else if (motion2 == NULL)
        {
            motion2 = &motion;
            sizeOrigin = grainSize->get();
        }

        if (motion2 == &motion)
        {
            // float x = startOrigin + (motion.position.x - motion.origin.x) / (float)(textureSize.w);
            float x = sizeOrigin + (motion.position.x - motion.origin.x) / (float)(textureSize.w);
            if (x - grainSize->get() > 0.01 || grainSize->get() - x > 0.01)
            {
                grainSize->set(x);
            }

            float rangeMargin = 40;
            float y = 1.0 - (motion.position.y - position.y - margin - rangeMargin) / (float)(textureSize.h - (rangeMargin * 2));
            if (y - density->get() > 0.01 || density->get() - y > 0.01)
            {
                density->set(y);
            }
        }
    }

    void onMotionRelease(MotionInterface &motion)
    {
        Component::onMotionRelease(motion);
        if (motion1 == &motion)
        {
            motion1 = NULL;
            plugin.noteOff(48, 0);
        }
        if (motion2 == &motion)
        {
            motion2 = NULL;
        }
    }
};

#endif

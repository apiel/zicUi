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
    Value *browser = hostValue({"Granular", "BROWSER"});
    float lastBrowser = -1.0f;
    Value *start = hostValue({"Granular", "START"});
    float lastStart = -1.0f;
    Value *grainSize = hostValue({"Granular", "GRAIN_SIZE"});
    float lastGrainSize = -1.0f;
    Value *spray = hostValue({"Granular", "SPRAY"});
    float lastSpray = -1.0f;

    bool noteIsOn = false;

    Size textureSize;
    SDL_Texture *textureSampleWaveform = NULL;

    void renderSampleWaveform()
    {
        if (textureSampleWaveform == NULL)
        {
            lastBrowser = browser->get();
            textureSampleWaveform = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, textureSize.w, textureSize.h);
            SDL_SetRenderTarget(renderer, textureSampleWaveform);

            drawFilledRect({0, 0}, {textureSize.w, textureSize.h}, colors.granular.background);

            uint64_t *samplesCount = (uint64_t *)plugin.data(0);
            float *bufferSamples = (float *)plugin.data(1);
            int h = textureSize.h * 0.5f;
            for (int i = 0; i < *samplesCount; i++)
            {
                int x = margin + (i * textureSize.w / *samplesCount);
                int y1 = margin + (h - (int)(bufferSamples[i] * h));
                int y2 = margin + (h + (int)(bufferSamples[i] * h));
                drawLine({x, y1}, {x, y2}, colors.granular.samples);
            }

            drawText({10, 5}, browser->string(), colors.granular.info, 12);
            SDL_SetRenderTarget(renderer, texture);
        }
        SDL_Rect rect = {position.x + margin, position.y + margin, textureSize.w, textureSize.h};
        SDL_RenderCopy(renderer, textureSampleWaveform, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    void renderStartRange()
    {
        int x = position.x + margin + (start->get() * (textureSize.w));
        int w = (grainSize->get() * (textureSize.w));
        if (x + w > position.x + textureSize.w)
        {
            w -= (x + w) - (position.x + textureSize.w);
        }
        drawFilledRect({x, position.y + margin}, {w, textureSize.h}, colors.granular.start);
    }

    void renderInfo()
    {
        char info[256];
        snprintf(info, sizeof(info), "Start: %d%% Size: %d%% Spray: %d%%",
                 (int)(lastStart * 100), (int)(lastGrainSize * 100), (int)(lastSpray * 100));
        drawText({position.x + margin + 10, position.y + size.h - 20 - margin}, info, colors.granular.info, 12);
    }

    void render()
    {
        renderSampleWaveform();
        renderInfo();
        renderStartRange();
    }

public:
    const int margin = styles.margin;

    ComponentGranular(Point position, Size size)
        : Component(position, size), plugin(getPlugin("Granular"))
    {
        textureSize = {size.w - 2 * margin, size.h - 2 * margin};
    }

    virtual void triggerRenderer() override
    {
        if (lastBrowser != browser->get())
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Sample browser position changed, clear textureSampleWaveform.");
            SDL_DestroyTexture(textureSampleWaveform);
            textureSampleWaveform = NULL;
            needRendering = true;
        }
        if (lastStart != start->get() || lastGrainSize != grainSize->get() || lastSpray != spray->get())
        {
            lastStart = start->get();
            lastGrainSize = grainSize->get();
            lastSpray = spray->get();
            needRendering = true;
        }
        Component::triggerRenderer();
    }

    float xStart = 0.0;
    void onMotion(Motion &motion)
    {
        if (!noteIsOn)
        {
            plugin.noteOn(48, 127);
            noteIsOn = true;
        }

        if (motion.isStarting())
        {
            xStart = start->get();
        }
        float x = xStart + (motion.position.x - motion.origin.x) / (float)(textureSize.w);
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

        // y change spray / or density
        // Second point change x the grain size, y the spray / or density
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

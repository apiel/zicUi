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
    Value *start = hostValue({"Granular", "START"});

    bool noteIsOn = false;

    SDL_Texture *textureSampleWaveform = NULL;
    float lastSampleBrowserPosition = -1.0f;

    void renderSampleWaveform()
    {
        Size textureSize = {size.w - 2 * margin, size.h - 2 * margin};
        if (textureSampleWaveform == NULL)
        {
            lastSampleBrowserPosition = browser->get();
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

            drawText({10, 5}, browser->string(), colors.granular.sampleName, 12);
            SDL_SetRenderTarget(renderer, texture);
        }
        SDL_Rect rect = {position.x + margin, position.y + margin, textureSize.w, textureSize.h};
        SDL_RenderCopy(renderer, textureSampleWaveform, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    void render()
    {
        renderSampleWaveform();

        int x = position.x + margin + (start->get() * (size.w - 2 * margin));
        drawLine({x, position.y + margin}, {x, position.y + margin + size.h}, colors.granular.start);
    }

public:
    const int margin = styles.margin;

    ComponentGranular(Point position, Size size)
        : Component(position, size), plugin(getPlugin("Granular"))
    {
        if (start != NULL)
        {
            start->onUpdate([](float, void *data)
                            { ((ComponentGranular *)data)->renderNext(); },
                            this);
        }
    }

    virtual void triggerRenderer() override
    {
        if (lastSampleBrowserPosition != browser->get())
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Sample browser position changed, clear textureSampleWaveform.");
            SDL_DestroyTexture(textureSampleWaveform);
            textureSampleWaveform = NULL;
            needRendering = true;
        }
        Component::triggerRenderer();
    }

    void onMotion(Motion &motion)
    {
        if (!noteIsOn)
        {
            plugin.noteOn(48, 127);
            noteIsOn = true;
        }

        float x = (motion.position.x - position.x - margin) / (float)(size.w - 2 * margin);
        if (x - start->get() > 0.01 || start->get() - x > 0.01)
        {
            start->set(x);
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

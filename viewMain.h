#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include <vector>

#include "view.h"
#include "draw.h"
#include "def.h"
#include "component.h"
#include "component/components.h"

class ViewMain : public View
{
protected:
    // TODO add multiple screen/view/page
    std::vector<Component *> components;

    static ViewMain *instance;

    ViewMain()
    {
        // TODO load component from config file
        addComponent("Encoder", {0, 0}, styles.encoder);
        addComponent("Encoder", {SCREEN_W / ENCODER_COUNT, 0}, styles.encoder);
        addComponent("Encoder", {SCREEN_W / ENCODER_COUNT * 2, 0}, styles.encoder);
        addComponent("Encoder", {SCREEN_W / ENCODER_COUNT * 3, 0}, styles.encoder);
        int pad_h = SCREEN_H - styles.encoder.h;
        int pad_y = styles.encoder.h;
        int halfScreen_w = SCREEN_W / 2;
        addComponent("Pad", {0, pad_y}, {halfScreen_w, pad_h});
        addComponent("Granular", {halfScreen_w, pad_y}, {halfScreen_w, 300});
    }

public:
    static ViewMain &get()
    {
        if (!instance)
        {
            instance = new ViewMain();
        }
        return *instance;
    }

    void render()
    {
        drawClear();
        for (auto &component : components)
        {
            component->render();
        }
        drawNext();
    }

    void onMotion(Motion &motion)
    {
        for (auto &component : components)
        {
            component->onMotion(motion);
        }
    }

    void onMotionRelease(Motion &motion)
    {
        for (auto &component : components)
        {
            component->onMotionRelease(motion);
        }
    }

    void onEncoder(int id, int8_t direction)
    {
        for (auto &component : components)
        {
            component->onEncoder(id, direction);
        }
    }

    void addComponent(const char *name, Point position, Size size)
    {
        if (strcmp(name, "Pad") == 0)
        {
            components.push_back(new ComponentPad(position, size));
        }
        else if (strcmp(name, "Encoder") == 0)
        {
            components.push_back(new ComponentEncoder(position, size));
        }
        else if (strcmp(name, "Granular") == 0)
        {
            components.push_back(new ComponentGranular(position, size));
        }
        else
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown component: %s", name);
        }
    }
};

ViewMain *ViewMain::instance = NULL;

#endif
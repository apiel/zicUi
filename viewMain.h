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
        renderComponents();
        drawNext();
    }

    void renderComponents()
    {
        for (auto &component : components)
        {
            component->triggerRenderer();
        }
    }

    void onMotion(Motion &motion)
    {
        for (auto &component : components)
        {
            component->handleMotion(motion);
        }
    }

    void onMotionRelease(Motion &motion)
    {
        for (auto &component : components)
        {
            component->handleMotionRelease(motion);
        }
    }

    void onEncoder(int id, int8_t direction)
    {
        for (auto &component : components)
        {
            component->onEncoder(id + encoderRootIndex, direction);
        }
    }

    void addComponent(const char *name, Point position, Size size)
    {
        if (strcmp(name, "Pad") == 0)
        {
            components.push_back(new ComponentPad(position, size));
        }
        else if (strcmp(name, "Value") == 0)
        {
            components.push_back(new ComponentValue(position, size));
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

    void config(char *key, char *value)
    {
        if (strcmp(key, "COMPONENT") == 0)
        {
            char * name = strtok(value, " ");
            Point position = {atoi(strtok(NULL, " ")), atoi(strtok(NULL, " "))};
            Size size = {atoi(strtok(NULL, " ")), atoi(strtok(NULL, " "))};
            addComponent(name, position, size); 
        }
        else
        {
            components.back()->baseConfig(key, value);
        }
    }

    void config(const char *key, const char *value)
    {

        config((char *)key, (char *)value);
    }
};

ViewMain *ViewMain::instance = NULL;

#endif
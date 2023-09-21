#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include <vector>

#include "styles.h"
#include "view.h"
#include "draw.h"
#include "state.h"
#include "plugins/componentInterface.h"
#include "host.h"
#include "plugins/PadComponent.h"
#include "plugins/EncoderComponent.h"
#include "plugins/GranularComponent.h"

class ViewMain : public View
{
protected:
    // TODO add multiple screen/view/page
    std::vector<ComponentInterface *> components;
    int8_t lastGroup = -100;

    static ViewMain *instance;

    ViewMain()
    : draw(styles)
    {
    }

public:
    Draw draw;

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
        draw.clear();
        draw.next();
        renderComponents();
    }

    void renderComponents()
    {
        if (group != lastGroup)
        {
            lastGroup = group;
            for (auto &component : components)
            {
                component->onGroupChanged(group);
            }
        }
        for (auto &component : components)
        {
            component->triggerRenderer();
        }
        draw.triggerRender();
    }

    void onMotion(MotionInterface &motion)
    {
        for (auto &component : components)
        {
            component->handleMotion(motion);
        }
    }

    void onMotionRelease(MotionInterface &motion)
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
            component->onEncoder(id, direction);
        }
    }

    void addComponent(const char *name, Point position, Size size)
    {
        ComponentInterface::Props props = {position, size, draw, getPlugin, hostValue, setGroup};
        if (strcmp(name, "Pad") == 0)
        {
            components.push_back(new PadComponent(props));
        }
        else if (strcmp(name, "Encoder") == 0)
        {
            components.push_back(new EncoderComponent(props));
        }
        else if (strcmp(name, "Granular") == 0)
        {
            components.push_back(new GranularComponent(props));
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
        else if (components.size() > 0)
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
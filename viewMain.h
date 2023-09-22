#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include <vector>
#include <dlfcn.h>

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

    struct Plugin
    {
        char name[64];
        ComponentInterface *(*allocator)(ComponentInterface::Props &props);
    };
    std::vector<Plugin> plugins;

    void loadPlugin(char *value)
    {
        Plugin plugin;
        strcpy(plugin.name, strtok(value, " "));
        char *path = strtok(NULL, " ");

        void *handle = dlopen(path, RTLD_LAZY);

        if (!handle)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot open library: %s\n", dlerror());
            return;
        }

        dlerror();
        plugin.allocator = (ComponentInterface * (*)(ComponentInterface::Props & props)) dlsym(handle, "allocator");
        const char *dlsym_error = dlerror();
        if (dlsym_error)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot load symbol: %s\n", dlsym_error);
            dlclose(handle);
            return;
        }
        plugins.push_back(plugin);
    }

    void addComponent(const char *name, Point position, Size size)
    {
        ComponentInterface::Props props = {position, size, draw, getPlugin, hostValue, setGroup};

        for (auto &plugin : plugins)
        {
            if (strcmp(plugin.name, name) == 0)
            {
                ComponentInterface *component = plugin.allocator(props);
                components.push_back(component);
                return;
            }
        }
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown component: %s", name);
        // for (auto &plugin : plugins)
        // {
        //     printf("plugin differ: %s vs %s\n", plugin.name, name);
        // }
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

    bool config(char *key, char *value)
    {
        if (strcmp(key, "PLUGIN_COMPONENT") == 0)
        {
            loadPlugin(value);
            return true;
        }
        else if (strcmp(key, "COMPONENT") == 0)
        {
            char *name = strtok(value, " ");
            Point position = {atoi(strtok(NULL, " ")), atoi(strtok(NULL, " "))};
            Size size = {atoi(strtok(NULL, " ")), atoi(strtok(NULL, " "))};
            addComponent(name, position, size);
            return true;
        }
        else if (components.size() > 0)
        {
            return components.back()->baseConfig(key, value);
        }
        return false;
    }

    void config(const char *key, const char *value)
    {

        config((char *)key, (char *)value);
    }
};

ViewMain *ViewMain::instance = NULL;

#endif
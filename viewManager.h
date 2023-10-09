#ifndef _VIEW_MANAGER_H
#define _VIEW_MANAGER_H

#include <vector>
#include <dlfcn.h>

#include "styles.h"
#include "draw.h"
#include "state.h"
#include "plugins/componentInterface.h"
#include "host.h"
#include "UiPlugin.h"

class ViewManager
{
protected:
    UiPlugin &ui = UiPlugin::get();
    int8_t lastGroup = -100;

    static ViewManager *instance;

    ViewManager()
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
        ComponentInterface::Props props = {position, size, draw, getPlugin, setGroup};

        for (auto &plugin : plugins)
        {
            if (strcmp(plugin.name, name) == 0)
            {
                ComponentInterface *component = plugin.allocator(props);
                ui.addComponent(component);
                return;
            }
        }
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown component: %s", name);
    }

public:
    Draw draw;

    static ViewManager &get()
    {
        if (!instance)
        {
            instance = new ViewManager();
        }
        return *instance;
    }

    bool init()
    {
        if (!ui.getViewCount())
        {
            return false;
        }

        for (auto &component : ui.getView())
        {
            for (auto *value : component->values)
            {
                // TODO could this be optimized by creating mapping values to components?
                value->onUpdate([](float, void *data)
                                { ViewManager::get().onUpdate((ValueInterface *)data); },
                                value);
            }
        }

        ui.viewSelector.onUpdate([](float, void *data)
                                 { ViewManager::get().render(true); },
                                 NULL);
        return true;
    }

    // TODO could this be optimized by creating mapping values to components?
    void onUpdate(ValueInterface *val)
    {
        for (auto &component : ui.getView())
        {
            for (auto *value : component->values)
            {
                if (value == val)
                {
                    component->onUpdate(value);
                }
            }
        }
    }

    void render(bool forceReRender = false)
    {
        draw.clear();
        draw.next();
        if (forceReRender)
        {
            for (auto &component : ui.getView())
            {
                component->renderNext();
            }
        }
        renderComponents();
    }

    void renderComponents()
    {
        if (group != lastGroup)
        {
            lastGroup = group;
            for (auto &component : ui.getView())
            {
                component->onGroupChanged(group);
            }
        }
        for (auto &component : ui.getView())
        {
            component->triggerRenderer();
        }
        draw.triggerRender();
    }

    void onMotion(MotionInterface &motion)
    {
        for (auto &component : ui.getView())
        {
            component->handleMotion(motion);
        }
    }

    void onMotionRelease(MotionInterface &motion)
    {
        for (auto &component : ui.getView())
        {
            component->handleMotionRelease(motion);
        }
    }

    void onEncoder(int id, int8_t direction)
    {
        for (auto &component : ui.getView())
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
        if (strcmp(key, "COMPONENT") == 0)
        {
            char *name = strtok(value, " ");
            Point position = {atoi(strtok(NULL, " ")), atoi(strtok(NULL, " "))};
            Size size = {atoi(strtok(NULL, " ")), atoi(strtok(NULL, " "))};
            addComponent(name, position, size);
            return true;
        }
        return ui.config(key, value);
    }

    void config(const char *key, const char *value)
    {

        config((char *)key, (char *)value);
    }
};

ViewManager *ViewManager::instance = NULL;

#endif
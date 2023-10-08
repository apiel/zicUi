#ifndef _UI_PLUGIN_H_
#define _UI_PLUGIN_H_

// #include "plugins/valueInterface.h"
#include "../zicHost/plugins/mapping.h"

#include <cstdio> // printf

/**
 * Simulate audio plugin component to be usable within the UI component
 */
class UiPlugin : public Mapping<UiPlugin>
{
protected:
    struct View
    {
        char *name;
        std::vector<ComponentInterface *> view = {};
    };

    std::vector<View *> views;
    int viewIndex = -1;

    static UiPlugin *instance;
    AudioPlugin::Props props = {NULL, 0, 0, NULL, 0};
    UiPlugin() : Mapping(props, (char *)"UI")
    {
    }

public:
    Val<UiPlugin> &viewSelector = val(this, 1.0f, "VIEW", &UiPlugin::setView, {"View", 1, VALUE_STRING});

    static UiPlugin &get()
    {
        if (!instance)
        {
            instance = new UiPlugin();
        }
        return *instance;
    }

    void sample(float *buf) {}

    UiPlugin &setView(float value)
    {
        viewSelector.setFloat(value);
        viewIndex = viewSelector.getAsInt();

        viewSelector.setString(views[viewIndex]->name);

        // printf("................... Set view to %f => %d > %s\n", viewSelector.get(), viewIndex, viewSelector.string());

        return *this;
    }

    int getViewCount()
    {
        return views.size();
    }

    std::vector<ComponentInterface *> &getView()
    {
        return views[viewIndex]->view;
    }

    bool config(char *key, char *value)
    {
        if (strcmp(key, "VIEW") == 0)
        {
            View *v = new View;
            v->name = new char[strlen(value) + 1];
            strcpy(v->name, value);

            views.push_back(v);

            viewSelector.props().stepCount = views.size();

            setView(1.0f); // To set to last
            // setView(0.0f);

            return true;
        }

        if (views.size() > 0 && views.back()->view.size() > 0)
        {
            return views.back()->view.back()->baseConfig(key, value);
        }

        return false;
    }

    void addComponent(ComponentInterface *component)
    {
        if (views.size() > 0)
        {
            views.back()->view.push_back(component);
        }
        else
        {
            printf("ERROR: No view to add component to. Create first a view to be able to add components.\n");
        }
    }
};

UiPlugin *UiPlugin::instance = NULL;

#endif
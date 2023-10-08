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
        std::vector<ComponentInterface *> *view = new std::vector<ComponentInterface *>({});
    };

    std::vector<View> views = {{(char *)"Main", new std::vector<ComponentInterface *>({})}};
    std::vector<ComponentInterface *> &view = *views[0].view;

    static UiPlugin *instance;
    AudioPlugin::Props props = {NULL, 0, 0, NULL, 0};
    UiPlugin() : Mapping(props, (char *)"UI")
    {
        setView(0.0f);
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
        uint viewIndex = viewSelector.getAsInt();

        viewSelector.setString(views[viewIndex].name);

        printf("................... Set view to %f => %d > %s\n", viewSelector.get(), viewIndex, viewSelector.string());

        return *this;
    }

    std::vector<ComponentInterface *> &getView()
    {
        return view;
    }

    bool config(char *key, char *value)
    {
        if (strcmp(key, "VIEW") == 0)
        {
            View* v = new View;
            v->name = new char[strlen(value) + 1];
            strcpy(v->name, value);

            views.push_back(*v);
            view = *views.back().view;

            viewSelector.props().stepCount = views.size();

            return true;
        }

        if (view.size() > 0)
        {
            return view.back()->baseConfig(key, value);
        }

        return false;
    }

    void addComponent(ComponentInterface *component)
    {
        if (views.size() > 0)
        {
            view.push_back(component);
            // views.back().view->push_back(component);
        }
        else
        {
            printf("ERROR: No view to add component to. Create first a view to be able to add components.\n");
        }
    }
};

UiPlugin *UiPlugin::instance = NULL;

#endif
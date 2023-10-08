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
    static UiPlugin *instance;
    AudioPlugin::Props props = {NULL, 0, 0, NULL, 0};
    UiPlugin() : Mapping(props, (char *)"UI") {}

public:
    std::vector<std::vector<ComponentInterface *> *> views = {new std::vector<ComponentInterface *>({})};
    std::vector<ComponentInterface *> &view = *views[0];

    Val<UiPlugin> &viewSelector = val(this, 1.0f, "VIEW", &UiPlugin::setView, {"View"});

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
        printf("Set view to %f\n", value);
        viewSelector.setFloat(value);
        return *this;
    }

    bool config(char *key, char *value)
    {
        if (strcmp(key, "VIEW") == 0)
        {
            if (strcmp(value, "NEXT") == 0)
            {
                views.push_back(new std::vector<ComponentInterface *>());
                view = *views[views.size() - 1];
                return true;
            }
        }
        else if (view.size() > 0)
        {
            return view.back()->baseConfig(key, value);
        }
        return false;
    }
};

UiPlugin *UiPlugin::instance = NULL;

#endif
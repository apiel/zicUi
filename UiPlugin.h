#ifndef _UI_PLUGIN_H_
#define _UI_PLUGIN_H_

// #include "plugins/valueInterface.h"
#include "../zicHost/plugins/mapping.h"

#include <cstdio> // printf

class UiPlugin : public Mapping<UiPlugin>
{
protected:
    static UiPlugin *instance;
    AudioPlugin::Props props = {NULL, 0, 0, NULL, 0};
    UiPlugin() : Mapping(props, (char *)"UI") {}

public:
    Val<UiPlugin> &view = val(this, 1.0f, "VIEW", &UiPlugin::setView, {"View"});

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
        view.setFloat(value);
        return *this;
    }
};

UiPlugin* UiPlugin::instance = NULL;

#endif
#ifndef _VALUE_INTERFACE_H_
#define _VALUE_INTERFACE_H_

// FIXME
#include "../../zicHost/plugin.h"

struct ValueProps
{
    const char *pluginName;
    const char *key;
};


class ValueInterface
{
protected:
    int index = -1;

public:
    AudioPlugin::ValueProps *props;
    AudioPlugin &plugin;
    const char *valueKey;

    ValueInterface(AudioPlugin &plugin, int index)
        : plugin(plugin), index(index)
    {
        valueKey = plugin.getValueKey(index);
        props = plugin.getValueProps(index);
    }

    virtual float get() = 0;
    virtual char *string() = 0;
    virtual void set(float value) = 0;
    virtual const char *key() = 0;
    virtual const char *label() = 0;
    virtual void onUpdate(void (*callback)(float, void *data), void *data) = 0;
};

#endif
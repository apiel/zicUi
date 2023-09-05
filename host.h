#ifndef _HOST_H_
#define _HOST_H_

#include "def.h"
#include <dlfcn.h>
#include <vector>
#include <stdexcept>

// FIXME
#include "../zicHost/plugin.h"

std::vector<Plugin> *plugins = NULL;

std::vector<Plugin> *(*initHost)() = NULL;
int (*mainLoopHost)() = NULL;
void (*midiHost)(std::vector<unsigned char> *message) = NULL;

bool loadHost();

int hostThread(void *data)
{
    return mainLoopHost();
}

void *linkHost(void *handle, const char *name)
{
    void *fn = dlsym(handle, name);
    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot load symbol: %s\n", dlsym_error);
        dlclose(handle);
        return NULL;
    }
    return fn;
}

AudioPlugin &getPlugin(const char *name)
{
    if (!plugins)
    {
        if (!loadHost())
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load host");
        }
    }

    for (Plugin &plugin : *plugins)
    {
        if (strcmp(plugin.instance->name(), name) == 0)
        {
            return *plugin.instance;
        }
    }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not find plugin: %s\n", name);
    throw std::runtime_error("Could not find plugin");
}

class Value
{
public:
    AudioPlugin &plugin;
    const char *key;
    int index = -1;

    Value(AudioPlugin &plugin, int index)
        : plugin(plugin), index(index)
    {
        key = plugin.getValueName(index);
    }

    Value(AudioPlugin &plugin, const char *key)
        : plugin(plugin), key(key)
    {
        for (int i = 0; i < plugin.getValueCount(); i++)
        {
            if (strcmp(plugin.getValueName(i), key) == 0)
            {
                index = i;
                break;
            }
        }
        // if index is still -1 should we throw?
    }

    Value(const char *pluginName, const char *key)
        : Value(getPlugin(pluginName), key)
    {
    }

    int stepCount()
    {
        return plugin.getStepCount(index);
    }

    float get()
    {
        return plugin.getValue(index);
    }

    void set(float value)
    {
        plugin.setValue(index, value);
    }

    const char *name()
    {
        return plugin.getValueName(index);
    }
};

std::vector<Value *> hostValues;
void loadHostValues()
{
    for (Plugin &plugin : *plugins)
    {
        AudioPlugin &audioPlugin = *plugin.instance;
        for (int i = 0; i < audioPlugin.getValueCount(); i++)
        {
            hostValues.push_back(new Value(audioPlugin, i));
        }
    }
}

Value * hostValue(const char *pluginName, const char *key)
{
    if (!plugins)
    {
        if (!loadHost())
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load host");
        }
    }
    for (Value *value : hostValues)
    {
        if (strcmp(value->key, key) == 0 && strcmp(value->plugin.name(), pluginName) == 0)
        {
            return value;
        }
    }
    return NULL;
}

bool loadHost()
{
    if (plugins)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Host already loaded\n");
        return true;
    }

    // FIXME
    const char *path = "../zicHost/zicHost.so";
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading host from %s\n", path);
    void *handle = dlopen(path, RTLD_LAZY);

    if (!handle)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading host (%s): %s\n", path, dlerror());
        return false;
    }

    dlerror(); // clear previous error

    initHost = (std::vector<Plugin> * (*)(void)) linkHost(handle, "init");
    if (!initHost)
    {
        return false;
    }

    mainLoopHost = (int (*)())linkHost(handle, "mainLoop");
    if (!mainLoopHost)
    {
        return false;
    }

    midiHost = (void (*)(std::vector<unsigned char> *))linkHost(handle, "midi");
    if (!midiHost)
    {
        return false;
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing host\n");
    plugins = initHost();
    if (!plugins)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing host\n");
        return false;
    }
    loadHostValues();

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting host in SDL thread\n");
    SDL_Thread *thread = SDL_CreateThread(hostThread, "host", NULL);

    return true;
}

#endif
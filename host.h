#ifndef _HOST_H_
#define _HOST_H_

#include "def.h"
#include <dlfcn.h>
#include <vector>

// FIXME
#include "../zicHost/plugin.h"

std::vector<Plugin> *plugins = NULL;

std::vector<Plugin> *(*initHost)() = NULL;
int (*mainLoopHost)() = NULL;
void (*midiHost)(std::vector<unsigned char> *message) = NULL;

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

AudioPlugin* getPlugin(const char *name)
{
    for (Plugin &plugin : *plugins)
    {
        if (strcmp(plugin.instance->name(), name) == 0)
        {
            return plugin.instance;
        }
    }
    return NULL;
}

bool loadHost()
{
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

    AudioPlugin * plugin = getPlugin("GainVolume");
    printf("-------> Name: %s\n", plugin->name());

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting host in SDL thread\n");
    SDL_Thread *thread = SDL_CreateThread(hostThread, "host", NULL);

    return true;
}

#endif
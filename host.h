#ifndef _HOST_H_
#define _HOST_H_

#include "def.h"
#include <dlfcn.h>
#include <vector>

void (*startHost)() = NULL;
void (*midiHost)(std::vector<unsigned char> *message) = NULL;

int hostThread(void *data)
{
    startHost();
    return 0;
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

void loadHost()
{
    const char *path = "../zicHost/zicHost.so";
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading host from %s", path);
    void *handle = dlopen(path, RTLD_LAZY);

    if (!handle)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading host (%s): %s\n", path, dlerror());
        return;
    }

    dlerror(); // clear previous error

    startHost = (void (*)())linkHost(handle, "start");
    if (!startHost)
    {
        return;
    }

    midiHost = (void (*)(std::vector<unsigned char> *))linkHost(handle, "midi");
    if (!midiHost)
    {
        return;
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting host in SDL thread");
    SDL_Thread *thread = SDL_CreateThread(hostThread, "host", NULL);
}

#endif
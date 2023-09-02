#ifndef _HOST_H_
#define _HOST_H_

#include "def.h"
#include <dlfcn.h>

void (*startHost)() = NULL;

int hostThread(void *data)
{
    startHost();
    return 0;
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

    dlerror();
    startHost = (void (*)())dlsym(handle, "start");
    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot load symbol: %s\n", dlsym_error);
        dlclose(handle);
        return;
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting host in SDL thread");
    SDL_Thread *thread = SDL_CreateThread(hostThread, "host", NULL);
}

#endif
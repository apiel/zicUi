#ifndef _HOST_H_
#define _HOST_H_

#include "def.h"
#include <dlfcn.h>
#include <vector>

bool (*initHost)() = NULL;
int (*mainLoopHost)() = NULL;
void (*midiHost)(std::vector<unsigned char> *message) = NULL;

float (*getValueHost)(int index) = NULL;
int (*getValueIndexHost)(const char * moduleName, const char * name) = NULL;

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

void loadHost()
{
    const char *path = "../zicHost/zicHost.so";
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading host from %s\n", path);
    void *handle = dlopen(path, RTLD_LAZY);

    if (!handle)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading host (%s): %s\n", path, dlerror());
        return;
    }

    dlerror(); // clear previous error

    initHost = (bool (*)())linkHost(handle, "init");
    if (!initHost)
    {
        return;
    }

    mainLoopHost = (int (*)())linkHost(handle, "mainLoop");
    if (!mainLoopHost)
    {
        return;
    }

    midiHost = (void (*)(std::vector<unsigned char> *))linkHost(handle, "midi");
    if (!midiHost)
    {
        return;
    }

    getValueHost = (float (*)(int))linkHost(handle, "getValue");
    if (!getValueHost)
    {
        return;
    }

    getValueIndexHost = (int (*)(const char *, const char *))linkHost(handle, "getValueIndex");
    if (!getValueIndexHost)
    {
        return;
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initializing host\n");
    if (!initHost()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing host\n");
        return;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting host in SDL thread\n");
    SDL_Thread *thread = SDL_CreateThread(hostThread, "host", NULL);
}

#endif
#ifndef _PLUGINS_H_
#define _PLUGINS_H_

#include "viewManager.h"
#include "plugins/controllerInterface.h"
#include "host.h"

#include <dlfcn.h>

ControllerInterface *lastPluginInstance = NULL;

void encoderHandler(int id, int8_t direction)
{
    ViewManager::get().onEncoder(id, direction);
}

void midiHandler(std::vector<unsigned char> *message)
{
    // should there be a if statement or should midiHost have a default value
    midiHost(message);
}

void loadPluginController(const char *path)
{
    void *handle = dlopen(path, RTLD_LAZY);

    if (!handle)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot open library: %s\n", dlerror());
        return;
    }

    dlerror();
    void *allocator = dlsym(handle, "allocator");
    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot load symbol: %s\n", dlsym_error);
        dlclose(handle);
        return;
    }

    ControllerInterface::Props props = {midiHandler, encoderHandler};
    lastPluginInstance = ((ControllerInterface * (*)(ControllerInterface::Props & props)) allocator)(props);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "plugin interface loaded: %s\n", path);
}

#endif

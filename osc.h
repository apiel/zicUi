#ifndef _OSC_INTERFACE_H_
#define _OSC_INTERFACE_H_

// sudo apt-get install liblo-dev

#include <lo/lo.h>
#include <vector>

#include "def.h"
#include "host.h"
#include "viewMain.h"

// TODO make configurable
char port[6] = "8888";

lo_server_thread serverThread;

ViewMain &viewMain = ViewMain::get();

void oscError(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

int exitOscHandler(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data)
{
    lo_server_thread_free(serverThread);
    exit(0);

    return 0;
}

int midiOscHandler(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data)
{
    std::vector<unsigned char> msg;
    for (int i = 0; i < argc; i++)
    {
        msg.push_back(argv[i]->c);
    }
    midiHost(&msg);

    return 0;
}

int encoderOscHandler(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data)
{
    viewMain.onEncoder(argv[0]->i, argv[1]->i);
    return 0;
}

void startOscServer()
{
    serverThread = lo_server_thread_new(port, oscError);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "OSC server listening on port %s\n", port);

    lo_server_thread_add_method(serverThread, "/midi", NULL, midiOscHandler, NULL);
    lo_server_thread_add_method(serverThread, "/encoder", "ii", encoderOscHandler, NULL);
    lo_server_thread_add_method(serverThread, "/exit", "", exitOscHandler, NULL);

    lo_server_thread_start(serverThread);
};

#endif

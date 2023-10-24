#ifndef _GET_FULLPATH_H
#define _GET_FULLPATH_H

#include <string.h>

char *getFullpath(char *path, const char *parentFilename)
{
    if (path[0] == '/')
    {
        char *fullpath = new char[strlen(path) + 1];
        strcpy(fullpath, path);
        return fullpath;
    }

    char *fullpath = new char[strlen(parentFilename) + strlen(path) + 1];
    strcpy(fullpath, parentFilename);
    char *lastSlash = strrchr(fullpath, '/');
    if (lastSlash)
    {
        *lastSlash = '\0';
    } else {
        *fullpath = '\0';
    }
    strcat(fullpath, "/");
    strcat(fullpath, path);
    return fullpath;
}

#endif
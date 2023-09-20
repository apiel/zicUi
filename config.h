#ifndef _UI_CONFIG_H_
#define _UI_CONFIG_H_

#include "fs.h"
#include "viewMain.h"
#include "plugins.h"

#define CONFIG_FILE "./config.ui.cfg"

char *trimChar(char *str, char c = '\n')
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] == c)
        {
            str[i] = '\0';
            break;
        }
    }
    return str;
}

void assignKeyValue(char *key, char *value)
{
    if (strcmp(key, "PLUGIN_INTERFACE") == 0)
    {
        loadPluginInterface(value);
    }
    else if (lastPluginInstance && lastPluginInstance->config(key, value))
    {
        return;
    }
    else
    {
        ViewMain::get().config(key, value);
    }
}

void parseConfigLine(char *line)
{
    // ignore comments and empty lines
    if (line[0] == '#' || line[0] == '\n')
    {
        return;
    }
    // split by '='
    char *key = strtok(line, "=");
    char *value = strtok(NULL, "=");
    if (key == NULL || value == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid config line: %s\n", line);
        return;
    }
    assignKeyValue(key, trimChar(value));
}

bool loadConfig()
{
    FILE *file = fopen(CONFIG_FILE, "r");
    if (file == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load config file: %s\n", CONFIG_FILE);
        return false;
    }

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        parseConfigLine(line);
    }
    fclose(file);

    return true;
}

#endif
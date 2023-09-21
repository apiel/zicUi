#ifndef _UI_COMPONENT_INTERFACE_H_
#define _UI_COMPONENT_INTERFACE_H_

#include "drawInterface.h"
#include "valueInterface.h"

#include "../motion.h"

class ComponentInterface
{
protected:
    DrawInterface &draw;
    Styles &styles;
    virtual void render() = 0;

    AudioPlugin &(*getPlugin)(const char *name);
    ValueInterface *(*hostValue)(ValueProps props);

public:
    Point position;
    const Size size;
    bool needRendering = true;
    int8_t group = -1;

    ComponentInterface(Point position, Size size, DrawInterface &draw,
                       AudioPlugin &(*getPlugin)(const char *name), ValueInterface *(*hostValue)(ValueProps props))
        : draw(draw), styles(draw.styles), getPlugin(getPlugin), hostValue(hostValue), position(position), size(size)
    {
    }

    virtual void triggerRenderer() = 0;
    virtual void renderNext() = 0;
    virtual void onMotion(Motion &motion) = 0;
    virtual void handleMotion(Motion &motion) = 0;
    virtual void onMotionRelease(Motion &motion) = 0;
    virtual void handleMotionRelease(Motion &motion) = 0;
    virtual void onEncoder(int id, int8_t direction) = 0;
    virtual void config(char *key, char *value) = 0;
    virtual void baseConfig(char *key, char *value) = 0;
    virtual void onGroupChanged(int8_t index) = 0;
};

#endif

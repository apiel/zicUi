#ifndef _UI_COMPONENT_GRANULAR_H_
#define _UI_COMPONENT_GRANULAR_H_

#include "../def.h"
#include "../component.h"
#include "../draw.h"
#include "../host.h"

class ComponentGranular : public Component
{
protected:
    AudioPlugin &plugin;

    bool noteIsOn = false;

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.pad.background);
    }

public:
    const int margin = styles.margin;

    ComponentGranular(Point position, Size size)
        : Component(position, size), plugin(getPlugin("Granular"))
    {
    }

    bool onMotion(Motion &motion)
    {
        if (!Component::onMotion(motion))
        {
            return false;
        }

        if (!noteIsOn)
        {
            plugin.noteOn(48, 127);
            noteIsOn = true;
        }
        return true;
    }

    void onMotionRelease(Motion &motion)
    {
        if (noteIsOn)
        {
            plugin.noteOff(48, 0);
            noteIsOn = false;
        }
    }
};

#endif

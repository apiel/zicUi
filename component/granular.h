#ifndef _UI_COMPONENT_GRANULAR_H_
#define _UI_COMPONENT_GRANULAR_H_

#include "../def.h"
#include "../view.h"
#include "../draw.h"
#include "../host.h"

class ComponentGranular : public View
{
protected:
    AudioPlugin &plugin;

    bool noteIsOn = false;

public:


    const int margin = styles.margin;

    ComponentGranular(Point position, Size size)
        : View(position, size), plugin(getPlugin("Granular"))
    {
    }

    void render()
    {
        drawFilledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.pad.background);

        drawNext();
    }

    void onMotion(Motion &motion)
    {
        if (!noteIsOn)
        {
            plugin.noteOn(48, 127);
            noteIsOn = true;
        }
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

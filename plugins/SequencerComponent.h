#ifndef _UI_COMPONENT_SEQUENCER_H_
#define _UI_COMPONENT_SEQUENCER_H_

#include "../plugins/component.h"
#include "../../zicHost/plugins/stepInterface.h"

#include <string>

class SequencerComponent : public Component
{
protected:
    AudioPlugin &plugin;
    ValueInterface *selectedStep = getPlugin("Sequencer").getValue("SELECTED_STEP");
    ValueInterface *pattern = getPlugin("Sequencer").getValue("PATTERN");

    Point stepPosition;
    Size stepSize;
    int stepMarginTop = 15;
    int infoMarginRight = 10;

    Step *steps;
    uint8_t *stepCounter;
    uint8_t previousStepCounter = 0;

    void renderStep(uint8_t index)
    {
        Color color = colors.stepBackground;
                if (steps[index].enabled)
        {
            color = colors.stepEnabled;
        }
        else if (index % 4 != 0)
        {
            color.a = 150;
        }

        int x = stepPosition.x + (index * (stepSize.w + margin));
        draw.filledRect({x, stepPosition.y}, stepSize, color);

        if (index == *stepCounter)
        {
            draw.filledRect({x, stepPosition.y - 3}, {stepSize.w, 2}, colors.activePosition);
        }
    }

    void render()
    {
        draw.filledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.background);

        for (int i = 0; i < selectedStep->props().stepCount; i++)
        {
            renderStep(i);
        }
    }

    struct Colors
    {
        Color background;
        Color stepBackground;
        Color stepEnabled;
        Color activePosition;
    } colors;

    const int margin;

public:
    SequencerComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground,
                  styles.colors.foreground2,
                  styles.colors.text,
                  styles.colors.on}),
          margin(styles.margin),
          plugin(getPlugin("Sequencer"))
    {
        stepSize = {
            (((props.size.w) / selectedStep->props().stepCount) - margin),
            props.size.h - (stepMarginTop + margin * 2)};

        stepPosition = {
            position.x + margin + (int)((size.w - ((stepSize.w + margin) * selectedStep->props().stepCount)) * 0.5),
            position.y + stepMarginTop};

        if (stepPosition.x > infoMarginRight)
        {
            infoMarginRight = stepPosition.x;
        }

        steps = (Step *)plugin.data(0);
        stepCounter = (uint8_t *)plugin.data(1);
    }

    void triggerRenderer() override
    {
        if (previousStepCounter != *stepCounter)
        {
            needRendering = true;
            // TODO could only render necessary part
            previousStepCounter = *stepCounter;
        }
        Component::triggerRenderer();
    }
};

#endif

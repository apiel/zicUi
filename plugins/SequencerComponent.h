#ifndef _UI_COMPONENT_SEQUENCER_H_
#define _UI_COMPONENT_SEQUENCER_H_

#include "../plugins/component.h"
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

    void renderStep(uint8_t index)
    {
        Color color = colors.stepBackground;
        if (index % 4 != 0)
        {
            color.a = 150;
        }
        draw.filledRect({stepPosition.x + (index * (stepSize.w + margin)), stepPosition.y}, stepSize, color);
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
    } colors;

    const int margin;

public:
    SequencerComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground,
                  styles.colors.foreground2}),
          margin(styles.margin),
          plugin(getPlugin("Sequencer"))
    {
        stepSize = {
            (((props.size.w) / selectedStep->props().stepCount) - margin),
            props.size.h - (stepMarginTop + margin * 2)};

        stepPosition = {
            position.x + margin + (int)((size.w - ((stepSize.w + margin) * selectedStep->props().stepCount)) * 0.5),
            position.y + stepMarginTop};
    }
};

#endif

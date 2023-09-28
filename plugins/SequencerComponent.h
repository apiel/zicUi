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
    ValueInterface *stepEnabled = getPlugin("Sequencer").getValue("STEP_ENABLED");
    ValueInterface *stepLength = getPlugin("Sequencer").getValue("STEP_LENGTH");
    ValueInterface *stepVelocity = getPlugin("Sequencer").getValue("STEP_VELOCITY");
    ValueInterface *stepNote = getPlugin("Sequencer").getValue("STEP_NOTE");
    ValueInterface *pattern = getPlugin("Sequencer").getValue("PATTERN");

    Point stepPosition;
    Size stepSize;
    int stepMarginTop = 18;

    Step *steps;
    uint8_t *stepCounter;
    uint8_t previousStepCounter = 0;

    int debounceSelectedStep = 0;
    float previousSelectedStep = 0.0;

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

        int sel = selectedStep->get() * selectedStep->props().stepCount;
        if (index == sel)
        {
            draw.filledRect({x, stepPosition.y - 3}, {stepSize.w, 2}, colors.stepBackground);
        }

        if (index == *stepCounter)
        {
            draw.filledRect({x, stepPosition.y - 3}, {stepSize.w, 2}, colors.activePosition);
        }
        else if (steps[index].counter)
        {
            int w = stepSize.w * (steps[index].counter / (float)steps[index].len);
            draw.filledRect({x + stepSize.w - w, stepPosition.y - 3}, {w, 2}, colors.activePosition);
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
            // printf("step %d, note %d, enabled %d, len %d\n", i, steps[i].note, steps[i].enabled, steps[i].len);
        }

        char info[16];
        int stepIndex = selectedStep->get() * selectedStep->props().stepCount + 1;
        int len = stepLength->get() * selectedStep->props().stepCount;
        snprintf(info, 16, "Step: %.2d/%d", stepIndex, selectedStep->props().stepCount);
        draw.text({stepPosition.x, position.y}, info, colors.textInfo, 9);

        draw.text({stepPosition.x + 50, position.y}, stepEnabled->string(), colors.textInfo, 9);
        draw.text({stepPosition.x + 70, position.y}, stepNote->string(), colors.textInfo, 9);

        snprintf(info, 16, "len: %2d", len);
        draw.text({stepPosition.x + 90, position.y}, info, colors.textInfo, 9);

        snprintf(info, 16, "velocity: %3d%%", (int)(stepVelocity->get() * 100));
        draw.text({stepPosition.x + 120, position.y}, info, colors.textInfo, 9);
    }

    struct Colors
    {
        Color background;
        Color stepBackground;
        Color stepEnabled;
        Color activePosition;
        Color textInfo;
    } colors;

    const int margin;

public:
    SequencerComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground,
                  styles.colors.foreground2,
                  styles.colors.text,
                  styles.colors.on,
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

        steps = (Step *)plugin.data(0);
        stepCounter = (uint8_t *)plugin.data(1);
    }

    void triggerRenderer() override
    {
        if (previousStepCounter != *stepCounter || previousSelectedStep != selectedStep->get())
        {
            needRendering = true;
            // TODO could only render necessary part
            previousStepCounter = *stepCounter;
            previousSelectedStep = selectedStep->get();
        }
        Component::triggerRenderer();
    }

    void onMotion(MotionInterface &motion)
    {

        int index = (motion.position.x - stepPosition.x) / (stepSize.w + margin);
        if (debounceSelectedStep != index)
        {
            debounceSelectedStep = index;
            selectedStep->set(index / (float)selectedStep->props().stepCount);
        }
    }
};

#endif

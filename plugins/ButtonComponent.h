#ifndef _UI_COMPONENT_BUTTON_H_
#define _UI_COMPONENT_BUTTON_H_

#include "../plugins/component.h"
#include <string>

class ButtonComponent : public Component
{
protected:
    const char *label = NULL;
    ValueInterface *value = NULL;

int fontSize = 12;
    Point labelPosition;

    void render()
    {
        draw.filledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            colors.background);

        draw.textCentered(labelPosition, label, colors.title, fontSize);
    }

    void set(const char *pluginName, const char *key)
    {
        value = val(getPlugin(pluginName).getValue(key));
        if (value != NULL && label == NULL)
        {
            label = value->label();
        }
    }

    struct Colors
    {
        Color background;
        Color title;
    } colors;

    const int margin;

public:
    ButtonComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground2, styles.colors.text}),
          margin(styles.margin),
          labelPosition({ (int)(position.x + size.w * 0.5f), (int)(position.y + size.h * 0.5f - (fontSize * 0.5f)) })
    {
    }

    bool config(char *key, char *value)
    {
        if (strcmp(key, "VALUE") == 0)
        {
            printf("value: %s\n", value);
            char *pluginName = strtok(value, " ");
            char *keyValue = strtok(NULL, " ");
            set(pluginName, keyValue);
            return true;
        }

        return false;
    }

    // void onMotion(MotionInterface &motion)
    // {
    //     // could draw button pressed
    // }

    void onMotionRelease(MotionInterface &motion)
    {
        Component::onMotionRelease(motion);
        printf("onMotionRelease btn\n");
    }
};

#endif

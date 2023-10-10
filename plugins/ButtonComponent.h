#ifndef _UI_COMPONENT_BUTTON_H_
#define _UI_COMPONENT_BUTTON_H_

#include "../plugins/component.h"
#include <string>

class ButtonComponent : public Component
{
protected:
    char *label = NULL;
    ValueInterface *value = NULL;

    int fontSize = 12;
    Point labelPosition;

    bool isPressed = false;
    float pressedValue = 0.0f;

    void render()
    {
        draw.filledRect(
            {position.x + margin, position.y + margin},
            {size.w - 2 * margin, size.h - 2 * margin},
            isPressed ? colors.pressedBackground : colors.background);

        draw.textCentered(labelPosition, label, colors.title, fontSize);
    }

    void set(const char *pluginName, const char *key)
    {
        value = val(getPlugin(pluginName).getValue(key));
        if (value != NULL && label == NULL)
        {
            label = (char *)value->label();
        }
    }

    struct Colors
    {
        Color background;
        Color pressedBackground;
        Color title;
    } colors;

    const int margin;

public:
    ButtonComponent(ComponentInterface::Props &props)
        : Component(props),
          colors({styles.colors.foreground2, styles.colors.foreground3, styles.colors.text}),
          margin(styles.margin),
          labelPosition({(int)(position.x + size.w * 0.5f), (int)(position.y + size.h * 0.5f - (fontSize * 0.5f))})
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

        if (strcmp(key, "LABEL") == 0)
        {
            // maybe we check that the current point is not pointing to value->label()
            // and free the label, but why should we assign multiple time a label...
            label = new char[strlen(value) + 1];
            strcpy(label, value);
            return true;
        }

        if (strcmp(key, "PRESSED_VALUE") == 0)
        {
            pressedValue = atof(value);
            return true;
        }

        // TODO PRESSED_INT_VALUE 

        return false;
    }

    void onMotion(MotionInterface &motion)
    {
        isPressed = true;
        renderNext();
    }

    void onMotionRelease(MotionInterface &motion)
    {
        if (motion.originIn({position, size}))
        {
            isPressed = false;
            renderNext();
            value->set(pressedValue);
        }
    }
};

#endif

#ifndef _UI_DRAW_INTERFACE_H_
#define _UI_DRAW_INTERFACE_H_

// to be removed...
#include "../def.h"

struct DrawTextOptions
{
    const char *fontPath = APP_FONT;
    int maxWidth = SCREEN_W;
};

class DrawInterface
{
public:
    virtual void render() = 0;
    virtual void triggerRender() = 0;
    virtual void next() = 0;

    virtual int textCentered(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, DrawTextOptions options = {}) = 0;
    virtual int text(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, DrawTextOptions options = {}) = 0;
    virtual int textRight(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, DrawTextOptions options = {}) = 0;
    virtual void clear(SDL_Color color = colors.core.background) = 0;
    virtual void filledRect(Point position, Size size, SDL_Color color = colors.core.foreground) = 0;
    virtual void rect(Point position, Size size, SDL_Color color = colors.core.foreground) = 0;
    virtual void line(Point start, Point end, SDL_Color color = colors.core.white) = 0;
};

#endif
#ifndef _UI_DRAW_INTERFACE_H_
#define _UI_DRAW_INTERFACE_H_

// to be removed...
#include "../def.h"

struct DrawTextOptions
{
    const char *fontPath = font.regular;
    int maxWidth = styles.screen.w;
};

class DrawInterface
{
public:
    virtual void render() = 0;
    virtual void triggerRender() = 0;
    virtual void next() = 0;

    virtual int textCentered(Point position, const char *text, SDL_Color color, uint32_t size = font.defaultSize, DrawTextOptions options = {}) = 0;
    virtual int text(Point position, const char *text, SDL_Color color, uint32_t size = font.defaultSize, DrawTextOptions options = {}) = 0;
    virtual int textRight(Point position, const char *text, SDL_Color color, uint32_t size = font.defaultSize, DrawTextOptions options = {}) = 0;
    virtual void clear(SDL_Color color = styles.colors.background) = 0;
    virtual void filledRect(Point position, Size size, SDL_Color color) = 0;
    virtual void rect(Point position, Size size, SDL_Color color) = 0;
    virtual void line(Point start, Point end, SDL_Color color) = 0;
};

#endif
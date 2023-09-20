#ifndef _UI_DEF_H_
#define _UI_DEF_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "helpers/range.h"
#include "helpers/rangeFloat.h"

#define ENCODER_COUNT 8

#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888

struct Size
{
    int w;
    int h;
};

struct Point
{
    int x;
    int y;
};

struct Rect
{
    Point position;
    Size size;
};

struct Styles
{
    Size screen = {800, 480};
    const int margin = 1;
    struct Font
    {
        uint32_t defaultSize = 16;
        const char *regular = "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Regular.ttf";
        const char *bold = "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Bold.ttf";
        const char *italic = "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Italic.ttf";
    } font;
    struct Colors
    {
        SDL_Color background = {0x21, 0x25, 0x2b, 255};  // #21252b
        SDL_Color foreground = {0x2b, 0x2c, 0x2e, 255};  // #2b2c2e
        SDL_Color foreground2 = {0x47, 0x56, 0x6e, 255}; // #47566e
        SDL_Color foreground3 = {0x6f, 0x86, 0xab, 255}; // #6f86ab
        SDL_Color text = {0xad, 0xcd, 0xff, 255};        // #adcdff
        SDL_Color textDark = {0x7c, 0x98, 0xc4, 255};    // #7c98c4
        SDL_Color overlay = {0xd1, 0xe3, 0xff, 0x1e};    // #d1e3ff1e
    } colors;
} styles;

#endif

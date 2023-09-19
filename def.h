#ifndef _UI_DEF_H_
#define _UI_DEF_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "helpers/range.h"
#include "helpers/rangeFloat.h"

#ifndef DEFAULT_FONT_SIZE
#define DEFAULT_FONT_SIZE 16
#endif

#ifndef APP_FONT
// #define APP_FONT "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
// #define APP_FONT "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
// #define APP_FONT "/usr/share/fonts/truetype/freefont/FreeMono.ttf"
// #define APP_FONT "/usr/share/fonts/truetype/roboto/unhinted/RobotoCondensed-Regular.ttf"
#define APP_FONT "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Regular.ttf"
#endif

#ifndef APP_FONT_BOLD
// #define APP_FONT_BOLD "/usr/share/fonts/truetype/liberation2/LiberationSans-Bold.ttf"
// #define APP_FONT_BOLD "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf"
// #define APP_FONT_BOLD "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf"
// #define APP_FONT_BOLD "/usr/share/fonts/truetype/roboto/unhinted/RobotoCondensed-Bold.ttf"
#define APP_FONT_BOLD "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Bold.ttf"
#endif

#ifndef APP_FONT_ITALIC
// #define APP_FONT_ITALIC "/usr/share/fonts/truetype/liberation2/LiberationSans-Italic.ttf"
// #define APP_FONT_ITALIC "/usr/share/fonts/truetype/freefont/FreeSansOblique.ttf"
// #define APP_FONT_ITALIC "/usr/share/fonts/truetype/freefont/FreeMonoOblique.ttf"
// #define APP_FONT_ITALIC "/usr/share/fonts/truetype/roboto/unhinted/RobotoCondensed-Italic.ttf"
#define APP_FONT_ITALIC "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Italic.ttf"
#endif

#define SCREEN_W 800
#define SCREEN_H 480
#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888

#define ENCODER_COUNT 8

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

struct CoreColors
{
    SDL_Color background = {0x21, 0x25, 0x2b, 255};  // #21252b
    SDL_Color foreground = {0x2b, 0x2c, 0x2e, 255};  // #2b2c2e
    SDL_Color foreground2 = {0x47, 0x56, 0x6e, 255}; // #47566e
    SDL_Color foreground3 = {0x6f, 0x86, 0xab, 255}; // #6f86ab
    SDL_Color text = {0xad, 0xcd, 0xff, 255};        // #adcdff
    SDL_Color textDark = {0x7c, 0x98, 0xc4, 255};    // #7c98c4
    SDL_Color overlay = {0xd1, 0xe3, 0xff, 0x1e};    // #d1e3ff1e
} coreColors;

struct Styles
{
    const int margin = 1;
} styles;

#endif

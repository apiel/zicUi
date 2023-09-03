#ifndef _UI_DEF_H_
#define _UI_DEF_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "helpers/range.h"
#include "helpers/rangeFloat.h"

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

#ifndef APP_DEFAULT_FONT_SIZE
#define APP_DEFAULT_FONT_SIZE 16
#endif

#define SCREEN_W 800
#define SCREEN_H 480

#define ENCODER_COUNT 4

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
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color background = {0x21, 0x25, 0x2b, 255}; // #21252b
    SDL_Color foreground = {0x2b, 0x2c, 0x2e, 255}; // #2b2c2e
    SDL_Color foreground2 = { 0x38, 0x3a, 0x3d, 255 }; // #384a3d
    SDL_Color font = this->white;
        SDL_Color value = { 0xad, 0xcd, 0xff, 255 }; // #adcdff
} coreColors;

struct Colors
{
    CoreColors core = coreColors;
    struct Encoder {
        // SDL_Color background = coreColors.foreground2;
        SDL_Color background = { 0x47, 0x56, 0x6e, 255 }; // #47566e
        SDL_Color title = { 0x7c, 0x98, 0xc4, 255 }; // #7c98c4
        SDL_Color value = coreColors.value;
    } encoder;
    struct Pad {
        SDL_Color background = coreColors.foreground;
        SDL_Color value = coreColors.value;
    } pad;
} colors;

struct Dimensions
{
    Size encoder = {SCREEN_W / ENCODER_COUNT, 50};
    int pad_h = SCREEN_H - this->encoder.h;
    int pad_y = this->encoder.h;
} dimensions;

#endif

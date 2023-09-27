#ifndef _STYLES_H_
#define _STYLES_H_

#include "plugins/drawInterface.h"

Styles styles = {
    .screen = {800, 480},
    .margin = 1,
    .font = {
        "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Regular.ttf",
        "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Bold.ttf",
        "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Italic.ttf"},
    .colors = {
        .background = {0x21, 0x25, 0x2b, 255},  // #21252b
        .foreground = {0x2b, 0x2c, 0x2e, 255},  // #2b2c2e
        .foreground2 = {0x47, 0x56, 0x6e, 255}, // #47566e
        .foreground3 = {0x6f, 0x86, 0xab, 255}, // #6f86ab
        .text = {0xad, 0xcd, 0xff, 255},        // #adcdff
        .textDark = {0x7c, 0x98, 0xc4, 255},    // #7c98c4
        .overlay = {0xd1, 0xe3, 0xff, 0x1e},    // #d1e3ff1e
        .on = {0x00, 0xb3, 0x00, 255},          // #00b300
    },
};

#endif

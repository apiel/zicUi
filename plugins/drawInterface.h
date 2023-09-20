#ifndef _UI_DRAW_INTERFACE_H_
#define _UI_DRAW_INTERFACE_H_

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

typedef struct Color
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Color;

// FIXME
// TODO pass this as props
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
        Color background = {0x21, 0x25, 0x2b, 255};  // #21252b
        Color foreground = {0x2b, 0x2c, 0x2e, 255};  // #2b2c2e
        Color foreground2 = {0x47, 0x56, 0x6e, 255}; // #47566e
        Color foreground3 = {0x6f, 0x86, 0xab, 255}; // #6f86ab
        Color text = {0xad, 0xcd, 0xff, 255};        // #adcdff
        Color textDark = {0x7c, 0x98, 0xc4, 255};    // #7c98c4
        Color overlay = {0xd1, 0xe3, 0xff, 0x1e};    // #d1e3ff1e
    } colors;
} styles;

struct DrawTextOptions
{
    const char *fontPath = styles.font.regular;
    int maxWidth = styles.screen.w;
};

class DrawInterface
{
public:
    virtual void render() = 0;
    virtual void triggerRender() = 0;
    virtual void next() = 0;

    virtual int textCentered(Point position, const char *text, Color color, uint32_t size = styles.font.defaultSize, DrawTextOptions options = {}) = 0;
    virtual int text(Point position, const char *text, Color color, uint32_t size = styles.font.defaultSize, DrawTextOptions options = {}) = 0;
    virtual int textRight(Point position, const char *text, Color color, uint32_t size = styles.font.defaultSize, DrawTextOptions options = {}) = 0;
    virtual void clear(Color color = styles.colors.background) = 0;
    virtual void filledRect(Point position, Size size, Color color) = 0;
    virtual void rect(Point position, Size size, Color color) = 0;
    virtual void line(Point start, Point end, Color color) = 0;
};

#endif
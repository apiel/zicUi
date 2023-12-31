#ifndef _UI_DRAW_H_
#define _UI_DRAW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "state.h"
#include "plugins/drawInterface.h"

#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888

class Draw : public DrawInterface
{
protected:
    bool needToRender = false;

    SDL_Surface *getTextSurface(const char *text, Color color, uint32_t size, const char *fontPath)
    {
        TTF_Font *font = TTF_OpenFont(fontPath, size);
        if (font == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open font\n");
            return 0;
        }
        SDL_Surface *surface = TTF_RenderText_Solid(font, text, {color.r, color.g, color.b, color.a});
        if (surface == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render text\n");
            return 0;
        }
        TTF_CloseFont(font);

        return surface;
    }

    void textToRenderer(Point position, SDL_Surface *surface, int maxWidth)
    {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture\n");
            return;
        }
        // SDL_Rect rect = {position.x, position.y, surface->w, surface->h};
        // SDL_RenderCopy(renderer, texture, NULL, &rect);

        int w = maxWidth < surface->w ? maxWidth : surface->w;
        SDL_Rect rect1 = {0, 0, w, surface->h};
        SDL_Rect rect2 = {position.x, position.y, w, surface->h};
        SDL_RenderCopy(renderer, texture, &rect1, &rect2);
        SDL_DestroyTexture(texture);
    }

    float min(float a, float b)
    {
        return a < b ? a : b;
    }

    Color *getStyleColor(char *color)
    {
        if (strcmp(color, "background") == 0)
        {
            return &styles.colors.background;
        }

        if (strcmp(color, "overlay") == 0)
        {
            return &styles.colors.overlay;
        }

        if (strcmp(color, "on") == 0)
        {
            return &styles.colors.on;
        }

        if (strcmp(color, "white") == 0)
        {
            return &styles.colors.white;
        }

        if (strcmp(color, "blue") == 0)
        {
            return &styles.colors.blue;
        }

        if (strcmp(color, "red") == 0)
        {
            return &styles.colors.red;
        }

        return NULL;
    }

    Color hex2rgb(char *hex)
    {

        hex++;
        unsigned int color = strtol(hex, NULL, 16);
        return Color({
            .r = (uint8_t)((color & 0x00FF0000) >> 16),
            .g = (uint8_t)((color & 0x0000FF00) >> 8),
            .b = (uint8_t)((color & 0x000000FF)),
            .a = 255,
        });
    }

public:
    Draw(Styles styles)
        : DrawInterface(styles)
    {
    }

    void render()
    {
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_SetRenderTarget(renderer, texture);
    }

    void triggerRender()
    {
        if (needToRender)
        {
            render();
            needToRender = false;
        }
    }

    void next()
    {
        needToRender = true;
    }

    int textCentered(Point position, const char *text, Color color, uint32_t size, DrawTextOptions options = {})
    {
        options = getDefaultTextOptions(options);
        SDL_Surface *surface = getTextSurface(text, color, size, options.fontPath);
        int x = position.x - (surface->w * 0.5);
        textToRenderer({x, position.y}, surface, options.maxWidth);
        int xEnd = x + surface->w;
        SDL_FreeSurface(surface);

        return xEnd;
    }

    int text(Point position, const char *text, Color color, uint32_t size, DrawTextOptions options = {})
    {
        options = getDefaultTextOptions(options);
        SDL_Surface *surface = getTextSurface(text, color, size, options.fontPath);
        textToRenderer(position, surface, options.maxWidth);
        int xEnd = position.x + surface->w;
        SDL_FreeSurface(surface);

        return xEnd;
    }

    int textRight(Point position, const char *text, Color color, uint32_t size, DrawTextOptions options = {})
    {
        options = getDefaultTextOptions(options);
        SDL_Surface *surface = getTextSurface(text, color, size, options.fontPath);
        int x = position.x - surface->w;
        textToRenderer({x, position.y}, surface, options.maxWidth);
        SDL_FreeSurface(surface);

        return x;
    }

    void clear()
    {
        Color color = styles.colors.background;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(renderer);
    }

    void filledRect(Point position, Size size, Color color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = {position.x, position.y, size.w, size.h};
        SDL_RenderFillRect(renderer, &rect);
    }

    void rect(Point position, Size size, Color color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = {position.x, position.y, size.w, size.h};
        SDL_RenderDrawRect(renderer, &rect);
    }

    void line(Point start, Point end, Color color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    }

    void *setTextureRenderer(Size size)
    {
        SDL_Texture *texture = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, size.w, size.h);
        SDL_SetRenderTarget(renderer, texture);
        return texture;
    }

    void setMainRenderer()
    {
        SDL_SetRenderTarget(renderer, texture);
    }

    void destroyTexture(void *texture)
    {
        SDL_DestroyTexture((SDL_Texture *)texture);
    }

    void applyTexture(void *texture, Rect dest)
    {
        SDL_Rect rect = {dest.position.x, dest.position.y, dest.size.w, dest.size.h};
        SDL_RenderCopy(renderer, (SDL_Texture *)texture, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    Color darken(Color color, float amount)
    {
        return {
            (uint8_t)(color.r * (1.0f - amount)),
            (uint8_t)(color.g * (1.0f - amount)),
            (uint8_t)(color.b * (1.0f - amount)),
            color.a};
    }

    Color lighten(Color color, float amount)
    {
        return {
            (uint8_t)min(color.r * (1.0f + amount), 255.0f),
            (uint8_t)min(color.g * (1.0f + amount), 255.0f),
            (uint8_t)min(color.b * (1.0f + amount), 255.0f),
            color.a};
    }

    Color getColor(char *color)
    {
        // if first char is # then call hex2rgb
        if (color[0] == '#')
        {
            return hex2rgb(color);
        }

        Color *styleColor = getStyleColor(color);
        if (styleColor != NULL)
        {
            return *styleColor;
        }

        return styles.colors.white;
    }

    void setColor(char *name, char *color)
    {
        Color *styleColor = getStyleColor(name);
        if (styleColor != NULL)
        {
            Color newColor = hex2rgb(color);
            //    *styleColor = newColor; // Dont do like this, to keep transparency
            styleColor->r = newColor.r;
            styleColor->g = newColor.g;
            styleColor->b = newColor.b;
        }
    }
};

#endif
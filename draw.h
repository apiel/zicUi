#ifndef _UI_DRAW_H_
#define _UI_DRAW_H_

#include "def.h"
#include "state.h"
// #include "drawButton.h"

void draw()
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, texture);
}

void drawNext()
{
    needToDraw = true;
}

SDL_Surface *getTextSurface(const char *text, SDL_Color color, uint32_t size, const char *fontPath)
{
    TTF_Font *font = TTF_OpenFont(fontPath, size);
    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open font\n");
        return 0;
    }
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
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

struct DrawTextOptions
{
    const char *fontPath = APP_FONT;
    int maxWidth = SCREEN_W;
};

int drawTextCentered(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, DrawTextOptions options = {})
{
    SDL_Surface *surface = getTextSurface(text, color, size, options.fontPath);
    int x = position.x - (surface->w * 0.5);
    textToRenderer({x, position.y}, surface, options.maxWidth);
    int xEnd = x + surface->w;
    SDL_FreeSurface(surface);

    return xEnd;
}

int drawText(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, DrawTextOptions options = {})
{
    SDL_Surface *surface = getTextSurface(text, color, size, options.fontPath);
    textToRenderer(position, surface, options.maxWidth);
    int xEnd = position.x + surface->w;
    SDL_FreeSurface(surface);

    return xEnd;
}

int drawTextRight(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, DrawTextOptions options = {})
{
    SDL_Surface *surface = getTextSurface(text, color, size, options.fontPath);
    int x = position.x - surface->w;
    textToRenderer({x, position.y}, surface, options.maxWidth);
    SDL_FreeSurface(surface);

    return x;
}

void drawClear(SDL_Color color = colors.core.background)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void drawFilledRect(Point position, Size size, SDL_Color color = colors.core.foreground)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {position.x, position.y, size.w, size.h};
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(Point position, Size size, SDL_Color color = colors.core.foreground)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {position.x, position.y, size.w, size.h};
    SDL_RenderDrawRect(renderer, &rect);
}

void drawLine(Point start, Point end, SDL_Color color = colors.core.white)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

#endif
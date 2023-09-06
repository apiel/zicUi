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

void textToRenderer(Point position, SDL_Surface *surface)
{
    SDL_Rect rect = {position.x, position.y, surface->w, surface->h};

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture\n");
        return;
    }
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void drawTextCentered(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, const char *fontPath = APP_FONT)
{
    SDL_Surface *surface = getTextSurface(text, color, size, fontPath);
    int x = position.x - (surface->w * 0.5);
    textToRenderer({x, position.y}, surface);
    SDL_FreeSurface(surface);
}

int drawText(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, const char *fontPath = APP_FONT)
{
    SDL_Surface *surface = getTextSurface(text, color, size, fontPath);
    textToRenderer(position, surface);
    int w = surface->w;
    SDL_FreeSurface(surface);

    return w;
}

int drawTextRight(Point position, const char *text, SDL_Color color = colors.core.font, uint32_t size = APP_DEFAULT_FONT_SIZE, const char *fontPath = APP_FONT)
{
    SDL_Surface *surface = getTextSurface(text, color, size, fontPath);
    textToRenderer({position.x - surface->w, position.y}, surface);
    int w = surface->w;
    SDL_FreeSurface(surface);

    return w;
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
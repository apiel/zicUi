#include "def.h"
#include "state.h"
#include "event.h"
#include "viewMain.h"
#include "host.h"
#include "config.h"

int main()
{
    if (!loadConfig())
    {
        // FIXME here we should close audio stuff
        // however how come that it is loaded even before being called
        return 1;
    }

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL video driver: %s", SDL_GetCurrentVideoDriver());

#ifdef IS_RPI
    SDL_ShowCursor(SDL_DISABLE);
#endif

    window = SDL_CreateWindow(
        "Zic",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN
#ifdef IS_RPI
            | SDL_WINDOW_FULLSCREEN
#endif
    );

    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s", SDL_GetError());
        return 1;
    }

    TTF_Init();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    texture = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
    SDL_SetRenderTarget(renderer, texture);

    if (!loadHost())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load host");
    }

    ViewMain &viewMain = ViewMain::get();
    viewMain.render();
    draw();

    EventHandler &event = EventHandler::get();
    unsigned long lastUpdate = SDL_GetTicks();
    while (event.handle())
    {
        unsigned long now = SDL_GetTicks();
        if (now - lastUpdate > 50)
        {
            lastUpdate = now;
            viewMain.renderComponents();
            if (needToDraw)
            {
                draw();
                needToDraw = false;
            }
        }
        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

//
// Created by lin-k on 01.10.2016.
//

#include "../include/Emulator.h"

#ifndef __EMSCRIPTEN__
static const uint32_t idealFrameTime = ((1. / 60) * 1000);
static Uint32 ticks;
#endif
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Texture *texture;
static pthread_t thread;
static Uint32 vsyncEvent;
static bool isRunning;

Emulator::Emulator(std::string _filename) : filename(_filename) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "NESEmulator");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                SCREEN_HEIGHT);
    SDL_RenderClear(renderer);
    vsyncEvent = SDL_RegisterEvents(1);
#ifndef __EMSCRIPTEN__
    ticks = SDL_GetTicks();
#endif
}

void drawerFunc(int x, int y, uint32_t color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *) p = color;
}

void vertSyncHandler(void) {
#ifndef __EMSCRIPTEN__
    Uint32 frametime = SDL_GetTicks() - ticks;
    if (frametime < idealFrameTime) {
        SDL_Delay(idealFrameTime - frametime);
    }
#endif
    if (vsyncEvent != ((Uint32) -1)) {
        SDL_Event event;
        SDL_memset(&event, 0, sizeof(event));
        event.type = vsyncEvent;
        SDL_PushEvent(&event);
    }
#ifndef __EMSCRIPTEN__
    ticks = SDL_GetTicks();
#endif
}

void *mainLoop(void *arg) {
    Console &c = Console::Instance();
    while (isRunning)
        c.step();
    return arg;
}

void mainLoopStep() {
    SDL_Event event;
    bool states[8] = {false, false, false, false, false, false, false, false};
    Console &c = Console::Instance();
    if (SDL_PollEvent(&event)) {
        if (event.type == vsyncEvent) {
            SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.repeat == 0) {
                if (!states[Buttons::A] && event.key.keysym.sym == SDLK_z) {
                    states[Buttons::A] = true;
                }
                if (!states[Buttons::B] && event.key.keysym.sym == SDLK_x) {
                    states[Buttons::B] = true;
                }
                if (!states[Buttons::SELECT] && event.key.keysym.sym == SDLK_SPACE) {
                    states[Buttons::SELECT] = true;
                }
                if (!states[Buttons::START] && event.key.keysym.sym == SDLK_RETURN) {
                    states[Buttons::START] = true;
                }
                if (!states[Buttons::UP] && event.key.keysym.sym == SDLK_UP) {
                    states[Buttons::UP] = true;
                }
                if (!states[Buttons::DOWN] && event.key.keysym.sym == SDLK_DOWN) {
                    states[Buttons::DOWN] = true;
                }
                if (!states[Buttons::LEFT] && event.key.keysym.sym == SDLK_LEFT) {
                    states[Buttons::LEFT] = true;
                }
                if (!states[Buttons::RIGHT] && event.key.keysym.sym == SDLK_RIGHT) {
                    states[Buttons::RIGHT] = true;
                }
                c.getController()->setButtons(states);
            }
        } else if (event.type == SDL_KEYUP) {
            if (states[Buttons::A] && event.key.keysym.sym == SDLK_z) {
                states[Buttons::A] = false;
            }
            if (states[Buttons::B] && event.key.keysym.sym == SDLK_x) {
                states[Buttons::B] = false;
            }
            if (states[Buttons::SELECT] && event.key.keysym.sym == SDLK_SPACE) {
                states[Buttons::SELECT] = false;
            }
            if (states[Buttons::START] && event.key.keysym.sym == SDLK_RETURN) {
                states[Buttons::START] = false;
            }
            if (states[Buttons::UP] && event.key.keysym.sym == SDLK_UP) {
                states[Buttons::UP] = false;
            }
            if (states[Buttons::DOWN] && event.key.keysym.sym == SDLK_DOWN) {
                states[Buttons::DOWN] = false;
            }
            if (states[Buttons::LEFT] && event.key.keysym.sym == SDLK_LEFT) {
                states[Buttons::LEFT] = false;
            }
            if (states[Buttons::RIGHT] && event.key.keysym.sym == SDLK_RIGHT) {
                states[Buttons::RIGHT] = false;
            }
            c.getController()->setButtons(states);
        }
    }
}

int Emulator::run() {
    isRunning = true;
    Console &c = Console::Instance();
    c.init(filename, drawerFunc, vertSyncHandler);
    pthread_create(&thread, NULL, &mainLoop, NULL);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoopStep, 0, 1);
#else
    while(isRunning) {
        mainLoopStep();
    }
#endif
    return 0;
}

Emulator::~Emulator() {
    pthread_join(thread, NULL);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
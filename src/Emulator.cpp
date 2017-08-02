//
// Created by lin-k on 01.10.2016.
//

#include "../include/Emulator.h"

static const uint32_t idealFrameTime = ((1. / 60) * 1000);
static Uint32 ticks;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Texture *texture;
static pthread_t thread;

Emulator::Emulator(std::string _filename) : filename(_filename), isRunning(true) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "NESEmulator");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderClear(renderer);
    ticks = SDL_GetTicks();
}

void drawerFunc(int x, int y, uint32_t color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *) p = color;
}

void vertSyncHandler(void) {
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
    Uint32 frametime = SDL_GetTicks() - ticks;
    if (frametime < idealFrameTime) {
        SDL_Delay(idealFrameTime - frametime);
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_GL_SwapWindow(window);

    ticks = SDL_GetTicks();
}

void *mainLoop(void *arg) {
    Console &c = Console::Instance();
    while(true)
        c.step();
    return arg;
}

int Emulator::run() {
    SDL_Event event;
    bool states[8] = {false, false, false, false, false, false, false, false};
    isRunning = true;
    Console &c = Console::Instance();
    c.init(filename, drawerFunc, vertSyncHandler);
    pthread_create(&thread, NULL, &mainLoop, NULL);
    while (true) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                return 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.repeat == 0) {
                    if (!states[0] && event.key.keysym.sym == SDLK_z) {
                        states[0] = true;
                    }
                    if (!states[1] && event.key.keysym.sym == SDLK_x) {
                        states[1] = true;
                    }
                    if (!states[2] && event.key.keysym.sym == SDLK_SPACE) {
                        states[2] = true;
                    }
                    if (!states[3] && event.key.keysym.sym == SDLK_RETURN) {
                        states[3] = true;
                    }
                    if (!states[4] && event.key.keysym.sym == SDLK_UP) {
                        states[4] = true;
                    }
                    if (!states[5] && event.key.keysym.sym == SDLK_DOWN) {
                        states[5] = true;
                    }
                    if (!states[6] && event.key.keysym.sym == SDLK_LEFT) {
                        states[6] = true;
                    }
                    if (!states[7] && event.key.keysym.sym == SDLK_RIGHT) {
                        states[7] = true;
                    }
                    c.getController()->setButtons(states);
                }
            } else if (event.type == SDL_KEYUP) {
                if (states[0] && event.key.keysym.sym == SDLK_z) {
                    states[0] = false;
                }
                if (states[1] && event.key.keysym.sym == SDLK_x) {
                    states[1] = false;
                }
                if (states[2] && event.key.keysym.sym == SDLK_SPACE) {
                    states[2] = false;
                }
                if (states[3] && event.key.keysym.sym == SDLK_RETURN) {
                    states[3] = false;
                }
                if (states[4] && event.key.keysym.sym == SDLK_UP) {
                    states[4] = false;
                }
                if (states[5] && event.key.keysym.sym == SDLK_DOWN) {
                    states[5] = false;
                }
                if (states[6] && event.key.keysym.sym == SDLK_LEFT) {
                    states[6] = false;
                }
                if (states[7] && event.key.keysym.sym == SDLK_RIGHT) {
                    states[7] = false;
                }
                c.getController()->setButtons(states);
            }
        }
    }
}

Emulator::~Emulator() {
    pthread_join(thread, NULL);
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
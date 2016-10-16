//
// Created by lin-k on 01.10.2016.
//

#include "../include/Emulator.h"

static const uint32_t idealFrameTime = ((1. / 60) * 1000) - 2;

Emulator::Emulator(std::string _filename) : filename(_filename), isRunning(true){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "NESEmulator");
    surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void Emulator::drawerFunc(int x, int y, int color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *) p = color;
}

void Emulator::vertSyncHandler() {
    Uint32 frametime = ticks - SDL_GetTicks();
    if (frametime < idealFrameTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds(idealFrameTime - frametime));
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
}

void Emulator::emuThread() {
    auto f1 = std::bind(&Emulator::drawerFunc, this, std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3);
    auto f2 = std::bind(&Emulator::vertSyncHandler, this);

    Console& c = Console::Instance();
    c.init(filename, f1, f2);
    while(isRunning) {
        ticks = SDL_GetTicks();
        c.step();
    }

    exit(0);
}

int Emulator::run() {
    SDL_Event event;
    bool states[8] = {false, false, false, false, false, false, false, false};
    isRunning = true;
    worker = std::thread(&Emulator::emuThread, this);

    Console& c = Console::Instance();

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
    worker.join();
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
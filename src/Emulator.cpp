//
// Created by lin-k on 01.10.2016.
//

#include "../include/Emulator.h"
#include "../apu_emu/Sound_Queue.h"
#include <thread>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

static const uint32_t idealFrameTime = ((1. / 60) * 1000);
static const long sample_rate = 44100;
static Uint32 ticks;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Texture *texture;
static std::thread thread;
static Uint32 vsyncEvent;
static bool isRunning;
static Sound_Queue *sound_queue;

void play_samples(const blip_sample_t* samples, int count) {
	sound_queue->write(samples, count);
}

Emulator::Emulator(std::string _filename) : filename(move(_filename)) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "NESEmulator");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                SCREEN_HEIGHT);
    SDL_RenderClear(renderer);
    vsyncEvent = SDL_RegisterEvents(1);
    sound_queue = new Sound_Queue;
    sound_queue->init(sample_rate);
    ticks = SDL_GetTicks();
}

void drawerFunc(int x, int y, uint32_t color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *) p = color;
}

void vertSyncHandler(void) {
    Uint32 frametime = SDL_GetTicks() - ticks;

    if (vsyncEvent != ((Uint32) -1)) {
        SDL_Event event;
        SDL_memset(&event, 0, sizeof(event));
        event.type = vsyncEvent;
        SDL_PushEvent(&event);
    }

    if (frametime < idealFrameTime) {
    	SDL_Delay(idealFrameTime - frametime);
    }

    ticks = SDL_GetTicks();
}

void mainLoop() {
    Console &c = Console::Instance();

    while (isRunning) {
	    c.frame();
    }
}

void mainLoopStep() {
    SDL_Event event;
    static bool states[8] = {false, false, false, false, false, false, false, false};
    Console &c = Console::Instance();
    if (SDL_PollEvent(&event)) {
        if (event.type == vsyncEvent) {
            SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
#ifndef __EMSCRIPTEN__
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

            }
        }
        if (event.type == SDL_KEYUP) {
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
        }
        c.getController()->setButtons(states);
#endif
    }
}

#ifdef __EMSCRIPTEN__
#define CMP(ev, kcode) if (strcmp(ev->code, kcode) == 0) \

static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent *event, void *userData) {
	static bool states[8] = {false, false, false, false, false, false, false, false};
	Controller *controller = (Controller *)userData;
	if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
		if (!states[Buttons::A] && event->keyCode == 90) {
			states[Buttons::A] = true;
		}
		if (!states[Buttons::B] && event->keyCode == 65) {
			states[Buttons::B] = true;
		}
		if (!states[Buttons::SELECT] && event->keyCode == 32) {
			states[Buttons::SELECT] = true;
		}
		if (!states[Buttons::START] && event->keyCode == 13) {
			states[Buttons::START] = true;
		}
		if (!states[Buttons::UP] && event->keyCode == 38) {
			states[Buttons::UP] = true;
		}
		if (!states[Buttons::DOWN] && event->keyCode == 40) {
			states[Buttons::DOWN] = true;
		}
		if (!states[Buttons::LEFT] && event->keyCode == 37) {
			states[Buttons::LEFT] = true;
		}
		if (!states[Buttons::RIGHT] && event->keyCode == 39) {
			states[Buttons::RIGHT] = true;
		}
	} else if (eventType == EMSCRIPTEN_EVENT_KEYUP) {
		if (states[Buttons::A] && event->keyCode == 90) {
			states[Buttons::A] = false;
		}
		if (states[Buttons::B] && event->keyCode == 65) {
			states[Buttons::B] = false;
		}
		if (states[Buttons::SELECT] && event->keyCode == 32) {
			states[Buttons::SELECT] = false;
		}
		if (states[Buttons::START] && event->keyCode == 13) {
			states[Buttons::START] = false;
		}
		if (states[Buttons::UP] && event->keyCode == 38) {
			states[Buttons::UP] = false;
		}
		if (states[Buttons::DOWN] && event->keyCode == 40) {
			states[Buttons::DOWN] = false;
		}
		if (states[Buttons::LEFT] && event->keyCode == 37) {
			states[Buttons::LEFT] = false;
		}
		if (states[Buttons::RIGHT] && event->keyCode == 39) {
			states[Buttons::RIGHT] = false;
		}
	}
	controller->setButtons(states);
	return EM_TRUE;
}
#endif

int Emulator::run() {
    isRunning = true;
    Console &c = Console::Instance();
    c.init(filename);
    thread = std::thread([&]() { mainLoop(); });
#ifdef __EMSCRIPTEN__
    emscripten_set_keydown_callback(NULL, c.getController(), true, keyCallback);
    emscripten_set_keyup_callback(NULL, c.getController(), true, keyCallback);
    emscripten_set_main_loop(mainLoopStep, 0, 1);
#else
    while(isRunning) {
        mainLoopStep();
        SDL_Delay(idealFrameTime / 5);
    }
#endif
    return 0;
}

Emulator::~Emulator() {
    thread.join();
    delete sound_queue;
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
//
// Created by lin-k on 01.10.2016.
//

#ifndef NESEMULATOR_EMULATOR_H
#define NESEMULATOR_EMULATOR_H

#include "Common.h"
#include "CPU.h"
#include "Memory.h"
#include "Controller.h"
#include <SDL2/SDL.h>
#include <pthread.h>

class Emulator {
public:
    Emulator(std::string filename);

    void emuThread();

    int run();

    void drawerFunc(int x, int y, int color);

    void vertSyncHandler();

    virtual ~Emulator();

protected:
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    std::thread worker;

    bool isRunning;
    CPU cpu;
    Memory mem;
    ROM rom;
    PPU ppu;
    Controller controller;
};


#endif //NESEMULATOR_EMULATOR_H

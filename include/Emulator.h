//
// Created by lin-k on 01.10.2016.
//

#ifndef NESEMULATOR_EMULATOR_H
#define NESEMULATOR_EMULATOR_H

#include "Common.h"
#include <SDL2/SDL.h>
#include "Console.h"

class Emulator {
public:
    Emulator(std::string _filename);

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

    Uint32 ticks;
    bool isRunning;
    std::string filename;
};


#endif //NESEMULATOR_EMULATOR_H

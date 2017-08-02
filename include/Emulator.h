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

    int run();

    virtual ~Emulator();

protected:
private:
    std::string filename;
};


#endif //NESEMULATOR_EMULATOR_H

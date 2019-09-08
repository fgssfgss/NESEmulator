//
// Created by lin-k on 15.10.2016.
//

#ifndef NESEMULATOR_CONSOLE_H
#define NESEMULATOR_CONSOLE_H

#pragma once

#include "Common.h"
#include "CPU.h"
#include "PPU.h"
#include "ROM.h"
#include "Memory.h"
#include "Controller.h"
#include "APU.h"

class PPU;
class CPU;
class Memory;
class ROM;
class Controller;
class APU;

class Console {
public:
    static Console &Instance() {
        static Console c;
        return c;
    }

    void frame();

    void init(std::string filename);

    CPU *getCPU();

    Memory *getMemory();

    ROM *getROM();

    PPU *getPPU();

    Controller *getController();

    APU *getAPU();

private:
    Console() {};

    ~Console() {};

    Console(Console const &) = delete;

    Console &operator=(Console const &) = delete;

    int timing_helper = 0;

    CPU *cpu;
    Memory *mem;
    ROM *rom;
    PPU *ppu;
    APU *apu;
    Controller *controller;
};


#endif //NESEMULATOR_CONSOLE_H

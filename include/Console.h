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

class PPU;
class CPU;
class Memory;
class ROM;
class Controller;

class Console {
public:
    static Console &Instance() {
        static Console c;
        return c;
    }

    void step();

    void init(std::string filename, std::function<void(int, int, int)> func, std::function<void(void)> vsync);

    CPU *getCPU();

    Memory *getMemory();

    ROM *getROM();

    PPU *getPPU();

    Controller *getController();

private:
    Console() {};

    ~Console() {};

    Console(Console const &) = delete;

    Console &operator=(Console const &) = delete;

    CPU *cpu;
    Memory *mem;
    ROM *rom;
    PPU *ppu;
    Controller *controller;
};


#endif //NESEMULATOR_CONSOLE_H

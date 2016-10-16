//
// Created by lin-k on 01.10.2016.
//

#ifndef NESEMULATOR_MEMORY_H
#define NESEMULATOR_MEMORY_H

#include "Common.h"
#include "Controller.h"
#include "Console.h"
#include "PPU.h"
#include "ROM.h"

class PPU;

class Memory {
public:
    Memory();

    virtual ~Memory();

    void init();

    uint8_t Read8(uint16_t addr);

    uint16_t Read16(uint16_t addr);

    void Write8(uint16_t addr, uint8_t value);

    void Write16(uint16_t addr, uint16_t value);

    int addCyclesAfterDMA = 0;
private:
    uint8_t ram[0x800];
    ROM *rom;
    PPU *ppu;
    Controller *controller;
};


#endif //NESEMULATOR_MEMORY_H

//
// Created by lin-k on 01.10.2016.
//

#ifndef NESEMULATOR_MEMORY_H
#define NESEMULATOR_MEMORY_H

#include "Common.h"
#include "PPU.h"
#include "ROM.h"
#include "Controller.h"

class PPU;

class Memory {
public:
    Memory();

    virtual ~Memory();

    void setROM(ROM *_rom);

    void setPPU(PPU *_ppu);

    void setController(Controller *_c);

    uint8_t Read8(uint16_t addr);

    uint16_t Read16(uint16_t addr);

    void Write8(uint16_t addr, uint8_t value);

    void Write16(uint16_t addr, uint16_t value);

    int addCyclesAfterDMA = 0;
protected:
private:

    uint8_t ram[0x800];
    ROM *rom;
    PPU *ppu;
    Controller *c;
};


#endif //NESEMULATOR_MEMORY_H

//
// Created by lin-k on 01.10.2016.
//


#include "../include/Memory.h"

Memory::Memory() {
    memset((void *) ram, 0xFF, sizeof(ram));
}

void Memory::init() {
    Console &c = Console::Instance();

    ppu = c.getPPU();
    rom = c.getROM();
    controller = c.getController();
}


uint8_t Memory::Read8(uint16_t addr) {
    // https://wiki.nesdev.com/w/index.php/CPU_memory_map
    if (addr >= 0 && addr <= 0x07FF) {
        return ram[addr];
    } else if (addr >= 0x0800 && addr <= 0x0FFF) {
        return ram[addr - 0x0800];
    } else if (addr >= 0x1000 && addr <= 0x17FF) {
        return ram[addr - 0x1000];
    } else if (addr >= 0x1800 && addr <= 0x1FFF) {
        return ram[addr - 0x1800];
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        return ppu->Read((addr - 0x2000) % 8);
    } else if (addr >= 0x4000 && addr <= 0x4017) {
        if (addr == 0x4016) {
            return controller->Read();
        }
    } else if (addr >= 0x4018 && addr <= 0x401F) {
        // I/O registers from test mode cpu, and ROM banks
    } else if (addr >= 0x4020 && addr <= 0xFFFF) {
        if (addr >= 0x6000 && addr <= 0x7FFF) {
            return rom->ReadSRAM(addr - 0x6000);
        } else if (addr >= 0x8000) {
            return rom->Read(addr - 0x8000);
        }
    }

    return 0;
}

uint16_t Memory::Read16(uint16_t addr) {
    return (Read8(addr) | (Read8(addr + 1) << 8));
}

void Memory::Write8(uint16_t addr, uint8_t value) {
    if (addr >= 0 && addr <= 0x07FF) {
        ram[addr] = value;
    } else if (addr >= 0x0800 && addr <= 0x0FFF) {
        ram[addr - 0x0800] = value;
    } else if (addr >= 0x1000 && addr <= 0x17FF) {
        ram[addr - 0x1000] = value;
    } else if (addr >= 0x1800 && addr <= 0x1FFF) {
        ram[addr - 0x1800] = value;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        ppu->Write(addr % 8, value);
    } else if (addr >= 0x4000 && addr <= 0x4017) {
        if (addr == 0x4014) { // broken stuff
            uint8_t *where = ppu->getOAMRAM();
            for (uint16_t i = 0; i < 0x100; i++) {
                where[i] = Read8((value << 8) + i);
            }
            addCyclesAfterDMA = 513;
        } else if (addr == 0x4016) {
            controller->Write(value);
        }
    } else if (addr >= 0x4018 && addr <= 0x401F) {
        // I/O registers from test mode cpu
    } else if (addr >= 0x4020 && addr <= 0xFFFF) {
        if (addr >= 0x6000 && addr <= 0x7FFF) {
            rom->WriteSRAM(addr - 0x6000, value);
        } else {
            rom->Write(addr, value);
        }

    }
}

void Memory::Write16(uint16_t addr, uint16_t value) {
    uint8_t lo = (uint8_t) value;
    uint8_t hi = ((value >> 8) & 0xFF);

    Write8(addr, lo);
    Write8(addr + 1, hi);
}


Memory::~Memory() {
}
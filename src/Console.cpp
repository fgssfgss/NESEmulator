//
// Created by lin-k on 15.10.2016.
//

#include <SDL_timer.h>
#include "../include/Console.h"

void Console::init(std::string filename, void (*draw)(int, int, uint32_t), void (*vsync)()) {
    rom = new ROM();
    ppu = new PPU();
    cpu = new CPU();
    apu = new APU();
    mem = new Memory();
    controller = new Controller();

    rom->init(filename);
    mem->init();
    cpu->init();
    ppu->init();
    this->draw = draw;
    this->vsync = vsync;
}

void Console::step() {
    int cycles = cpu->execute();
    if (mem->addCyclesAfterDMA == 513) { // kostyl, for better synchronization
        cycles += mem->addCyclesAfterDMA;
        mem->addCyclesAfterDMA = 0;
    }

    for (int i = 0; i < cycles * 3; i++) {
        ppu->execute();
        rom->execute();
    }
}

void Console::callVSync() {
    vsync();
}

void Console::putPixel(int x, int y, uint32_t color) {
    draw(x, y, color);
}

Controller *Console::getController() {
    return controller;
}

CPU *Console::getCPU() {
    return cpu;
}

PPU *Console::getPPU() {
    return ppu;
}

Memory *Console::getMemory() {
    return mem;
}

ROM *Console::getROM() {
    return rom;
}

APU *Console::getAPU() {
    return apu;
}
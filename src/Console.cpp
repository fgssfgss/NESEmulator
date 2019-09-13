//
// Created by lin-k on 15.10.2016.
//

#include <SDL_timer.h>
#include "../include/Console.h"

void Console::init(std::string filename) {
    timing_helper = 0;
    rom = new ROM();
    apu = new APU();
    ppu = new PPU(apu);
    cpu = new CPU();
    mem = new Memory();
    controller = new Controller();
    apu->init();
    rom->init(std::move(filename));
    mem->init();
    cpu->init();
    ppu->init();
}

void Console::frame() {
    int cpu_cycles = 0;
    timing_helper ^= 1;

    for (int i = 0; i < 89341 + timing_helper;) {
        cpu_cycles = cpu->execute();

        i += cpu_cycles * 3;

        for (int j = 0; j < (cpu_cycles * 3); ++j) {
            ppu->execute();
        }
    }

    getAPU()->step();
    getAPU()->stepFrame();
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
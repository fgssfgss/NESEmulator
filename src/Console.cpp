//
// Created by lin-k on 15.10.2016.
//

#include <SDL_timer.h>
#include "../include/Console.h"

void Console::init(std::string filename) {
    prev_frame = 0;
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

int Console::step() {
    int frame = 0;
    int cpu_cycles = cpu->execute();
    for (int i = 0; i < (3*cpu_cycles); ++i) {
    	frame = ppu->execute();
    }
    return frame;
}

void Console::frame() {
    int frame = 0;

    do {
        frame = step();
    } while (prev_frame == frame);

    prev_frame = frame;

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
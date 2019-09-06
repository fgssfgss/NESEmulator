//
// Created by lin-k on 15.10.2016.
//

#include <SDL_timer.h>
#include "../include/Console.h"

void Console::init(std::string filename, const long sample_rate) {
    rom = new ROM();
    apu = new APU();
    ppu = new PPU(apu);
    cpu = new CPU();
    mem = new Memory();
    controller = new Controller();
    apu->init(sample_rate);
    rom->init(std::move(filename));
    mem->init();
    cpu->init();
    ppu->init();
}

void Console::frame() {
    int prev_frame = 0;
    int frame = 0;
    int frame_chg = 0;
    int cpu_cycles = 0;

    long cpu_c = getCPU()->getTotalCycles();

    for (int i = 0; i < 89341; ++i) {
    	if (cpu_cycles < 29780 && i % 3 == 0) {
            cpu_cycles += cpu->execute();
    	}

    	prev_frame = frame;
    	frame = ppu->execute();
    	if (prev_frame != frame) {
	        frame_chg++;
    	}
    }

    getAPU()->step();

    std::cout << "frame changed " << frame_chg << " cpu cycles is " << getCPU()->getTotalCycles() - cpu_c << std::endl;
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
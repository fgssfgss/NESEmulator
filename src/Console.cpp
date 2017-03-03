//
// Created by lin-k on 15.10.2016.
//

#include "../include/Console.h"

void Console::init(std::string filename, std::function<void(int, int, int)> func, std::function<void(void)> vsync) {
    rom = new ROM();
    ppu = new PPU();
    cpu = new CPU();
    mem = new Memory();
    controller = new Controller();

    rom->init(filename);
    mem->init();
    cpu->init();
    ppu->init();
    ppu->setPixelWriterHandler(func, vsync);
}

void Console::step() {
    auto t1 = std::chrono::system_clock::now();

    int cycles = cpu->execute();
    if (mem->addCyclesAfterDMA == 513) { // kostyl, for better synchronization
        cycles += mem->addCyclesAfterDMA;
        mem->addCyclesAfterDMA = 0;
    }

    for (int i = 0; i < cycles * 3; i++) {
        ppu->execute();
        rom->execute();
    }

    auto t2 = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = t2-t1;
    if(diff.count() == 0.0)
        return;
    std::cout << "Duration of tick: " << diff.count() << "s" << std::endl;
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
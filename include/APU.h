//
// Created by lin-k on 26.12.2018.
//

#ifndef NESEMULATOR_AUDIO_H
#define NESEMULATOR_AUDIO_H

#include "Common.h"
#include "CPU.h"
#include <stdlib.h>
#include "../apu_emu/Simple_Apu.h"

class CPU;

class APU {
public:
    APU();

	void init(const long sample_rate);

    uint8_t read(uint16_t address);

    void write(uint16_t address, uint8_t value);

    void stepFrame();

	void step(void (*cb)(const blip_sample_t* samples, long count));

    ~APU();
};


#endif //NESEMULATOR_AUDIO_H

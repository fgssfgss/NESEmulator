//
// Created by lin-k on 26.12.2018.
//

#include "SDL.h"
#include "../include/APU.h"

static Simple_Apu *apu;

static int read_dmc(void* _, cpu_addr_t addr) {
	Console &c = Console::Instance();
	return (int)c.getMemory()->Read8(addr);
}

static void irq_handle(void *_) {
	Console &c = Console::Instance();
	c.getCPU()->IRQ();
}

APU::APU() {
	apu = new Simple_Apu;
}

void APU::init(const long sample_rate) {
	apu->sample_rate(sample_rate);
	apu->dmc_reader(read_dmc, NULL);
	apu->irq_notifier(irq_handle, NULL);
}

uint8_t APU::read(uint16_t address) {
	if (address == 0x4015) {
		return apu->read_status();
	} else {
		return 0;
	}
}

void APU::write(uint16_t address, uint8_t value) {
	apu->write_register(address, (int)value);
}

void APU::stepFrame() {
	apu->end_frame();
}

void APU::step(void (*cb)(const blip_sample_t* samples, long count)) {
	const int buf_size = 2048;
	static blip_sample_t buf[buf_size];

	long count = apu->read_samples(buf, buf_size);
	cb(buf, count);
}

APU::~APU() {
	delete apu;
}

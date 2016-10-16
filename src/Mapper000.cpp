//
// Created by lin-k on 15.10.2016.
//

#include "../include/Mapper000.h"

Mapper000::Mapper000(struct FileHeader *_header) {
    params = _header;
}

Mapper000::~Mapper000() {
}

void Mapper000::execute() {
    // nothing to do in nrom
}

uint32_t Mapper000::mapToROM(uint16_t addr) {
    if(params->sizePrgRom == 1) {
        return addr % 16384;
    } else {
        return addr;
    }
}

uint32_t Mapper000::mapToVROM(uint16_t addr) {
    return addr;
}

int Mapper000::mirroringStatus() {
    return params->flagsPPUDetails & 1;
}

void Mapper000::writeHandler(uint16_t addr, uint8_t value) {
    return;
}
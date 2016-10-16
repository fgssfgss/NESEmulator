//
// Created by lin-k on 15.10.2016.
//

#include "../include/Mapper004.h"


Mapper004::Mapper004(struct FileHeader *_header) {
    params = _header;
}

Mapper004::~Mapper004() {
}

void Mapper004::execute() {

}

uint32_t Mapper004::mapToROM(uint16_t addr) {
    // I will think about it
    return addr;
}

uint32_t Mapper004::mapToVROM(uint16_t addr) {
    // and about it
    return addr;
}

int Mapper004::mirroringStatus() {
    return params->flagsPPUDetails & 1;
}

void Mapper004::writeHandler(uint16_t addr, uint8_t value) {
    // and about it
}
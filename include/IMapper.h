//
// Created by lin-k on 15.10.2016.
//

#ifndef NESEMULATOR_IMAPPER_H
#define NESEMULATOR_IMAPPER_H

#include "Common.h"

// Mapper Interface

// no definition, only declare, heh

class IMapper {
public:
    virtual int mirroringStatus() = 0;
    virtual void execute() = 0; // need for irq, which mapper can invoke
    virtual uint32_t mapToROM(uint16_t addr) = 0; // maps address from NES Addressing to INES format rom
    virtual uint32_t mapToVROM(uint16_t addr) = 0; // maps address from NES Addressing to INES format vrom
    virtual void writeHandler(uint16_t addr, uint8_t value) = 0; // changes mapping and other things
};


#endif //NESEMULATOR_IMAPPER_H

//
// Created by lin-k on 15.10.2016.
//

#ifndef NESEMULATOR_MAPPER004_H
#define NESEMULATOR_MAPPER004_H

#include "Common.h"
#include "ROM.h"
#include "IMapper.h"


class Mapper004 : IMapper {
public:
    Mapper004(struct FileHeader *header);

    ~Mapper004();

    void execute();

    uint32_t mapToROM(uint16_t addr);

    uint32_t mapToVROM(uint16_t addr);

    int mirroringStatus();

    void writeHandler(uint16_t addr, uint8_t value);

private:
    void handleScanline();

    void switchBanks();

    uint32_t calcRBankOffset(int8_t idx);

    uint32_t calcVBankOffset(int8_t idx);

    struct FileHeader *params;
    bool irqEnabled = false;
    uint8_t rommode;
    uint8_t vrommode;
    uint8_t reloadVal = 0;
    uint8_t counter = 0;
    bool mirroring;
    uint32_t ROMBanks[4];
    uint32_t VROMBanks[8];
    uint8_t reg;
    uint8_t regs[8];
};


#endif //NESEMULATOR_MAPPER004_H

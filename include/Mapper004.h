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
    struct FileHeader *params;
};



#endif //NESEMULATOR_MAPPER004_H

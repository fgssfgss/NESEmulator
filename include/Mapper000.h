//
// Created by lin-k on 15.10.2016.
//

#ifndef NESEMULATOR_MAPPER000_H
#define NESEMULATOR_MAPPER000_H

#include "Common.h"
#include "ROM.h"
#include "IMapper.h"


class Mapper000 : IMapper {
public:
    Mapper000(struct FileHeader *header);
    ~Mapper000();
    void execute();
    uint32_t mapToROM(uint16_t addr);
    uint32_t mapToVROM(uint16_t addr);
    int mirroringStatus();
    void writeHandler(uint16_t addr, uint8_t value);
private:
    struct FileHeader *params;
};


#endif //NESEMULATOR_MAPPER000_H

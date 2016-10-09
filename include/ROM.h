//
// Created by lin-k on 01.10.2016.
//

#ifndef NESEMULATOR_ROM_H
#define NESEMULATOR_ROM_H

#include "Common.h"

#pragma pack(push, 1)
struct FileHeader {
    char magic[4]; // 0-3 bytes
    uint8_t sizePrgRom; // 4 byte
    uint8_t sizeChrRom; // 5 byte
    uint8_t flagsPPUDetails; // 6 byte
    uint8_t flagsOther0; // 7 byte
    uint8_t sizePrgRam; // 8 byte
    uint8_t flagsPalNtsc; // 9 byte
    uint8_t flagsOther1; // 10 byte
    uint8_t zeroes[5]; // 15 bytes
};
#pragma pack(pop)

class ROM {
public:
    ROM(std::string filename);

    ~ROM();

    uint8_t Read(uint16_t address);

    uint8_t ReadCHR(uint16_t address);

    uint8_t MirroringStatus();

private:
    uint8_t mapperNumber;
    FileHeader header;
    uint8_t *romBanks;
    uint8_t *vromBanks;
};


#endif //NESEMULATOR_ROM_H

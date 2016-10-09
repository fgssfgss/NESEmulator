//
// Created by lin-k on 01.10.2016.
//

#include "../include/ROM.h"

ROM::ROM(std::string filename) {
    std::ifstream in(filename.c_str(), std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Can\'t Open File\n";
        in.close();
        exit(0);
    }
    in.read((char *) &header, sizeof(FileHeader));
    header.magic[3] = '\0';
    std::cout << header.magic << std::endl;
    std::cout << "PRG ROM size:" << (int) header.sizePrgRom << std::endl;
    std::cout << "CHR ROM size:" << (int) header.sizeChrRom << std::endl;
    std::cout << "PPU Flags:" << std::bitset<8>(header.flagsPPUDetails) << std::endl;

    mapperNumber = (header.flagsPPUDetails >> 4);
    if (mapperNumber != 0) {
        std::cout << "Sorry, only NROM Mapper is working!" << std::endl;
        return;
    }

    romBanks = new uint8_t[header.sizePrgRom * 16384 * sizeof(uint8_t)];
    in.read((char *) romBanks, header.sizePrgRom * 16384 * sizeof(uint8_t));

    vromBanks = new uint8_t[header.sizeChrRom * 8192 * sizeof(uint8_t)];
    in.read((char *) vromBanks, header.sizeChrRom * 8192 * sizeof(uint8_t));
    in.close();
}

ROM::~ROM() {
}

uint8_t ROM::MirroringStatus() {
    return header.flagsPPUDetails & 1; // 0: vertical/horizontal, 1: horizontal/vertical
}

uint8_t ROM::ReadCHR(uint16_t address) {
    return vromBanks[address];
}

uint8_t ROM::Read(uint16_t address) {
    if (header.sizePrgRom == 1)
        return romBanks[address % 16384];
    else
        return romBanks[address];
}

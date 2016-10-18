//
// Created by lin-k on 01.10.2016.
//

#include "../include/ROM.h"

void ROM::init(std::string filename) {
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

    romBanks = new uint8_t[header.sizePrgRom * 16384 * sizeof(uint8_t)];
    in.read((char *) romBanks, header.sizePrgRom * 16384 * sizeof(uint8_t));

    vromBanks = new uint8_t[header.sizeChrRom * 8192 * sizeof(uint8_t)];
    in.read((char *) vromBanks, header.sizeChrRom * 8192 * sizeof(uint8_t));
    in.close();

    if (mapperNumber == 0) {
        mapper = (IMapper *) new Mapper000(&header);
    } else if (mapperNumber == 4) {
        mapper = (IMapper *) new Mapper004(&header);
    } else {
        std::cout << "Sorry, only NROM and MMC3 Mapper is working! Current: " << (int) mapperNumber << std::endl;
        exit(0);
    }

}

ROM::ROM() {
    memset(SRAM, 0x00, sizeof(SRAM));
}

ROM::~ROM() {
}

uint8_t ROM::MirroringStatus() {
    return mapper->mirroringStatus(); // 0: vertical/horizontal, 1: horizontal/vertical
}

void ROM::execute() {
    mapper->execute();
}

uint8_t ROM::ReadCHR(uint16_t address) {
    return vromBanks[mapper->mapToVROM(address)]; // this in mapper too
}

uint8_t ROM::Read(uint16_t address) {
    return romBanks[mapper->mapToROM(address)];
}

void ROM::Write(uint16_t addr, uint8_t value) {
    mapper->writeHandler(addr, value);
}

uint8_t ROM::ReadSRAM(uint16_t address) {
    if (!(header.flagsPPUDetails & (1 << 1))) {
        return 0;
    }
    return SRAM[address];
}

void ROM::WriteSRAM(uint16_t addr, uint8_t value) {
    if (!(header.flagsPPUDetails & (1 << 1))) {
        return;
    }
    SRAM[addr] = value;
}
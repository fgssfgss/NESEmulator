//
// Created by lin-k on 02.10.2016.
//

#ifndef NESEMULATOR_PPU_H
#define NESEMULATOR_PPU_H

#include "Common.h"
#include "Console.h"
#include "ROM.h"
#include "CPU.h"

class CPU;

enum FlagsCTRL {
    flagNameTable = 3,
    flagIncrement = 1 << 2,
    flagSpriteTable = 1 << 3,
    flagBackgroundTable = 1 << 4,
    flagSpriteSize = 1 << 5,
    flagMasterSlave = 1 << 6,
    flagNmi = 1 << 7
};

enum FlagsMask {
    flagGrayScale = 0,
    flagShowLeftBackground = 1 << 1,
    flagShowLeftSprites = 1 << 2,
    flagShowBackground = 1 << 3,
    flagShowSprites = 1 << 4,
    flagRedTint = 1 << 5,
    flagGreenTint = 1 << 6,
    flagBlueTint = 1 << 7
};

enum FlagsSTATUS {
    flagSpriteOverflow = 1 << 5,
    flagSprite0 = 1 << 6,
    flagNmiOccured = 1 << 7
};

class PPU {
public:
    PPU();

    void Reset();

    void init();

    void Write(uint16_t addr, uint8_t value);

    uint8_t Read(uint16_t addr);

    uint8_t ReadRam(uint16_t addr);

    uint8_t *getOAMRAM();

    void getCycleScanlineRendering(int &_cycle, int &_scanline, bool &isRendering);

    void WriteRam(uint16_t addr, uint8_t value);

    void setVSync();

    void clearVSync();

    void nmiChange();

    void incrementY();

    void incrementX();

    void copyX();

    void copyY();

    void fetchNameTableByte();

    void fetchAttributeTableByte();

    void fetchLowTileByte();

    void fetchHighTileByte();

    void storeTileData();

    uint32_t fetchTileData();

    uint8_t backgroundPixel();

    void spritePixel(uint8_t &index, uint8_t &sprite);

    void renderPixel();

    uint32_t fetchSpritePattern(int i, int row);

    void evaluateSprites();

    uint8_t readPalette(uint16_t addr);

    void writePalette(uint16_t addr, uint8_t value);

    void tick();

    int execute(); // one cycle

    ~PPU();

private:
    uint16_t trnslt_addr(uint16_t addr);

    uint8_t PPUCTRL;
    uint8_t PPUMASK;
    uint8_t PPUSTATUS;
    uint8_t OAMADDR;
    uint8_t dummy_reg;

    int cycle;
    int scanline;
    uint64_t frame;
    int mirroringType;
    int f;
    int nmiDelay;
    bool prevNmi = false;

    uint16_t vramCur;
    uint16_t vramTmp;
    bool writeToggle;
    uint16_t xscroll;
    uint8_t buffered;

    uint8_t nameTableByte;
    uint8_t attributeTableByte;
    uint8_t lowTileByte;
    uint8_t highTileByte;
    uint64_t tileData;

    int spriteCount;
    uint32_t spritePatterns[8];
    uint8_t spritePositions[8];
    uint8_t spritePriorities[8];
    uint8_t spriteIndexes[8];

    uint8_t ram[0x800];
    uint8_t oamram[0x100];
    uint8_t oampal[0x20];

    uint32_t palette[64] = {
            0x666666, 0x002A88, 0x1412A7, 0x3B00A4, 0x5C007E, 0x6E0040, 0x6C0600, 0x561D00,
            0x333500, 0x0B4800, 0x005200, 0x004F08, 0x00404D, 0x000000, 0x000000, 0x000000,
            0xADADAD, 0x155FD9, 0x4240FF, 0x7527FE, 0xA01ACC, 0xB71E7B, 0xB53120, 0x994E00,
            0x6B6D00, 0x388700, 0x0C9300, 0x008F32, 0x007C8D, 0x000000, 0x000000, 0x000000,
            0xFFFEFF, 0x64B0FF, 0x9290FF, 0xC676FF, 0xF36AFF, 0xFE6ECC, 0xFE8170, 0xEA9E22,
            0xBCBE00, 0x88D800, 0x5CE430, 0x45E082, 0x48CDDE, 0x4F4F4F, 0x000000, 0x000000,
            0xFFFEFF, 0xC0DFFF, 0xD3D2FF, 0xE8C8FF, 0xFBC2FF, 0xFEC4EA, 0xFECCC5, 0xF7D8A5,
            0xE4E594, 0xCFEF96, 0xBDF4AB, 0xB3F3CC, 0xB5EBF2, 0xB8B8B8, 0x000000, 0x000000,
    };

    ROM *rom;
    CPU *cpu;
};


#endif //NESEMULATOR_PPU_H

//
// Created by lin-k on 02.10.2016.
//

#include "../include/PPU.h"

PPU::PPU() {
    memset(ram0, 0xff, sizeof(ram0));
    memset(ram1, 0xff, sizeof(ram1));
    memset(oamram, 0xff, sizeof(oamram));
    memset(spritePositions, 0x00, sizeof(spritePositions));
    memset(spritePriorities, 0x00, sizeof(spritePriorities));
    memset(spritePatterns, 0x00, sizeof(spritePatterns));
    memset(spriteIndexes, 0x00, sizeof(spriteIndexes));

    // by default it will be like that
    nt1 = ram0;
    nt2 = ram0;
    nt3 = ram0;
    nt4 = ram0;
    rom = NULL;

    dummy_reg = 0;
    PPUCTRL = 0x0;
    PPUMASK = 0x0;
    PPUSTATUS = 0;
    OAMADDR = 0x0;
    scanline = 240;
    cycle = 340;
    frame = 0;
    f = 0;
    nmiDelay = 0;
    writeToggle = true;
    vramCur = 0;
    vramTmp = 0;
    xscroll = 0;
    spriteCount = 0;
    nameTableByte = 0;
    attributeTableByte = 0;
    lowTileByte = 0;
    highTileByte = 0;
    tileData = 0;
    buffered = 0;

    uint8_t p[] = {0x09, 0x01, 0x00, 0x01, 0x00, 0x02, 0x02, 0x0D,
                   0x08, 0x10, 0x08, 0x24, 0x00, 0x00, 0x04, 0x2C, 0x09, 0x01, 0x34,
                   0x03, 0x00, 0x04, 0x00, 0x14, 0x08, 0x3A, 0x00, 0x02, 0x00, 0x20,
                   0x2C, 0x08};
    for (int i = 0; i < 32; i++) {
        oampal[i] = p[i];
    }
}

void PPU::init() { // for NMI
    Console &c = Console::Instance();

    cpu = c.getCPU();
    rom = c.getROM();

    mappingType = rom->MirroringStatus(); // setting initial mapping vram banks
    if (mappingType == 0) {
        nt1 = ram0;
        nt2 = ram0;
        nt3 = ram1;
        nt4 = ram1;
    } else if (mappingType == 1) {
        nt1 = ram0;
        nt2 = ram1;
        nt3 = ram0;
        nt4 = ram1;
    }
}

PPU::~PPU() {
}

void PPU::Write(uint16_t addr, uint8_t value) {
    dummy_reg = value;
    switch (addr) {
        case 0: // PPUCTRL
        {
            PPUCTRL = value;
            vramTmp = (vramTmp & 0xF3FF) | ((uint16_t(value) & 0x03) << 10);
            nmiChange();
            break;
        }
        case 1: // PPUMASK
        {
            PPUMASK = value;
            break;
        }
        case 3: // OAMADDR
        {
            OAMADDR = value;
            break;
        }
        case 4: // OAMDATA
        {
            oamram[OAMADDR] = value;
            OAMADDR++;
            break;
        }
        case 5: // PPUSCROLL
        {
            if (writeToggle) {
                vramTmp = ((vramTmp & 0xFFE0) | (value >> 3));
                xscroll = value & 0x07;
                writeToggle = false;
            } else {
                vramTmp = ((vramTmp & 0x8FFF) | ((value & 0x07) << 12));
                vramTmp = ((vramTmp & 0xFC1F) | ((value & 0xF8) << 2));
                writeToggle = true;
            }
            break;
        }
        case 6: // PPUADDR
        {
            if (writeToggle) {
                vramTmp = (vramTmp & 0x80FF) | ((value & 0x3F) << 8);
                vramTmp &= ~(1 << 15);
                writeToggle = false;
            } else {
                vramTmp = ((vramTmp & 0xFF00) | uint16_t(value));
                vramCur = vramTmp;
                writeToggle = true;
            }
            break;
        }
        case 7: // PPUDATA
        {
            WriteRam(vramCur, value);
            if (PPUCTRL & flagIncrement) {
                vramCur += 32;
            } else {
                vramCur++;
            }
            break;
        }
    }
}

uint8_t PPU::Read(uint16_t addr) {
    switch (addr) {
        case 2: // PPUSTATUS
        {
            uint8_t newStatus = (PPUSTATUS & 0xE0) | (dummy_reg & 0x1F);
            PPUSTATUS &= ~flagNmiOccured;
            nmiChange();
            writeToggle = true;
            return newStatus;
        }
        case 4: // OAMDATA
        {
            return oamram[OAMADDR++];
        }
        case 7: // PPUDATA
        {
            uint8_t data = ReadRam(vramCur);
            if ((vramCur % 0x4000) < 0x3F00) {
                uint8_t buf = buffered;
                buffered = data;
                data = buf;
            } else {
                buffered = ReadRam((vramCur & 0x3FFF) - 0x1000);
            }

            if (PPUCTRL & flagIncrement) {
                vramCur += 32;
            } else {
                vramCur++;
            }
            return data;
        }
    }

    return 0;
}

uint8_t PPU::ReadRam(uint16_t addr) {
    addr %= 0x4000;
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        return rom->ReadCHR(addr);
    } else if (addr >= 0x2000 && addr <= 0x23FF) {
        return nt1[addr - 0x2000];
    } else if (addr >= 0x2400 && addr <= 0x27FF) {
        return nt2[addr - 0x2400];
    } else if (addr >= 0x2800 && addr <= 0x2BFF) {
        return nt3[addr - 0x2800];
    } else if (addr >= 0x2C00 && addr <= 0x2FFF) {
        return nt4[addr - 0x2C00];
    } else if (addr >= 0x3000 && addr <= 0x3EFF) {
        return nt1[addr - 0x3000];
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        return readPalette((addr - 0x3F00) % 0x20);
    }
    return 0;
}

void PPU::WriteRam(uint16_t addr, uint8_t value) {
    addr %= 0x4000;
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        // lol
    } else if (addr >= 0x2000 && addr <= 0x23FF) {
        nt1[addr - 0x2000] = value;
    } else if (addr >= 0x2400 && addr <= 0x27FF) {
        nt2[addr - 0x2400] = value;
    } else if (addr >= 0x2800 && addr <= 0x2BFF) {
        nt3[addr - 0x2800] = value;
    } else if (addr >= 0x2C00 && addr <= 0x2FFF) {
        nt4[addr - 0x2C00] = value;
    } else if (addr >= 0x3000 && addr <= 0x3EFF) {
        nt1[addr - 0x3000] = value;
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        writePalette((addr - 0x3F00) % 0x20, value);
    }
}

void PPU::Reset() {
    cycle = 0;
    scanline = 0;
    frame = 0;
}

uint8_t *PPU::getOAMRAM() {
    return oamram;
}

void PPU::incrementX() {
    if ((vramCur & 0x001F) == 31) {
        vramCur &= ~0x001F;
        vramCur ^= 0x0400;
    } else {
        vramCur += 1;
    }
}

void PPU::incrementY() {
    if ((vramCur & 0x7000) == 0x7000) {
        vramCur &= ~0x7000;
        int y = (vramCur & 0x03E0) >> 5;
        if (y == 29) {
            y = 0;
            vramCur ^= 0x0800;
        } else {
            y = (y + 1) & 31;
        }
        vramCur = (vramCur & ~0x03E0) | (y << 5);
    } else {
        vramCur += 0x1000;
    }
}

void PPU::copyX() {
    vramCur = ((vramCur & 0xFBE0) | (vramTmp & 0x041F));
}

void PPU::copyY() {
    vramCur = ((vramCur & 0x841F) | (vramTmp & 0x7BE0));
}

void PPU::fetchNameTableByte() {
    uint16_t v = vramCur;
    uint16_t addr = (0x2000 | (v & 0x0FFF));
    nameTableByte = ReadRam(addr);
}

void PPU::fetchAttributeTableByte() {
    uint16_t v = vramCur;
    uint16_t addr = (0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07));
    uint16_t shift = (((v >> 4) & 4) | (v & 2));
    attributeTableByte = ((ReadRam(addr) >> shift) & 3) << 2;
}

void PPU::fetchLowTileByte() {
    uint8_t fineY = ((vramCur >> 12) & 7);
    uint8_t table = (PPUCTRL & flagBackgroundTable) ? 1 : 0;
    uint8_t tile = nameTableByte;
    uint16_t addr = (0x1000 * table) + (tile * 0x10) + fineY;
    lowTileByte = ReadRam(addr);
}

void PPU::fetchHighTileByte() {
    uint8_t fineY = ((vramCur >> 12) & 7);
    uint8_t table = (PPUCTRL & flagBackgroundTable) ? 1 : 0;
    uint8_t tile = nameTableByte;
    uint16_t addr = (0x1000 * table) + (tile * 0x10) + fineY;
    highTileByte = ReadRam(addr + 8);
}

void PPU::storeTileData() {
    uint32_t data = 0;
    for (int i = 0; i < 8; i++) {
        uint8_t a = attributeTableByte;
        uint8_t p1 = ((lowTileByte & 0x80) >> 7);
        uint8_t p2 = ((highTileByte & 0x80) >> 6);
        lowTileByte <<= 1;
        highTileByte <<= 1;
        data <<= 4;
        data |= uint32_t(a | p1 | p2);
    }
    tileData |= uint64_t(data);
}

uint32_t PPU::fetchTileData() {
    return uint32_t(tileData >> 32);
}

uint8_t PPU::backgroundPixel() {
    if (!(PPUMASK & flagShowBackground)) {
        return 0;
    }
    uint32_t data = (fetchTileData() >> ((7 - xscroll) * 4));
    return uint8_t(data & 0x0F);
}

void PPU::spritePixel(uint8_t &index, uint8_t &sprite) {
    if (!(PPUMASK & flagShowSprites)) {
        index = 0;
        sprite = 0;
        return;
    }
    for (int i = 0; i < spriteCount; i++) {
        int offset = (cycle - 1) - spritePositions[i];
        if (offset < 0 || offset > 7) {
            continue;
        }
        offset = 7 - offset;
        uint8_t color = uint8_t((spritePatterns[i] >> (offset * 4)) & 0x0F);
        if ((color % 4) == 0) {
            continue;
        }
        index = i;
        sprite = color;
        return;
    }
    index = 0;
    sprite = 0;
    return;
}

void PPU::renderPixel() {
    int x = cycle - 1;
    int y = scanline;
    uint8_t background = backgroundPixel();
    uint8_t sprite, i;
    spritePixel(i, sprite);
    if (x < 8 && !(PPUMASK & flagShowLeftBackground)) {
        background = 0;
    }
    if (x < 8 && !(PPUMASK & flagShowLeftSprites)) {
        sprite = 0;
    }
    bool b = ((background % 4) != 0);
    bool s = ((sprite % 4) != 0);
    uint8_t color = 0;

    if (!b && !s) {
        color = 0;
    } else if (!b && s) {
        color = sprite | 0x10;
    } else if (b && !s) {
        color = background;
    } else {
        if (spriteIndexes[i] == 0 && x < 255) {
            PPUSTATUS |= flagSprite0;
        }
        if (spritePriorities[i] == 0) {
            color = (sprite | 0x10);
        } else {
            color = background;
        }
    }

    drawer(x, y, palette[readPalette(color) % 64]);
}

uint8_t PPU::readPalette(uint16_t addr) {
    if (addr >= 16 && (addr % 4) == 0) {
        addr -= 16;
    }
    return oampal[addr];
}

void PPU::writePalette(uint16_t addr, uint8_t value) {
    if (addr >= 16 && (addr % 4) == 0) {
        addr -= 16;
    }
    oampal[addr] = value;
}

uint32_t PPU::fetchSpritePattern(int i, int row) {
    uint8_t tile = oamram[i * 4 + 1];
    uint8_t attributes = oamram[i * 4 + 2];
    uint16_t address = 0;
    if (!(PPUCTRL & flagSpriteSize)) {
        if (attributes & 0x80) {
            row = 7 - row;
        }
        uint8_t table = (PPUCTRL & flagSpriteTable) ? 1 : 0;
        address = 0x1000 * table + tile * 16 + row;
    } else if (PPUCTRL & flagSpriteSize) {
        if (attributes & 0x80) {
            row = 15 - row;
        }
        uint8_t table = (tile & 1);
        tile &= 0xFE;
        if (row > 7) {
            tile++;
            row -= 8;
        }
        address = 0x1000 * table + tile * 16 + row;
    }
    uint8_t a = (attributes & 3) << 2;
    lowTileByte = ReadRam(address);
    highTileByte = ReadRam(address + 8);
    uint32_t data = 0;

    for (int j = 0; j < 8; j++) {
        uint8_t p1 = 0, p2 = 0;
        if ((attributes & 0x40) == 0x40) {
            p1 = (lowTileByte & 1) << 0;
            p2 = (highTileByte & 1) << 1;
            lowTileByte >>= 1;
            highTileByte >>= 1;
        } else {
            p1 = (lowTileByte & 0x80) >> 7;
            p2 = (highTileByte & 0x80) >> 6;
            lowTileByte <<= 1;
            highTileByte <<= 1;
        }
        data <<= 4;
        data |= uint32_t(a | p1 | p2);
    }
    return data;
}

void PPU::evaluateSprites() {
    int h = 0;
    if (PPUCTRL & flagSpriteSize) {
        h = 16;
    } else {
        h = 8;
    }
    int count = 0;
    for (int i = 0; i < 64; i++) {
        uint8_t y = oamram[i * 4 + 0];
        uint8_t a = oamram[i * 4 + 2];
        uint8_t x = oamram[i * 4 + 3];
        int row = scanline - y;
        if (row < 0 || row >= h) {
            continue;
        }
        if (count < 8) {
            spritePatterns[count] = fetchSpritePattern(i, row);
            spritePositions[count] = x;
            spritePriorities[count] = ((a >> 5) & 1);
            spriteIndexes[count] = uint8_t(i);
        }
        count++;
    }
    if (count > 8) {
        count = 8;
        PPUSTATUS |= flagSpriteOverflow;
    }
    spriteCount = count;
}

void PPU::setVSync() {
    vsyncHandler();
    PPUSTATUS |= flagNmiOccured;
    nmiChange();
}

void PPU::clearVSync() {
    PPUSTATUS &= ~flagNmiOccured;
    nmiChange();
}

void PPU::nmiChange() {
    bool was = ((PPUSTATUS & flagNmiOccured) && (PPUCTRL & flagNmi));
    if (was && !prevNmi) {
        nmiDelay = 15;
    }
    prevNmi = was;
}

void PPU::tick() {
    if (nmiDelay > 0) {
        nmiDelay--;
        if (nmiDelay == 0 && (PPUCTRL & flagNmi) && (PPUSTATUS & flagNmiOccured)) {
            cpu->NMI();
        }
    }

    if ((PPUMASK & flagShowBackground) || (PPUMASK & flagShowSprites)) {
        if (f == 1 && scanline == 261 && cycle == 339) {
            cycle = 0;
            scanline = 0;
            frame++;
            f ^= 1;
            return;
        }
    }
    cycle++;
    if (cycle > 340) {
        cycle = 0;
        scanline++;
        if (scanline > 261) {
            scanline = 0;
            frame++;
            f ^= 1;
        }
    }
}

int PPU::execute() {
    tick();

    if (mappingType != rom->MirroringStatus()) {
        mappingType = rom->MirroringStatus();

        if (mappingType == 0) {
            nt1 = ram0;
            nt1 = ram0;
            nt2 = ram0;
            nt3 = ram1;
            nt4 = ram1;
        } else if (mappingType == 1) {
            nt1 = ram0;
            nt2 = ram1;
            nt3 = ram0;
            nt4 = ram1;
        }
    }

    bool renderingEnabled = ((PPUMASK & flagShowBackground) || (PPUMASK & flagShowSprites));
    bool preLine = (scanline == 261);
    bool visibleLine = (scanline < 240);
    bool renderLine = (preLine || visibleLine);
    bool prefetchCycle = (cycle >= 321 && cycle <= 336);
    bool visibleCycle = (cycle >= 1 && cycle <= 256);
    bool fetchCycle = (prefetchCycle || visibleCycle);

    if (renderingEnabled) {
        if (visibleLine && visibleCycle) {
            renderPixel();
        }
        if (renderLine && fetchCycle) {
            tileData <<= 4;
            switch (cycle % 8) {
                case 1:
                    fetchNameTableByte();
                    break;
                case 3:
                    fetchAttributeTableByte();
                    break;
                case 5:
                    fetchLowTileByte();
                    break;
                case 7:
                    fetchHighTileByte();
                    break;
                case 0:
                    storeTileData();
                    break;
            }
        }
        if (preLine && cycle >= 280 && cycle <= 304) {
            copyY();
        }
        if (renderLine) {
            if (fetchCycle && ((cycle % 8) == 0)) {
                incrementX();
            }
            if (cycle == 256) {
                incrementY();
            }
            if (cycle == 257) {
                copyX();
            }
        }
    }

    if (renderingEnabled) {
        if (cycle == 257) {
            if (visibleLine) {
                evaluateSprites();
            } else {
                spriteCount = 0;
            }
        }
    }

    if (scanline == 241 && cycle == 1) {
        setVSync();
    }

    if (preLine && cycle == 1) {
        clearVSync();
        PPUSTATUS &= (~flagSprite0);
        PPUSTATUS &= (~flagSpriteOverflow);
    }

    return frame;
}

void PPU::setPixelWriterHandler(std::function<void(int, int, int)> func, std::function<void(void)> vsync) {
    drawer = func;
    vsyncHandler = vsync;
}

void PPU::getCycleScanlineRendering(int &_cycle, int &_scanline, bool &isRendering) {
    _cycle = cycle;
    _scanline = scanline;
    isRendering = ((PPUMASK & flagShowBackground) && (PPUMASK & flagShowSprites));
}
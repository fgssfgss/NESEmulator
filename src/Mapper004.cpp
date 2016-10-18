//
// Created by lin-k on 15.10.2016.
//

#include "../include/Mapper004.h"
#include "../include/Console.h"


Mapper004::Mapper004(struct FileHeader *_header) {
    params = _header;
    mirroring = (params->flagsPPUDetails & 1);
    ROMBanks[0] = calcRBankOffset(0);
    ROMBanks[1] = calcRBankOffset(1);
    ROMBanks[2] = calcRBankOffset(-2);
    ROMBanks[3] = calcRBankOffset(-1);
    memset(regs, 0x00, sizeof(regs));
}

Mapper004::~Mapper004() {
}

void Mapper004::execute() {
    int cycle, scanline;
    bool render;
    Console &c = Console::Instance();
    c.getPPU()->getCycleScanlineRendering(cycle, scanline, render);
    if (cycle == 280) {
        return;
    }
    if (scanline > 239 && scanline < 261) {
        return;
    }
    if (render) {
        return;
    }
    handleScanline();
}

uint32_t Mapper004::mapToROM(uint16_t addr) {
    uint16_t bank = addr / 0x2000;
    uint16_t offset = addr % 0x2000;
    return ROMBanks[bank] + offset;
}

uint32_t Mapper004::mapToVROM(uint16_t addr) {
    uint16_t bank = addr / 0x0400;
    uint16_t offset = addr % 0x0400;
    return VROMBanks[bank] + offset;
}

int Mapper004::mirroringStatus() {
    return mirroring;
}

void Mapper004::writeHandler(uint16_t addr, uint8_t value) {
    if (addr >= 0x8000 && addr <= 0x9FFE && addr % 2 == 0) {
        rommode = ((value >> 6) & 1);
        vrommode = ((value >> 7) & 1);
        reg = value & 0x7;
        switchBanks();
    } else if (addr >= 0x8001 && addr <= 0x9FFF && addr % 2 == 1) {
        regs[reg] = value;
        switchBanks();
    } else if (addr >= 0xA000 && addr <= 0xBFFE && addr % 2 == 0) {
        mirroring = (value & 1);
    } else if (addr >= 0xA001 && addr <= 0xBFFF && addr % 2 == 1) {
        // ram protect(do we need this right now?)
    } else if (addr >= 0xC000 && addr <= 0xDFFE && addr % 2 == 0) {
        reloadVal = value;
    } else if (addr >= 0xC001 && addr <= 0xDFFF && addr % 2 == 1) {
        counter = reloadVal;
    } else if (addr >= 0xE000 && addr <= 0xFFFE && addr % 2 == 0) {
        irqEnabled = false;
    } else if (addr >= 0xE001 && addr <= 0xFFFF && addr % 2 == 1) {
        irqEnabled = true;
    }
}

uint32_t Mapper004::calcRBankOffset(int8_t idx) {
    if (idx > 0x80) {
        idx -= 0x100;
    }
    idx %= ((params->sizePrgRom * 16384) / 0x2000);
    int32_t offset = idx * 0x2000;
    if (offset < 0) {
        offset += (params->sizePrgRom * 16384);
    }
    return uint32_t(offset);
}

uint32_t Mapper004::calcVBankOffset(int8_t idx) {
    if (idx > 0x80) {
        idx -= 0x100;
    }
    idx %= ((params->sizeChrRom * 8192) / 0x0400);
    int32_t offset = idx * 0x0400;
    if (offset < 0) {
        offset += (params->sizeChrRom * 8192);
    }
    return uint32_t(offset);
}

void Mapper004::switchBanks() {
    if (rommode == 0) {
        ROMBanks[0] = calcRBankOffset(regs[6]);
        ROMBanks[1] = calcRBankOffset(regs[7]);
        ROMBanks[2] = calcRBankOffset(-2);
        ROMBanks[3] = calcRBankOffset(-1);
    } else if (rommode == 1) {
        ROMBanks[0] = calcRBankOffset(-2);
        ROMBanks[1] = calcRBankOffset(regs[7]);
        ROMBanks[2] = calcRBankOffset(regs[6]);
        ROMBanks[3] = calcRBankOffset(-1);
    }

    if (vrommode == 0) {
        VROMBanks[0] = calcVBankOffset(regs[0] & 0xFE);
        VROMBanks[1] = calcVBankOffset(regs[0] | 0x01);
        VROMBanks[2] = calcVBankOffset(regs[1] & 0xFE);
        VROMBanks[3] = calcVBankOffset(regs[1] | 0x01);
        VROMBanks[4] = calcVBankOffset(regs[2]);
        VROMBanks[5] = calcVBankOffset(regs[3]);
        VROMBanks[6] = calcVBankOffset(regs[4]);
        VROMBanks[7] = calcVBankOffset(regs[5]);
    } else if (vrommode == 1) {
        VROMBanks[0] = calcVBankOffset(regs[2]);
        VROMBanks[1] = calcVBankOffset(regs[3]);
        VROMBanks[2] = calcVBankOffset(regs[4]);
        VROMBanks[3] = calcVBankOffset(regs[5]);
        VROMBanks[4] = calcVBankOffset(regs[0] & 0xFE);
        VROMBanks[5] = calcVBankOffset(regs[0] | 0x01);
        VROMBanks[6] = calcVBankOffset(regs[1] & 0xFE);
        VROMBanks[7] = calcVBankOffset(regs[1] | 0x01);
    }
}

void Mapper004::handleScanline() {
    if (counter == 0) {
        counter = reloadVal;
    } else {
        counter--;
        if (counter == 0 && irqEnabled) {
            Console &c = Console::Instance();
            c.getCPU()->IRQ();
        }
    }
}
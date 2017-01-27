//
// Created by lin-k on 01.10.2016.
//

#include "../include/CPU.h"

CPU::CPU() {
    PC = 0xC000;
    A = 0;
    X = 0;
    Y = 0;
    S = 0xFD;
    I = 0;
    B = 0;
    U = 1;
    Z = 0;
    N = 0;
    V = 0;
    C = 0;
    m = NULL;
    cycles = 0;
}

CPU::~CPU() {
}

void CPU::setPC(uint16_t value) {
    PC = value;
}

void CPU::init() {
    Console& c = Console::Instance();

    m = c.getMemory();

    m->Write16(0x0008, 0xF7);
    m->Write16(0x0009, 0xEF);
    m->Write16(0x000A, 0xDF);
    m->Write16(0x000F, 0xBF);
    setPC(m->Read16(0xFFFC));
}

/* Addressing routines */

uint8_t CPU::immediate_addr() {
    return m->Read8(PC++);
}

int8_t CPU::relative_addr() {
    return int8_t(m->Read8(PC++)); // for branch ops
}

uint16_t CPU::zeropage_addr_j() {
    return m->Read8(PC++) & 0xFF;
}

uint16_t CPU::zeropage_x_addr_j() {
    uint16_t addr = uint16_t(m->Read8(PC++));
    addr += X;
    return addr & 0xFF;
}

uint16_t CPU::zeropage_y_addr_j() {
    uint16_t addr = uint16_t(m->Read8(PC++));
    addr += Y;
    return addr & 0xFF;
}

uint16_t CPU::absolute_addr_j() { // *_j - read only address, needed for jsr, jmp
    PC += 2;
    return m->Read16(PC - 2);
}

uint16_t CPU::absolute_x_addr_j() {
    uint16_t addr = m->Read16(PC);
    PC += 2;
    addr += X;
    return addr;
}

uint16_t CPU::absolute_y_addr_j() {
    uint16_t addr = m->Read16(PC);
    PC += 2;
    addr += Y;
    return addr;
}

uint16_t CPU::indexed_indirect_addr_j() { // (MEM, X)
    uint16_t addr = uint16_t(m->Read8(PC++));
    addr += X;
    uint16_t new_addr = m->Read8(addr & 0xFF) | (m->Read8((addr + 1) & 0xFF) << 8);
    return new_addr;
}

uint16_t CPU::indirect_indexed_addr_j() { // (MEM), Y
    uint16_t addr = uint16_t(m->Read8(PC++));
    uint16_t new_addr = m->Read8(addr & 0xFF) | (m->Read8((addr + 1) & 0xFF) << 8);
    new_addr = (new_addr + Y) & 0xFFFF;
    return new_addr;
}

uint8_t CPU::zeropage_addr() { // zeropage addressing
    return m->Read8(zeropage_addr_j());
}

uint8_t CPU::absolute_addr() { // read address arg and read value by it
    return m->Read8(absolute_addr_j());
}

uint8_t CPU::zeropage_x_addr() {
    return m->Read8(zeropage_x_addr_j());
}

uint8_t CPU::zeropage_y_addr() {
    return m->Read8(zeropage_y_addr_j());
}

uint8_t CPU::absolute_x_addr() {
    return m->Read8(absolute_x_addr_j());
}

uint8_t CPU::absolute_y_addr() {
    return m->Read8(absolute_y_addr_j());
}

uint8_t CPU::indexed_indirect_addr() { // (MEM, X)
    return m->Read8(indexed_indirect_addr_j());
}

uint8_t CPU::indirect_indexed_addr() { // (MEM), Y
    return m->Read8(indirect_indexed_addr_j());
}

/* Addressing routines end */

/* Stack routines */

uint8_t CPU::pop8() {
    S++;
    uint8_t data = m->Read8(0x0100 | S);
    return data;
}

uint16_t CPU::pop16() {
    uint8_t lo = pop8();
    uint8_t hi = pop8();
    uint16_t data = (hi << 8) | lo;
    return data;
}

void CPU::push8(uint8_t data) {
    m->Write8(0x0100 | S, data);
    S--;
}

void CPU::push16(uint16_t data) {
    uint8_t lo = data & 0xFF;
    uint8_t hi = data >> 8;
    push8(hi);
    push8(lo);
}

/* Stack routines end */

uint8_t CPU::flag_reg_value() {
    uint8_t n = 0;
    n |= N;
    n |= V;
    n |= (U << 5);
    n |= (B << 4);
    n |= (D << 3);
    n |= (I << 2);
    n |= (Z ? 1 : 0);
    n |= (C & (1 << 7)) ? 1 : 0;
    return n;
}

void CPU::NMI() {
    push16(PC);
    push8(flag_reg_value());
    uint16_t newPC = m->Read16(0xFFFA);
    setPC(newPC);
    I = 1;
}

void CPU::IRQ() {
    push16(PC);
    push8(flag_reg_value());
    uint16_t newPC = m->Read16(0xFFFE);
    setPC(newPC);
    I = 1;
}

int CPU::execute() {
    cycles = 0;
    uint8_t instr = m->Read8(PC);
    PC++;

    switch (instr) {
        case 0x00: // BRK
        {
            cycles = 7;
            BRK();
            break;
        }
        case 0x01: // ORA (indirect,X)
        {
            cycles = 6;
            ORA(indexed_indirect_addr());
            break;
        }
        case 0x05: // ORA zeropage
        {
            cycles = 3;
            ORA(zeropage_addr());
            break;
        }
        case 0x06: // ASL zeropage
        {
            cycles = 5;
            uint16_t address = zeropage_addr_j();
            m->Write8(address, ASL(m->Read8(address)));
            break;
        }
        case 0x08: // PHP
        {
            cycles = 3;
            PHP();
            break;
        }
        case 0x09: // ORA immediate
        {
            cycles = 2;
            ORA(immediate_addr());
            break;
        }
        case 0x0a: // ASL accumulator
        {
            cycles = 2;
            A = ASL(A);
            break;
        }
        case 0x0d: // ORA absolute
        {
            cycles = 4;
            ORA(absolute_addr());
            break;
        }
        case 0x0e: // ASL absolute
        {
            cycles = 6;
            uint16_t address = absolute_addr_j();
            m->Write8(address, ASL(m->Read8(address)));
            break;
        }
        case 0x10: // BPL
        {
            cycles = 2;
            BPL(relative_addr());
            break;
        }
        case 0x11: // ORA (indirect), Y
        {
            cycles = 5;
            ORA(indirect_indexed_addr());
            break;
        }
        case 0x15: // ORA zeropage, X
        {
            cycles = 4;
            ORA(zeropage_x_addr());
            break;
        }
        case 0x16: // ASL zeropage, X
        {
            cycles = 6;
            uint16_t address = zeropage_x_addr_j();
            m->Write8(address, ASL(m->Read8(address)));
            break;
        }
        case 0x18: // CLC
        {
            cycles = 2;
            CLC();
            break;
        }
        case 0x19: // ORA absolute, Y
        {
            cycles = 4;
            ORA(absolute_y_addr());
            break;
        }
        case 0x1d: // ORA absolute, X
        {
            cycles = 4;
            ORA(absolute_x_addr());
            break;
        }
        case 0x1e: // ASL absolute, X
        {
            cycles = 7;
            uint16_t address = absolute_x_addr_j();
            m->Write8(address, ASL(m->Read8(address)));
            break;
        }
        case 0x20: // JSR
        {
            cycles = 6;
            JSR(absolute_addr_j());
            break;
        }
        case 0x21: // AND (indirect, X)
        {
            cycles = 6;
            AND(indexed_indirect_addr());
            break;
        }
        case 0x24: // BIT zeropage
        {
            cycles = 3;
            BIT(zeropage_addr());
            break;
        }
        case 0x25: // AND zeropage
        {
            cycles = 3;
            AND(zeropage_addr());
            break;
        }
        case 0x26: // ROL zeropage
        {
            cycles = 5;
            uint16_t address = zeropage_addr_j();
            m->Write8(address, ROL(m->Read8(address)));
            break;
        }
        case 0x28: // PLP
        {
            cycles = 4;
            PLP();
            break;
        }
        case 0x29: // AND immediate
        {
            cycles = 2;
            AND(immediate_addr());
            break;
        }
        case 0x2a: // ROL A
        {
            cycles = 2;
            A = ROL(A);
            break;
        }
        case 0x2c: // BIT absolute
        {
            cycles = 4;
            BIT(absolute_addr());
            break;
        }
        case 0x2d: // AND absolute
        {
            cycles = 4;
            AND(absolute_addr());
            break;
        }
        case 0x2e: // ROL absolute
        {
            cycles = 6;
            uint16_t address = absolute_addr_j();
            m->Write8(address, ROL(m->Read8(address)));
            break;
        }
        case 0x30: // BMI
        {
            cycles = 2;
            BMI(relative_addr());
            break;
        }
        case 0x31: // AND (indirect), Y
        {
            cycles = 5;
            AND(indirect_indexed_addr());
            break;
        }
        case 0x35: // AND zeropage, X
        {
            cycles = 4;
            AND(zeropage_x_addr());
            break;
        }
        case 0x36: // ROL zeropage, X
        {
            cycles = 6;
            uint16_t address = zeropage_x_addr_j();
            m->Write8(address, ROL(m->Read8(address)));
            break;
        }
        case 0x38: // SEC
        {
            cycles = 2;
            SEC();
            break;
        }
        case 0x39: // AND absolute, Y
        {
            cycles = 4;
            AND(absolute_y_addr());
            break;
        }
        case 0x3d: // AND absolute, X
        {
            cycles = 4;
            AND(absolute_x_addr());
            break;
        }
        case 0x3e: // ROL absolute, X
        {
            cycles = 7;
            uint16_t address = absolute_x_addr_j();
            m->Write8(address, ROL(m->Read8(address)));
            break;
        }
        case 0x40: // RTI
        {
            cycles = 6;
            RTI();
            break;
        }
        case 0x41: // EOR (indirect, X)
        {
            cycles = 6;
            EOR(indexed_indirect_addr());
            break;
        }
        case 0x45: // EOR zeropage
        {
            cycles = 3;
            EOR(zeropage_addr());
            break;
        }
        case 0x46: // LSR zeropage
        {
            cycles = 5;
            uint16_t address = zeropage_addr_j();
            m->Write8(address, LSR(m->Read8(address)));
            break;
        }
        case 0x48: // PHA
        {
            cycles = 3;
            PHA();
            break;
        }
        case 0x49: // EOR immediate
        {
            cycles = 2;
            EOR(immediate_addr());
            break;
        }
        case 0x4a: // LSR accumulator
        {
            cycles = 2;
            A = LSR(A);
            break;
        }
        case 0x4c: // JMP absolute
        {
            cycles = 3;
            JMP(absolute_addr_j());
            break;
        }
        case 0x4d: // EOR absolute
        {
            cycles = 4;
            EOR(absolute_addr());
            break;
        }
        case 0x4e: // LSR absolute
        {
            cycles = 6;
            uint16_t address = absolute_addr_j();
            m->Write8(address, LSR(m->Read8(address)));
            break;
        }
        case 0x50: // BVC
        {
            cycles = 2;
            BVC(relative_addr());
            break;
        }
        case 0x51: // EOR (indirect), Y
        {
            cycles = 5;
            EOR(indirect_indexed_addr());
            break;
        }
        case 0x55: // EOR zeropage, X
        {
            cycles = 4;
            EOR(zeropage_x_addr());
            break;
        }
        case 0x56: // LSR zeropage, X
        {
            cycles = 6;
            uint16_t address = zeropage_x_addr_j();
            m->Write8(address, LSR(m->Read8(address)));
            break;
        }
        case 0x58: // CLI
        {
            cycles = 2;
            CLI();
            break;
        }
        case 0x59: // EOR absolute, Y
        {
            cycles = 4;
            EOR(absolute_y_addr());
            break;
        }
        case 0x5d: // EOR absolute, X
        {
            cycles = 4;
            EOR(absolute_x_addr());
            break;
        }
        case 0x5e: // LSR absolute, X
        {
            cycles = 7;
            uint16_t address = absolute_x_addr_j();
            m->Write8(address, LSR(m->Read8(address)));
            break;
        }
        case 0x60: // RTS
        {
            cycles = 6;
            RTS();
            break;
        }
        case 0x61: // ADC (indirect, X)
        {
            cycles = 6;
            ADC(indexed_indirect_addr());
            break;
        }
        case 0x65: // ADC zeropage
        {
            cycles = 3;
            ADC(zeropage_addr());
            break;
        }
        case 0x66: // ROR zeropage
        {
            cycles = 5;
            uint16_t address = zeropage_addr_j();
            m->Write8(address, ROR(m->Read8(address)));
            break;
        }
        case 0x68: // PLA
        {
            cycles = 4;
            PLA();
            break;
        }
        case 0x69: // ADC immediate
        {
            cycles = 2;
            ADC(immediate_addr());
            break;
        }
        case 0x6a: // ROR A
        {
            cycles = 2;
            A = ROR(A);
            break;
        }
        case 0x6c: // JMP indirect
        {
            cycles = 5;
            uint16_t old_addr = absolute_addr_j();
            uint16_t addr = m->Read16(old_addr);
            if ((old_addr & 0xFF) == 0xFF) // emulating bug
                addr = m->Read8(old_addr) | (m->Read8(old_addr & 0xFF00) << 8);
            JMP(addr);
            break;
        }
        case 0x6d: // ADC absolute
        {
            cycles = 4;
            ADC(absolute_addr());
            break;
        }
        case 0x6e: // ROR absolute
        {
            cycles = 6;
            uint16_t address = absolute_addr_j();
            m->Write8(address, ROR(m->Read8(address)));
            break;
        }
        case 0x70: // BVS
        {
            cycles = 2;
            BVS(relative_addr());
            break;
        }
        case 0x71: // ADC (indirect), Y
        {
            cycles = 5;
            ADC(indirect_indexed_addr());
            break;
        }
        case 0x75: // ADC zeropage, X
        {
            cycles = 4;
            ADC(zeropage_x_addr());
            break;
        }
        case 0x76: // ROR zeropage, X
        {
            cycles = 6;
            uint16_t address = zeropage_x_addr_j();
            m->Write8(address, ROR(m->Read8(address)));
            break;
        }
        case 0x78: // SEI
        {
            cycles = 2;
            SEI();
            break;
        }
        case 0x79: // ADC absolute, Y
        {
            ADC(absolute_y_addr());
            break;
        }
        case 0x7d: // ADC absolute, X
        {
            cycles = 4;
            ADC(absolute_x_addr());
            break;
        }
        case 0x7e: // ROR absolute, X
        {
            cycles = 7;
            uint16_t address = absolute_x_addr_j();
            m->Write8(address, ROR(m->Read8(address)));
            break;
        }
        case 0x81: // STA (indirect, X)
        {
            cycles = 6;
            m->Write8(indexed_indirect_addr_j(), A);
            break;
        }
        case 0x84: // STY zeropage
        {
            cycles = 3;
            m->Write8(zeropage_addr_j(), Y);
            break;
        }
        case 0x85: // STA zeropage
        {
            cycles = 3;
            m->Write8(zeropage_addr_j(), A);
            break;
        }
        case 0x86: // STX zeropage
        {
            cycles = 3;
            m->Write8(zeropage_addr_j(), X);
            break;
        }
        case 0x88: // DEY
        {
            cycles = 2;
            DEY();
            break;
        }
        case 0x8a: // TXA
        {
            cycles = 2;
            TXA();
            break;
        }
        case 0x8c: // STY absolute
        {
            cycles = 4;
            m->Write8(absolute_addr_j(), Y);
            break;
        }
        case 0x8d: // STA absolute
        {
            cycles = 4;
            m->Write8(absolute_addr_j(), A);
            break;
        }
        case 0x8e: // STX absolute
        {
            cycles = 4;
            m->Write8(absolute_addr_j(), X);
            break;
        }
        case 0x90: // BCC
        {
            cycles = 2;
            BCC(relative_addr());
            break;
        }
        case 0x91: // STA (indirect), Y
        {
            cycles = 6;
            m->Write8(indirect_indexed_addr_j(), A);
            break;
        }
        case 0x94: // STY zeropage, X
        {
            cycles = 4;
            m->Write8(zeropage_x_addr_j(), Y);
            break;
        }
        case 0x95: // STA zeropage, X
        {
            cycles = 4;
            m->Write8(zeropage_x_addr_j(), A);
            break;
        }
        case 0x96: // STX zeropage, Y
        {
            cycles = 4;
            m->Write8(zeropage_y_addr_j(), X);
            break;
        }
        case 0x98: // TYA
        {
            cycles = 2;
            TYA();
            break;
        }
        case 0x99: // STA absolute, Y
        {
            cycles = 5;
            m->Write8(absolute_y_addr_j(), A);
            break;
        }
        case 0x9a: // TXS
        {
            cycles = 2;
            TXS();
            break;
        }
        case 0x9d: // STA absolute, X
        {
            cycles = 5;
            m->Write8(absolute_x_addr_j(), A);
            break;
        }
        case 0xa0: // LDY immediate
        {
            cycles = 2;
            LDY(immediate_addr());
            break;
        }
        case 0xa1: // LDA (indirect, X)
        {
            cycles = 6;
            LDA(indexed_indirect_addr());
            break;
        }
        case 0xa2: // LDX immediate
        {
            cycles = 2;
            LDX(immediate_addr());
            break;
        }
        case 0xa4: // LDY zeropage
        {
            cycles = 3;
            LDY(zeropage_addr());
            break;
        }
        case 0xa5: // LDA zeropage
        {
            cycles = 3;
            LDA(zeropage_addr());
            break;
        }
        case 0xa6: // LDX zeropage
        {
            cycles = 3;
            LDX(zeropage_addr());
            break;
        }
        case 0xa8: // TAY
        {
            cycles = 2;
            TAY();
            break;
        }
        case 0xa9: // LDA Immediate
        {
            cycles = 2;
            LDA(immediate_addr());
            break;
        }
        case 0xaa: // TAX
        {
            cycles = 2;
            TAX();
            break;
        }
        case 0xac: // LDY absolute
        {
            cycles = 4;
            LDY(absolute_addr());
            break;
        }
        case 0xad: // LDA absolute
        {
            cycles = 4;
            LDA(absolute_addr());
            break;
        }
        case 0xae: // LDX absolute
        {
            cycles = 4;
            LDX(absolute_addr());
            break;
        }
        case 0xb0: // BCS
        {
            cycles = 2;
            BCS(relative_addr());
            break;
        }
        case 0xb1: // LDA (indirect), Y
        {
            cycles = 5;
            LDA(indirect_indexed_addr());
            break;
        }
        case 0xb4: // LDY zeropage, X
        {
            cycles = 4;
            LDY(zeropage_x_addr());
            break;
        }
        case 0xb5: // LDA zeropage, X
        {
            cycles = 4;
            LDA(zeropage_x_addr());
            break;
        }
        case 0xb6: // LDX zeropage, Y
        {
            cycles = 4;
            LDX(zeropage_y_addr());
            break;
        }
        case 0xb8: // CLV
        {
            cycles = 2;
            CLV();
            break;
        }
        case 0xb9: // LDA absolute, Y
        {
            cycles = 4;
            LDA(absolute_y_addr());
            break;
        }
        case 0xba: // TSX
        {
            cycles = 2;
            TSX();
            break;
        }
        case 0xbc: // LDY absolute, X
        {
            cycles = 4;
            LDY(absolute_x_addr());
            break;
        }
        case 0xbd: // LDA absolute, X
        {
            cycles = 4;
            LDA(absolute_x_addr());
            break;
        }
        case 0xbe: // LDX absolute, Y
        {
            cycles = 4;
            LDX(absolute_y_addr());
            break;
        }
        case 0xc0: // CPY immediate
        {
            cycles = 2;
            CMP(Y, immediate_addr());
            break;
        }
        case 0xc1: // CMP (indirect,X)
        {
            cycles = 6;
            CMP(A, indexed_indirect_addr());
            break;
        }
        case 0xc4: // CPY zeropage
        {
            cycles = 3;
            CMP(Y, zeropage_addr());
            break;
        }
        case 0xc5: // CMP zeropage
        {
            cycles = 3;
            CMP(A, zeropage_addr());
            break;
        }
        case 0xc6: // DEC zeropage
        {
            cycles = 5;
            uint16_t address = zeropage_addr_j();
            m->Write8(address, DEC(m->Read8(address)));
            break;
        }
        case 0xc8: // INY
        {
            cycles = 2;
            INY();
            break;
        }
        case 0xc9: // CMP immediate
        {
            cycles = 2;
            CMP(A, immediate_addr());
            break;
        }
        case 0xca: // DEX
        {
            cycles = 2;
            DEX();
            break;
        }
        case 0xcc: // CPY absolute
        {
            cycles = 4;
            CMP(Y, absolute_addr());
            break;
        }
        case 0xcd: // CMP absolute
        {
            cycles = 4;
            CMP(A, absolute_addr());
            break;
        }
        case 0xce: // DEC absolute
        {
            cycles = 6;
            uint16_t address = absolute_addr_j();
            m->Write8(address, DEC(m->Read8(address)));
            break;
        }
        case 0xd0: // BNE
        {
            cycles = 2;
            BNE(relative_addr());
            break;
        }
        case 0xd1: // CMP (indirect), Y
        {
            cycles = 5;
            CMP(A, indirect_indexed_addr());
            break;
        }
        case 0xd5: // CMP zeropage, X
        {
            cycles = 4;
            CMP(A, zeropage_x_addr());
            break;
        }
        case 0xd6: // DEC zeropage, X
        {
            cycles = 6;
            uint16_t address = zeropage_x_addr_j();
            m->Write8(address, DEC(m->Read8(address)));
            break;
        }
        case 0xd8: // CLD
        {
            cycles = 2;
            CLD();
            break;
        }
        case 0xd9: // CMP absolute, Y
        {
            cycles = 4;
            CMP(A, absolute_y_addr());
            break;
        }
        case 0xdd: // CMP absolute, X
        {
            cycles = 4;
            CMP(A, absolute_x_addr());
            break;
        }
        case 0xde: // DEC absolute, X
        {
            cycles = 7;
            uint16_t address = absolute_x_addr_j();
            m->Write8(address, DEC(m->Read8(address)));
            break;
        }
        case 0xe0: // CPX immediate
        {
            cycles = 2;
            CMP(X, immediate_addr());
            break;
        }
        case 0xe1: // SBC (indirect, X)
        {
            cycles = 6;
            SBC(indexed_indirect_addr());
            break;
        }
        case 0xe4: // CPX zeropage
        {
            cycles = 3;
            CMP(X, zeropage_addr());
            break;
        }
        case 0xe5: // SBC zeropage
        {
            cycles = 3;
            SBC(zeropage_addr());
            break;
        }
        case 0xe6: // INC zeropage
        {
            cycles = 5;
            uint16_t address = zeropage_addr_j();
            m->Write8(address, INC(m->Read8(address)));
            break;
        }
        case 0xe8: // INX
        {
            cycles = 2;
            INX();
            break;
        }
        case 0xe9: // SBC immediate
        {
            cycles = 2;
            SBC(immediate_addr());
            break;
        }
        case 0xea: // NOP
        {
            cycles = 2;
            NOP();
            break;
        }
        case 0xec: // CPX absolute
        {
            cycles = 4;
            CMP(X, absolute_addr());
            break;
        }
        case 0xed: // SBC absolute
        {
            cycles = 4;
            SBC(absolute_addr());
            break;
        }
        case 0xee: // INC absolute
        {
            cycles = 6;
            uint16_t address = absolute_addr_j();
            m->Write8(address, INC(m->Read8(address)));
            break;
        }
        case 0xf0: // BEQ
        {
            cycles = 2;
            BEQ(relative_addr());
            break;
        }
        case 0xf1: // SBC (indirect), Y
        {
            cycles = 5;
            SBC(indirect_indexed_addr());
            break;
        }
        case 0xf5: // SBC zeropage, X
        {
            cycles = 4;
            SBC(zeropage_x_addr());
            break;
        }
        case 0xf6: // INC zeropage,X
        {
            cycles = 6;
            uint16_t address = zeropage_x_addr_j();
            m->Write8(address, INC(m->Read8(address)));
            break;
        }
        case 0xf8: // SED
        {
            cycles = 2;
            SED();
            break;
        }
        case 0xf9: // SBC absolute, Y
        {
            cycles = 4;
            SBC(absolute_y_addr());
            break;
        }
        case 0xfd: // SBC absolute, X
        {
            cycles = 4;
            SBC(absolute_x_addr());
            break;
        }
        case 0xfe: // INC absolute, X
        {
            cycles = 7;
            uint16_t address = absolute_x_addr_j();
            m->Write8(address, INC(m->Read8(address)));
            break;
        }

        default:
            printf("Unknown opcode at 0x%04x!\n", PC - 1);
            return 1;
    }

    //LOGW("OOPS");

    return cycles;
}

void CPU::TYA() {
    A = Y;
    Z = A;
    N = A;
}

void CPU::TXS() {
    S = X;
}

void CPU::TXA() {
    A = X;
    Z = A;
    N = A;
}

void CPU::TSX() {
    X = S;
    Z = X;
    N = X;
}

void CPU::TAY() {
    Y = A;
    Z = Y;
    N = Y;
}

void CPU::TAX() {
    X = A;
    Z = X;
    N = X;
}

void CPU::SEI() {
    I = 1;
}

void CPU::SED() {
    D = 1;
}

void CPU::SEC() {
    C = 1;
}

void CPU::SBC(uint8_t argument) {
    ADC(~argument);
}

void CPU::RTS() {
    setPC(pop16() + 1);
}

void CPU::RTI() {
    PLP();
    I = 0;
    B = 0;
    uint16_t oldPC = pop16();
    setPC(oldPC);
}

uint8_t CPU::ROR(uint8_t argument) {
    uint8_t C_r = (C & (1 << 7)) ? 1 : 0;
    uint8_t value = ((argument >> 1) | (C_r << 7));
    Z = value;
    C = argument;
    N = value;
    return value;
}

uint8_t CPU::ROL(uint8_t argument) {
    uint8_t C_r = (C & (1 << 7)) ? 1 : 0;
    uint8_t value = ((argument << 1) | C_r);
    Z = value;
    C = argument;
    N = value;
    return value;
}

void CPU::PLP() {
    uint8_t newStatus = ((pop8() & 0xEF) | 0x20);
    N = (newStatus & (1 << 7));
    V = (newStatus & (1 << 6));
    U = 1;
    B = (newStatus & (1 << 4)) ? 1 : 0;
    D = (newStatus & (1 << 3)) ? 1 : 0;
    I = (newStatus & (1 << 2)) ? 1 : 0;
    Z = (newStatus & (1 << 1)) ? 0 : 1; // inverse logic
    C = (newStatus & (1 << 0)) ? 1 << 7 : 0;
}

void CPU::PLA() {
    A = pop8();
    Z = A;
    N = A;
}

void CPU::PHP() {
    push8(flag_reg_value() | 0x10);
}

void CPU::PHA() {
    push8(A);
}

void CPU::ORA(uint8_t argument) {
    A = A | argument;
    Z = A;
    N = A;
}

void CPU::NOP() {
    // kek
}

uint8_t CPU::LSR(uint8_t argument) {
    uint8_t result = argument >> 1;
    Z = result;
    N = result;
    C = (argument & 1) ? 1 << 7 : 0;
    return result;
}

void CPU::LDY(uint8_t argument) {
    Y = argument;
    Z = Y;
    N = Y;
}

void CPU::LDX(uint8_t argument) {
    X = argument;
    Z = X;
    N = X;
}

void CPU::LDA(uint8_t argument) {
    A = argument;
    Z = A;
    N = A;
}

void CPU::JSR(uint16_t address) {
    push16(PC - 1);
    setPC(address);
}

void CPU::JMP(uint16_t address) {
    setPC(address);
}

void CPU::INY() {
    Y++;
    Z = Y;
    N = Y;
}

void CPU::INX() {
    X++;
    Z = X;
    N = X;
}

uint8_t CPU::INC(uint8_t argument) {
    argument++;
    Z = argument;
    N = argument;
    return argument;
}

void CPU::EOR(uint8_t argument) {
    A = A ^ argument;
    Z = A;
    N = A;
}

void CPU::DEY() {
    Y--;
    Z = Y;
    N = Y;
}

void CPU::DEX() {
    X--;
    Z = X;
    N = X;
}

uint8_t CPU::DEC(uint8_t argument) {
    argument--;
    Z = argument;
    N = argument;
    return argument;
}

void CPU::CMP(uint8_t reg, uint8_t argument) { // CMP, CPY, CPX
    uint16_t result = uint16_t(reg) - argument;
    if (result < 0x100) {
        C = 1 << 7;
    } else {
        C = 0;
    }
    N = result & 0xFF;
    Z = result & 0xFF;
}

void CPU::CLV() {
    V = 0;
}

void CPU::CLI() {
    I = 0;
}

void CPU::CLD() {
    D = 0;
}

void CPU::CLC() {
    C = 0;
}

void CPU::BVS(int8_t address) {
    if (V & (1 << 6)) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BVC(int8_t address) {
    if (!(V & (1 << 6))) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BRK() {
    if (I == 1 || B == 1) {
        return; // if we're in interrupt - can't go there twice
    }
    push16(PC);
    push8(flag_reg_value());
    uint16_t newPC = m->Read16(0xFFFE);
    setPC(newPC);
    B = 1;
}

void CPU::BPL(int8_t address) {
    if (!(N & (1 << 7))) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BNE(int8_t address) {
    if (Z != 0) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BMI(int8_t address) {
    if (N & (1 << 7)) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BIT(uint8_t argument) {
    Z = argument & A;
    V = argument;
    N = argument;
}

void CPU::BEQ(int8_t address) {
    if (Z == 0) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BCS(int8_t address) {
    if (C & (1 << 7)) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BCC(int8_t address) {
    if (!(C & (1 << 7))) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

uint8_t CPU::ASL(uint8_t argument) {
    uint8_t result = argument << 1;
    Z = result;
    N = result;
    C = argument;
    return result;
}

void CPU::AND(uint8_t argument) {
    A = A & argument;
    Z = A;
    N = A;
}

void CPU::ADC(uint8_t argument) {
    uint16_t sum = A + argument + C;
    C = (sum > 0xFF) ? 1 << 7 : 0;
    V = (!((A ^ argument) & (1 << 7)) && ((uint16_t(A) ^ sum) & (1 << 7))) ? 1 << 6 : 0;
    Z = (sum & 0xFF);
    N = (sum & 0xFF);
    A = uint8_t(sum & 0xFF);
}



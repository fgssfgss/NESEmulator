//
// Created by lin-k on 01.10.2016.
//

#include "../include/CPU.h"

typedef int (CPU::*Instruction)();
static Instruction instrTable[0xFF];

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

	instrTable[0x00] = &CPU::instrBRK0x00;
	instrTable[0x01] = &CPU::instrORA0x01;
	instrTable[0x05] = &CPU::instrORA0x05;
	instrTable[0x06] = &CPU::instrASL0x06;
	instrTable[0x08] = &CPU::instrPHP0x08;
	instrTable[0x09] = &CPU::instrORA0x09;
	instrTable[0x0a] = &CPU::instrASL0x0a;
	instrTable[0x0d] = &CPU::instrORA0x0d;
	instrTable[0x0e] = &CPU::instrASL0x0e;
	instrTable[0x10] = &CPU::instrBPL0x10;
	instrTable[0x11] = &CPU::instrORA0x11;
	instrTable[0x15] = &CPU::instrORA0x15;
	instrTable[0x16] = &CPU::instrASL0x16;
	instrTable[0x18] = &CPU::instrCLC0x18;
	instrTable[0x19] = &CPU::instrORA0x19;
	instrTable[0x1d] = &CPU::instrORA0x1d;
	instrTable[0x1e] = &CPU::instrASL0x1e;
	instrTable[0x20] = &CPU::instrJSR0x20;
	instrTable[0x21] = &CPU::instrAND0x21;
	instrTable[0x24] = &CPU::instrBIT0x24;
	instrTable[0x25] = &CPU::instrAND0x25;
	instrTable[0x26] = &CPU::instrROL0x26;
	instrTable[0x28] = &CPU::instrPLP0x28;
	instrTable[0x29] = &CPU::instrAND0x29;
	instrTable[0x2a] = &CPU::instrROL0x2a;
	instrTable[0x2c] = &CPU::instrBIT0x2c;
	instrTable[0x2d] = &CPU::instrAND0x2d;
	instrTable[0x2e] = &CPU::instrROL0x2e;
	instrTable[0x30] = &CPU::instrBMI0x30;
	instrTable[0x31] = &CPU::instrAND0x31;
	instrTable[0x35] = &CPU::instrAND0x35;
	instrTable[0x36] = &CPU::instrROL0x36;
	instrTable[0x38] = &CPU::instrSEC0x38;
	instrTable[0x39] = &CPU::instrAND0x39;
	instrTable[0x3d] = &CPU::instrAND0x3d;
	instrTable[0x3e] = &CPU::instrROL0x3e;
	instrTable[0x40] = &CPU::instrRTI0x40;
	instrTable[0x41] = &CPU::instrEOR0x41;
	instrTable[0x45] = &CPU::instrEOR0x45;
	instrTable[0x46] = &CPU::instrLSR0x46;
	instrTable[0x48] = &CPU::instrPHA0x48;
	instrTable[0x49] = &CPU::instrEOR0x49;
	instrTable[0x4a] = &CPU::instrLSR0x4a;
	instrTable[0x4c] = &CPU::instrJMP0x4c;
	instrTable[0x4d] = &CPU::instrEOR0x4d;
	instrTable[0x4e] = &CPU::instrLSR0x4e;
	instrTable[0x50] = &CPU::instrBVC0x50;
	instrTable[0x51] = &CPU::instrEOR0x51;
	instrTable[0x55] = &CPU::instrEOR0x55;
	instrTable[0x56] = &CPU::instrLSR0x56;
	instrTable[0x58] = &CPU::instrCLI0x58;
	instrTable[0x59] = &CPU::instrEOR0x59;
	instrTable[0x5d] = &CPU::instrEOR0x5d;
	instrTable[0x5e] = &CPU::instrLSR0x5e;
	instrTable[0x60] = &CPU::instrRTS0x60;
	instrTable[0x61] = &CPU::instrADC0x61;
	instrTable[0x65] = &CPU::instrADC0x65;
	instrTable[0x66] = &CPU::instrROR0x66;
	instrTable[0x68] = &CPU::instrPLA0x68;
	instrTable[0x69] = &CPU::instrADC0x69;
	instrTable[0x6a] = &CPU::instrROR0x6a;
	instrTable[0x6c] = &CPU::instrJMP0x6c;
	instrTable[0x6d] = &CPU::instrADC0x6d;
	instrTable[0x6e] = &CPU::instrROR0x6e;
	instrTable[0x70] = &CPU::instrBVS0x70;
	instrTable[0x71] = &CPU::instrADC0x71;
	instrTable[0x75] = &CPU::instrADC0x75;
	instrTable[0x76] = &CPU::instrROR0x76;
	instrTable[0x78] = &CPU::instrSEI0x78;
	instrTable[0x79] = &CPU::instrADC0x79;
	instrTable[0x7d] = &CPU::instrADC0x7d;
	instrTable[0x7e] = &CPU::instrROR0x7e;
	instrTable[0x81] = &CPU::instrSTA0x81;
	instrTable[0x84] = &CPU::instrSTY0x84;
	instrTable[0x85] = &CPU::instrSTA0x85;
	instrTable[0x86] = &CPU::instrSTX0x86;
	instrTable[0x88] = &CPU::instrDEY0x88;
	instrTable[0x8a] = &CPU::instrTXA0x8a;
	instrTable[0x8c] = &CPU::instrSTY0x8c;
	instrTable[0x8d] = &CPU::instrSTA0x8d;
	instrTable[0x8e] = &CPU::instrSTX0x8e;
	instrTable[0x90] = &CPU::instrBCC0x90;
	instrTable[0x91] = &CPU::instrSTA0x91;
	instrTable[0x94] = &CPU::instrSTY0x94;
	instrTable[0x95] = &CPU::instrSTA0x95;
	instrTable[0x96] = &CPU::instrSTX0x96;
	instrTable[0x98] = &CPU::instrTYA0x98;
	instrTable[0x99] = &CPU::instrSTA0x99;
	instrTable[0x9a] = &CPU::instrTXS0x9a;
	instrTable[0x9d] = &CPU::instrSTA0x9d;
	instrTable[0xa0] = &CPU::instrLDY0xa0;
	instrTable[0xa1] = &CPU::instrLDA0xa1;
	instrTable[0xa2] = &CPU::instrLDX0xa2;
	instrTable[0xa4] = &CPU::instrLDY0xa4;
	instrTable[0xa5] = &CPU::instrLDA0xa5;
	instrTable[0xa6] = &CPU::instrLDX0xa6;
	instrTable[0xa8] = &CPU::instrTAY0xa8;
	instrTable[0xa9] = &CPU::instrLDA0xa9;
	instrTable[0xaa] = &CPU::instrTAX0xaa;
	instrTable[0xac] = &CPU::instrLDY0xac;
	instrTable[0xad] = &CPU::instrLDA0xad;
	instrTable[0xae] = &CPU::instrLDX0xae;
	instrTable[0xb0] = &CPU::instrBCS0xb0;
	instrTable[0xb1] = &CPU::instrLDA0xb1;
	instrTable[0xb4] = &CPU::instrLDY0xb4;
	instrTable[0xb5] = &CPU::instrLDA0xb5;
	instrTable[0xb6] = &CPU::instrLDX0xb6;
	instrTable[0xb8] = &CPU::instrCLV0xb8;
	instrTable[0xb9] = &CPU::instrLDA0xb9;
	instrTable[0xba] = &CPU::instrTSX0xba;
	instrTable[0xbc] = &CPU::instrLDY0xbc;
	instrTable[0xbd] = &CPU::instrLDA0xbd;
	instrTable[0xbe] = &CPU::instrLDX0xbe;
	instrTable[0xc0] = &CPU::instrCPY0xc0;
	instrTable[0xc1] = &CPU::instrCMP0xc1;
	instrTable[0xc4] = &CPU::instrCPY0xc4;
	instrTable[0xc5] = &CPU::instrCMP0xc5;
	instrTable[0xc6] = &CPU::instrDEC0xc6;
	instrTable[0xc8] = &CPU::instrINY0xc8;
	instrTable[0xc9] = &CPU::instrCMP0xc9;
	instrTable[0xca] = &CPU::instrDEX0xca;
	instrTable[0xcc] = &CPU::instrCPY0xcc;
	instrTable[0xcd] = &CPU::instrCMP0xcd;
	instrTable[0xce] = &CPU::instrDEC0xce;
	instrTable[0xd0] = &CPU::instrBNE0xd0;
	instrTable[0xd1] = &CPU::instrCMP0xd1;
	instrTable[0xd5] = &CPU::instrCMP0xd5;
	instrTable[0xd6] = &CPU::instrDEC0xd6;
	instrTable[0xd8] = &CPU::instrCLD0xd8;
	instrTable[0xd9] = &CPU::instrCMP0xd9;
	instrTable[0xdd] = &CPU::instrCMP0xdd;
	instrTable[0xde] = &CPU::instrDEC0xde;
	instrTable[0xe0] = &CPU::instrCPX0xe0;
	instrTable[0xe1] = &CPU::instrSBC0xe1;
	instrTable[0xe4] = &CPU::instrCPX0xe4;
	instrTable[0xe5] = &CPU::instrSBC0xe5;
	instrTable[0xe6] = &CPU::instrINC0xe6;
	instrTable[0xe8] = &CPU::instrINX0xe8;
	instrTable[0xe9] = &CPU::instrSBC0xe9;
	instrTable[0xea] = &CPU::instrNOP0xea;
	instrTable[0xec] = &CPU::instrCPX0xec;
	instrTable[0xed] = &CPU::instrSBC0xed;
	instrTable[0xee] = &CPU::instrINC0xee;
	instrTable[0xf0] = &CPU::instrBEQ0xf0;
	instrTable[0xf1] = &CPU::instrSBC0xf1;
	instrTable[0xf5] = &CPU::instrSBC0xf5;
	instrTable[0xf6] = &CPU::instrINC0xf6;
	instrTable[0xf8] = &CPU::instrSED0xf8;
	instrTable[0xf9] = &CPU::instrSBC0xf9;
	instrTable[0xfd] = &CPU::instrSBC0xfd;
	instrTable[0xfe] = &CPU::instrINC0xfe;

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
    n |= (N << 7);
    n |= (V << 6);
    n |= (U << 5);
    n |= (B << 4);
    n |= (D << 3);
    n |= (I << 2);
    n |= (Z << 1);
    n |= (C << 0);
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

int CPU::instrBRK0x00() {
	BRK();
	return 7;
}

int CPU::instrORA0x01() {
	ORA(indexed_indirect_addr());
	return 6;
}

int CPU::instrORA0x05() {
	ORA(zeropage_addr());
	return 3;
}

int CPU::instrASL0x06() {
	uint16_t address = zeropage_addr_j();
	m->Write8(address, ASL(m->Read8(address)));
	return 5;
}

int CPU::instrPHP0x08() {
	PHP();
	return 3;
}

int CPU::instrORA0x09() {
	ORA(immediate_addr());
	return 2;
}

int CPU::instrASL0x0a() {
	A = ASL(A);
	return 2;
}

int CPU::instrORA0x0d() {
	ORA(absolute_addr());
	return 4;
}

int CPU::instrASL0x0e() {
	uint16_t address = absolute_addr_j();
	m->Write8(address, ASL(m->Read8(address)));
	return 6;
}

int CPU::instrBPL0x10() {
	BPL(relative_addr());
	return 2;
}

int CPU::instrORA0x11() {
	ORA(indirect_indexed_addr());
	return 5;
}

int CPU::instrORA0x15() {
	ORA(zeropage_x_addr());
	return 4;
}

int CPU::instrASL0x16() {
	uint16_t address = zeropage_x_addr_j();
	m->Write8(address, ASL(m->Read8(address)));
	return 6;
}

int CPU::instrCLC0x18() {
	CLC();
	return 2;
}

int CPU::instrORA0x19() {
	ORA(absolute_y_addr());
	return 4;
}

int CPU::instrORA0x1d() {
	ORA(absolute_x_addr());
	return 4;
}

int CPU::instrASL0x1e() {
	uint16_t address = absolute_x_addr_j();
	m->Write8(address, ASL(m->Read8(address)));
	return 7;
}

int CPU::instrJSR0x20() {
	JSR(absolute_addr_j());
	return 6;
}

int CPU::instrAND0x21() {
	AND(indexed_indirect_addr());
	return 6;
}

int CPU::instrBIT0x24() {
	BIT(zeropage_addr());
	return 3;
}

int CPU::instrAND0x25() {
	AND(zeropage_addr());
	return 3;
}

int CPU::instrROL0x26() {
	uint16_t address = zeropage_addr_j();
	m->Write8(address, ROL(m->Read8(address)));
	return 5;
}

int CPU::instrPLP0x28() {
	PLP();
	return 4;
}

int CPU::instrAND0x29() {
	AND(immediate_addr());
	return 2;
}

int CPU::instrROL0x2a() {
	A = ROL(A);
	return 2;
}

int CPU::instrBIT0x2c() {
	BIT(absolute_addr());
	return 4;
}

int CPU::instrAND0x2d() {
	AND(absolute_addr());
	return 4;
}

int CPU::instrROL0x2e() {
	uint16_t address = absolute_addr_j();
	m->Write8(address, ROL(m->Read8(address)));
	return 6;
}

int CPU::instrBMI0x30() {
	BMI(relative_addr());
	return 2;
}

int CPU::instrAND0x31() {
	AND(indirect_indexed_addr());
	return 5;
}

int CPU::instrAND0x35() {
	AND(zeropage_x_addr());
	return 4;
}

int CPU::instrROL0x36() {
	uint16_t address = zeropage_x_addr_j();
	m->Write8(address, ROL(m->Read8(address)));
	return 6;
}

int CPU::instrSEC0x38() {
	SEC();
	return 2;
}

int CPU::instrAND0x39() {
	AND(absolute_y_addr());
	return 4;
}

int CPU::instrAND0x3d() {
	AND(absolute_x_addr());
	return 4;
}

int CPU::instrROL0x3e() {
	uint16_t address = absolute_x_addr_j();
	m->Write8(address, ROL(m->Read8(address)));
	return 7;
}

int CPU::instrRTI0x40() {
	RTI();
	return 6;
}

int CPU::instrEOR0x41() {
	EOR(indexed_indirect_addr());
	return 6;
}

int CPU::instrEOR0x45() {
	EOR(zeropage_addr());
	return 3;
}

int CPU::instrLSR0x46() {
	uint16_t address = zeropage_addr_j();
	m->Write8(address, LSR(m->Read8(address)));
	return 5;
}

int CPU::instrPHA0x48() {
	PHA();
	return 3;
}

int CPU::instrEOR0x49() {
	EOR(immediate_addr());
	return 2;
}


int CPU::instrLSR0x4a() {
	A = LSR(A);
	return 2;
}

int CPU::instrJMP0x4c() {
	JMP(absolute_addr_j());
	return 3;
}

int CPU::instrEOR0x4d() {
	EOR(absolute_addr());
	return 4;
}

int CPU::instrLSR0x4e() {
	uint16_t address = absolute_addr_j();
	m->Write8(address, LSR(m->Read8(address)));
	return 6;
}

int CPU::instrBVC0x50() {
	BVC(relative_addr());
	return 2;
}

int CPU::instrEOR0x51() {
	EOR(indirect_indexed_addr());
	return 5;
}

int CPU::instrEOR0x55() {
	EOR(zeropage_x_addr());
	return 4;
}

int CPU::instrLSR0x56() {
	uint16_t address = zeropage_x_addr_j();
	m->Write8(address, LSR(m->Read8(address)));
	return 6;
}

int CPU::instrCLI0x58() {
	CLI();
	return 2;
}

int CPU::instrEOR0x59() {
	EOR(absolute_y_addr());
	return 4;
}

int CPU::instrEOR0x5d() {
	EOR(absolute_x_addr());
	return 4;
}

int CPU::instrLSR0x5e() {
	uint16_t address = absolute_x_addr_j();
	m->Write8(address, LSR(m->Read8(address)));
	return 7;
}

int CPU::instrRTS0x60() {
	RTS();
	return 6;
}

int CPU::instrADC0x61() {
	ADC(indexed_indirect_addr());
	return 6;
}

int CPU::instrADC0x65() {
	ADC(zeropage_addr());
	return 3;
}

int CPU::instrROR0x66() {
	uint16_t address = zeropage_addr_j();
	m->Write8(address, ROR(m->Read8(address)));
	return 5;
}

int CPU::instrPLA0x68() {
	PLA();
	return 4;
}

int CPU::instrADC0x69() {
	ADC(immediate_addr());
	return 2;
}

int CPU::instrROR0x6a() {
	A = ROR(A);
	return 2;
}

int CPU::instrJMP0x6c() {
	uint16_t old_addr = absolute_addr_j();
	uint16_t addr = m->Read16(old_addr);
	if ((old_addr & 0xFF) == 0xFF) // emulating bug
		addr = m->Read8(old_addr) | (m->Read8(old_addr & 0xFF00) << 8);
	JMP(addr);
	return 5;
}

int CPU::instrADC0x6d() {
	ADC(absolute_addr());
	return 4;
}

int CPU::instrROR0x6e() {
	uint16_t address = absolute_addr_j();
	m->Write8(address, ROR(m->Read8(address)));
	return 6;
}

int CPU::instrBVS0x70() {
	BVS(relative_addr());
	return 2;
}

int CPU::instrADC0x71() {
	ADC(indirect_indexed_addr());
	return 5;
}

int CPU::instrADC0x75() {
	ADC(zeropage_x_addr());
	return 4;
}

int CPU::instrROR0x76() {
	uint16_t address = zeropage_x_addr_j();
	m->Write8(address, ROR(m->Read8(address)));
	return 6;
}

int CPU::instrSEI0x78() {
	SEI();
	return 2;
}

int CPU::instrADC0x79() {
	ADC(absolute_y_addr());
	return 4;
}

int CPU::instrADC0x7d() {
	ADC(absolute_x_addr());
	return 4;
}

int CPU::instrROR0x7e() {
	uint16_t address = absolute_x_addr_j();
	m->Write8(address, ROR(m->Read8(address)));
	return 7;
}

int CPU::instrSTA0x81() {
	m->Write8(indexed_indirect_addr_j(), A);
	return 6;
}

int CPU::instrSTY0x84() {
	m->Write8(zeropage_addr_j(), Y);
	return 3;
}

int CPU::instrSTA0x85() {
	m->Write8(zeropage_addr_j(), A);
	return 3;
}

int CPU::instrSTX0x86() {
	m->Write8(zeropage_addr_j(), X);
	return 3;
}

int CPU::instrDEY0x88() {
	DEY();
	return 2;
}

int CPU::instrTXA0x8a() {
	TXA();
	return 2;
}

int CPU::instrSTY0x8c() {
	m->Write8(absolute_addr_j(), Y);
	return 4;
}

int CPU::instrSTA0x8d() {
	m->Write8(absolute_addr_j(), A);
	return 4;
}

int CPU::instrSTX0x8e() {
	m->Write8(absolute_addr_j(), X);
	return 4;
}

int CPU::instrBCC0x90() {
	BCC(relative_addr());
	return 2;
}

int CPU::instrSTA0x91() {
	m->Write8(indirect_indexed_addr_j(), A);
	return 6;
}

int CPU::instrSTY0x94() {
	m->Write8(zeropage_x_addr_j(), Y);
	return 4;
}

int CPU::instrSTA0x95() {
	m->Write8(zeropage_x_addr_j(), A);
	return 4;
}

int CPU::instrSTX0x96() {
	m->Write8(zeropage_y_addr_j(), X);
	return 4;
}

int CPU::instrTYA0x98() {
	TYA();
	return 2;
}

int CPU::instrSTA0x99() {
	m->Write8(absolute_y_addr_j(), A);
	return 5;
}

int CPU::instrTXS0x9a() {
	TXS();
	return 2;
}

int CPU::instrSTA0x9d() {
	m->Write8(absolute_x_addr_j(), A);
	return 5;
}

int CPU::instrLDY0xa0() {
	LDY(immediate_addr());
	return 2;
}

int CPU::instrLDA0xa1() {
	LDA(indexed_indirect_addr());
	return 6;
}

int CPU::instrLDX0xa2() {
	LDX(immediate_addr());
	return 2;
}

int CPU::instrLDY0xa4() {
	LDY(zeropage_addr());
	return 3;
}

int CPU::instrLDA0xa5() {
	LDA(zeropage_addr());
	return 3;
}

int CPU::instrLDX0xa6() {
	LDX(zeropage_addr());
	return 3;
}

int CPU::instrTAY0xa8() {
	TAY();
	return 2;
}

int CPU::instrLDA0xa9() {
	LDA(immediate_addr());
	return 2;
}

int CPU::instrTAX0xaa() {
	TAX();
	return 2;
}

int CPU::instrLDY0xac() {
	LDY(absolute_addr());
	return 4;
}

int CPU::instrLDA0xad() {
	LDA(absolute_addr());
	return 4;
}

int CPU::instrLDX0xae() {
	LDX(absolute_addr());
	return 4;
}

int CPU::instrBCS0xb0() {
	BCS(relative_addr());
	return 2;
}

int CPU::instrLDA0xb1() {
	LDA(indirect_indexed_addr());
	return 5;
}

int CPU::instrLDY0xb4() {
	LDY(zeropage_x_addr());
	return 4;
}

int CPU::instrLDA0xb5() {
	LDA(zeropage_x_addr());
	return 4;
}

int CPU::instrLDX0xb6() {
	LDX(zeropage_y_addr());
	return 4;
}

int CPU::instrCLV0xb8() {
	CLV();
	return 2;
}

int CPU::instrLDA0xb9() {
	LDA(absolute_y_addr());
	return 4;
}

int CPU::instrTSX0xba() {
	TSX();
	return 2;
}

int CPU::instrLDY0xbc() {
	LDY(absolute_x_addr());
	return 4;
}

int CPU::instrLDA0xbd() {
	LDA(absolute_x_addr());
	return 4;
}

int CPU::instrLDX0xbe() {
	LDX(absolute_y_addr());
	return 4;
}

int CPU::instrCPY0xc0() {
	CMP(Y, immediate_addr());
	return 2;
}

int CPU::instrCMP0xc1() {
	CMP(A, indexed_indirect_addr());
	return 6;
}

int CPU::instrCPY0xc4() {
	CMP(Y, zeropage_addr());
	return 3;
}

int CPU::instrCMP0xc5() {
	CMP(A, zeropage_addr());
	return 3;
}

int CPU::instrDEC0xc6() {
	uint16_t address = zeropage_addr_j();
	m->Write8(address, DEC(m->Read8(address)));
	return 5;
}

int CPU::instrINY0xc8() {
	INY();
	return 2;
}

int CPU::instrCMP0xc9() {
	CMP(A, immediate_addr());
	return 2;
}

int CPU::instrDEX0xca() {
	DEX();
	return 2;
}

int CPU::instrCPY0xcc() {
	CMP(Y, absolute_addr());
	return 4;
}

int CPU::instrCMP0xcd() {
	CMP(A, absolute_addr());
	return 4;
}

int CPU::instrDEC0xce() {
	uint16_t address = absolute_addr_j();
	m->Write8(address, DEC(m->Read8(address)));
	return 6;
}

int CPU::instrBNE0xd0() {
	BNE(relative_addr());
	return 2;
}

int CPU::instrCMP0xd1() {
	CMP(A, indirect_indexed_addr());
	return 5;
}

int CPU::instrCMP0xd5() {
	CMP(A, zeropage_x_addr());
	return 4;
}

int CPU::instrDEC0xd6() {
	uint16_t address = zeropage_x_addr_j();
	m->Write8(address, DEC(m->Read8(address)));
	return 6;
}

int CPU::instrCLD0xd8() {
	CLD();
	return 2;
}

int CPU::instrCMP0xd9() {
	CMP(A, absolute_y_addr());
	return 4;
}

int CPU::instrCMP0xdd() {
	CMP(A, absolute_x_addr());
	return 4;
}

int CPU::instrDEC0xde() {
	uint16_t address = absolute_x_addr_j();
	m->Write8(address, DEC(m->Read8(address)));
	return 7;
}

int CPU::instrCPX0xe0() {
	CMP(X, immediate_addr());
	return 2;
}

int CPU::instrSBC0xe1() {
	SBC(indexed_indirect_addr());
	return 6;
}

int CPU::instrCPX0xe4() {
	CMP(X, zeropage_addr());
	return 3;
}

int CPU::instrSBC0xe5() {
	SBC(zeropage_addr());
	return 3;
}

int CPU::instrINC0xe6() {
	uint16_t address = zeropage_addr_j();
	m->Write8(address, INC(m->Read8(address)));
	return 5;
}

int CPU::instrINX0xe8() {
	INX();
	return 2;
}

int CPU::instrSBC0xe9() {
	SBC(immediate_addr());
	return 2;
}

int CPU::instrNOP0xea() {
	NOP();
	return 2;
}

int CPU::instrCPX0xec() {
	CMP(X, absolute_addr());
	return 4;
}

int CPU::instrSBC0xed() {
	SBC(absolute_addr());
	return 4;
}

int CPU::instrINC0xee() {
	uint16_t address = absolute_addr_j();
	m->Write8(address, INC(m->Read8(address)));
	return 6;
}

int CPU::instrBEQ0xf0() {
	BEQ(relative_addr());
	return 2;
}

int CPU::instrSBC0xf1() {
	SBC(indirect_indexed_addr());
	return 5;
}

int CPU::instrSBC0xf5() {
	SBC(zeropage_x_addr());
	return 4;
}

int CPU::instrINC0xf6() {
	uint16_t address = zeropage_x_addr_j();
	m->Write8(address, INC(m->Read8(address)));
	return 6;
}

int CPU::instrSED0xf8() {
	SED();
	return 2;
}

int CPU::instrSBC0xf9() {
	SBC(absolute_y_addr());
	return 4;
}

int CPU::instrSBC0xfd() {
	SBC(absolute_x_addr());
	return 4;
}

int CPU::instrINC0xfe() {
	uint16_t address = absolute_x_addr_j();
	m->Write8(address, INC(m->Read8(address)));
	return 7;
}


int CPU::execute() {
    uint8_t instr = m->Read8(PC);
    PC++;

    if (instrTable[instr] != nullptr) {
	    cycles = (this->*instrTable[instr])();
    } else {
    	cycles = 2;
    }

    return cycles;
}

void CPU::TYA() {
    A = Y;
    if (A == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (A & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::TXS() {
    S = X;
}

void CPU::TXA() {
    A = X;
    if (A == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (A & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::TSX() {
    X = S;
    if (X == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (X & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::TAY() {
    Y = A;
    if (Y == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (Y & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::TAX() {
    X = A;
    if (X == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (X & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
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
    uint16_t oldPC = pop16();
    setPC(oldPC);
}

uint8_t CPU::ROR(uint8_t argument) {
    uint8_t value = ((argument >> 1) | (C << 7));
    Z = (value == 0) ? 1 : 0;
    C = (argument & 1) ? 1 : 0;
    N = (value & (1 << 7)) ? 1 : 0;
    return value;
}

uint8_t CPU::ROL(uint8_t argument) {
    uint8_t value = ((argument << 1) | C);
    Z = (value == 0) ? 1 : 0;
    C = (argument & (1 << 7)) ? 1 : 0;
    N = (value & (1 << 7)) ? 1 : 0;
    return value;
}

void CPU::PLP() {
    uint8_t newStatus = ((pop8() & 0xEF) | 0x20);
    N = (newStatus & (1 << 7)) ? 1 : 0;
    V = (newStatus & (1 << 6)) ? 1 : 0;
    U = 1;
    B = (newStatus & (1 << 4)) ? 1 : 0;
    D = (newStatus & (1 << 3)) ? 1 : 0;
    I = (newStatus & (1 << 2)) ? 1 : 0;
    Z = (newStatus & (1 << 1)) ? 1 : 0;
    C = (newStatus & (1 << 0)) ? 1 : 0;
}

void CPU::PLA() {
    A = pop8();
    if (A == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (A & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::PHP() {
    push8(flag_reg_value() | 0x10);
}

void CPU::PHA() {
    push8(A);
}

void CPU::ORA(uint8_t argument) {
    A = A | argument;
    if (A == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (A & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::NOP() {
    // kek
}

uint8_t CPU::LSR(uint8_t argument) {
    uint8_t result = argument >> 1;
    if (result == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    N = (result & (1 << 7)) ? 1 : 0;
    C = (argument & 1) ? 1 : 0;
    return result;
}

void CPU::LDY(uint8_t argument) {
    Y = argument;
    if (Y == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (Y & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::LDX(uint8_t argument) {
    X = argument;
    if (X == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (X & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::LDA(uint8_t argument) {
    A = argument;
    if (A == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (A & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
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
    if (Y == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (Y & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::INX() {
    X++;
    if (X == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (X & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

uint8_t CPU::INC(uint8_t argument) {
    argument++;
    if (argument == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (argument & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
    return argument;
}

void CPU::EOR(uint8_t argument) {
    A = A ^ argument;
    if (A == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (A & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::DEY() {
    Y--;
    if (Y == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (Y & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::DEX() {
    X--;
    if (X == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (X & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

uint8_t CPU::DEC(uint8_t argument) {
    argument--;
    if (argument == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (argument & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
    return argument;
}

void CPU::CMP(uint8_t reg, uint8_t argument) { // CMP, CPY, CPX
    uint16_t result = uint16_t(reg) - argument;
    if (result < 0x100) {
        C = 1;
    } else {
        C = 0;
    }
    if (result & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
    if (reg == argument) {
        Z = 1;
    } else {
        Z = 0;
    }
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
    if (V == 1) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BVC(int8_t address) {
    if (V == 0) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BRK() {
    if (I == 1) {
        return; // if we're in interrupt - can't go there twice
    }
    push16(PC);
    push8(flag_reg_value());
    uint16_t newPC = m->Read16(0xFFFE);
    setPC(newPC);
    B = 1;
}

void CPU::BPL(int8_t address) {
    if (N == 0) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BNE(int8_t address) {
    if (Z == 0) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BMI(int8_t address) {
    if (N == 1) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BIT(uint8_t argument) {
    if ((argument & A) == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    V = (argument & (1 << 6)) ? 1 : 0;
    N = (argument & (1 << 7)) ? 1 : 0;
}

void CPU::BEQ(int8_t address) {
    if (Z == 1) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BCS(int8_t address) {
    if (C == 1) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

void CPU::BCC(int8_t address) {
    if (C == 0) {
        cycles++;
        PC = (int16_t) PC + address; // TODO: check crosspage
    }
}

uint8_t CPU::ASL(uint8_t argument) {
    uint8_t result = argument << 1;
    if (result == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    N = (result & (1 << 7)) ? 1 : 0;
    C = (argument & (1 << 7)) ? 1 : 0;
    return result;
}

void CPU::AND(uint8_t argument) {
    A = A & argument;
    if (A == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
    if (A & (1 << 7)) {
        N = 1;
    } else {
        N = 0;
    }
}

void CPU::ADC(uint8_t argument) {
    uint16_t sum = A + argument + C;
    C = (sum > 0xFF) ? 1 : 0;
    V = (!((A ^ argument) & (1 << 7)) && ((uint16_t(A) ^ sum) & (1 << 7))) ? 1 : 0;
    Z = ((sum & 0xFF) == 0) ? 1 : 0;
    N = (sum & (1 << 7)) ? 1 : 0;
    A = uint8_t(sum & 0xFF);
}


//
// Created by lin-k on 01.10.2016.
//

#ifndef NESEMULATOR_CPU_H
#define NESEMULATOR_CPU_H

#include "Common.h"
#include "Console.h"

class Memory;

class CPU {
public:
    CPU();

    virtual ~CPU();

    int execute();

    void init();

    void setPC(uint16_t value);

    void NMI();

    void IRQ();

    long getTotalCycles() { return total_cycles; }

protected:
private:
    Memory *m;

	// actual instructions for instr table
	int instrBRK0x00();
	int instrORA0x01();
	int instrORA0x05();
	int instrASL0x06();
	int instrPHP0x08();
	int instrORA0x09();
	int instrASL0x0a();
	int instrORA0x0d();
	int instrASL0x0e();
	int instrBPL0x10();
	int instrORA0x11();
	int instrORA0x15();
	int instrASL0x16();
	int instrCLC0x18();
	int instrORA0x19();
	int instrORA0x1d();
	int instrASL0x1e();
	int instrJSR0x20();
	int instrAND0x21();
	int instrBIT0x24();
	int instrAND0x25();
	int instrROL0x26();
	int instrPLP0x28();
	int instrAND0x29();
	int instrROL0x2a();
	int instrBIT0x2c();
	int instrAND0x2d();
	int instrROL0x2e();
	int instrBMI0x30();
	int instrAND0x31();
	int instrAND0x35();
	int instrROL0x36();
	int instrSEC0x38();
	int instrAND0x39();
	int instrAND0x3d();
	int instrROL0x3e();
	int instrRTI0x40();
	int instrEOR0x41();
	int instrEOR0x45();
	int instrLSR0x46();
	int instrPHA0x48();
	int instrEOR0x49();
	int instrLSR0x4a();
	int instrJMP0x4c();
	int instrEOR0x4d();
	int instrLSR0x4e();
	int instrBVC0x50();
	int instrEOR0x51();
	int instrEOR0x55();
	int instrLSR0x56();
	int instrCLI0x58();
	int instrEOR0x59();
	int instrEOR0x5d();
	int instrLSR0x5e();
	int instrRTS0x60();
	int instrADC0x61();
	int instrADC0x65();
	int instrROR0x66();
	int instrPLA0x68();
	int instrADC0x69();
	int instrROR0x6a();
	int instrJMP0x6c();
	int instrADC0x6d();
	int instrROR0x6e();
	int instrBVS0x70();
	int instrADC0x71();
	int instrADC0x75();
	int instrROR0x76();
	int instrSEI0x78();
	int instrADC0x79();
	int instrADC0x7d();
	int instrROR0x7e();
	int instrSTA0x81();
	int instrSTY0x84();
	int instrSTA0x85();
	int instrSTX0x86();
	int instrDEY0x88();
	int instrTXA0x8a();
	int instrSTY0x8c();
	int instrSTA0x8d();
	int instrSTX0x8e();
	int instrBCC0x90();
	int instrSTA0x91();
	int instrSTY0x94();
	int instrSTA0x95();
	int instrSTX0x96();
	int instrTYA0x98();
	int instrSTA0x99();
	int instrTXS0x9a();
	int instrSTA0x9d();
	int instrLDY0xa0();
	int instrLDA0xa1();
	int instrLDX0xa2();
	int instrLDY0xa4();
	int instrLDA0xa5();
	int instrLDX0xa6();
	int instrTAY0xa8();
	int instrLDA0xa9();
	int instrTAX0xaa();
	int instrLDY0xac();
	int instrLDA0xad();
	int instrLDX0xae();
	int instrBCS0xb0();
	int instrLDA0xb1();
	int instrLDY0xb4();
	int instrLDA0xb5();
	int instrLDX0xb6();
	int instrCLV0xb8();
	int instrLDA0xb9();
	int instrTSX0xba();
	int instrLDY0xbc();
	int instrLDA0xbd();
	int instrLDX0xbe();
	int instrCPY0xc0();
	int instrCMP0xc1();
	int instrCPY0xc4();
	int instrCMP0xc5();
	int instrDEC0xc6();
	int instrINY0xc8();
	int instrCMP0xc9();
	int instrDEX0xca();
	int instrCPY0xcc();
	int instrCMP0xcd();
	int instrDEC0xce();
	int instrBNE0xd0();
	int instrCMP0xd1();
	int instrCMP0xd5();
	int instrDEC0xd6();
	int instrCLD0xd8();
	int instrCMP0xd9();
	int instrCMP0xdd();
	int instrDEC0xde();
	int instrCPX0xe0();
	int instrSBC0xe1();
	int instrCPX0xe4();
	int instrSBC0xe5();
	int instrINC0xe6();
	int instrINX0xe8();
	int instrSBC0xe9();
	int instrNOP0xea();
	int instrCPX0xec();
	int instrSBC0xed();
	int instrINC0xee();
	int instrBEQ0xf0();
	int instrSBC0xf1();
	int instrSBC0xf5();
	int instrINC0xf6();
	int instrSED0xf8();
	int instrSBC0xf9();
	int instrSBC0xfd();
	int instrINC0xfe();

	// addressing helpers
    uint8_t immediate_addr();

    int8_t relative_addr();

    uint8_t zeropage_addr();

    uint16_t zeropage_addr_j();

    uint16_t absolute_addr_j();

    uint8_t absolute_addr();

    uint8_t zeropage_x_addr();

    uint16_t zeropage_x_addr_j();

    uint8_t zeropage_y_addr();

    uint16_t zeropage_y_addr_j();

    uint16_t absolute_x_addr_j();

    uint8_t absolute_x_addr();

    uint16_t absolute_y_addr_j();

    uint8_t absolute_y_addr();

    uint8_t indexed_indirect_addr();

    uint8_t indirect_indexed_addr();

    uint16_t indexed_indirect_addr_j();

    uint16_t indirect_indexed_addr_j();

    // stack and flag helpers
    uint8_t pop8();

    uint16_t pop16();

    void push8(uint8_t data);

    void push16(uint16_t data);

    uint8_t flag_reg_value();

    // commands
    void ADC(uint8_t argument);

    void AND(uint8_t argument);

    uint8_t ASL(uint8_t argument);

    void BCC(int8_t address);

    void BCS(int8_t address);

    void BEQ(int8_t address);

    void BIT(uint8_t argument);

    void BMI(int8_t address);

    void BNE(int8_t address);

    void BPL(int8_t address);

    void BRK();

    void BVC(int8_t address);

    void BVS(int8_t address);

    void CLC();

    void CLD();

    void CLI();

    void CLV();

    void CMP(uint8_t reg, uint8_t argument); // use for CPX and CPY too

    uint8_t DEC(uint8_t argument);

    void DEX();

    void DEY();

    void EOR(uint8_t argument);

    uint8_t INC(uint8_t argument);

    void INX();

    void INY();

    void JMP(uint16_t address);

    void JSR(uint16_t address);

    void LDA(uint8_t argument);

    void LDX(uint8_t argument);

    void LDY(uint8_t argument);

    uint8_t LSR(uint8_t argument);

    void NOP();

    void ORA(uint8_t argument);

    void PHA();

    void PHP();

    void PLA();

    void PLP();

    uint8_t ROL(uint8_t argument);

    uint8_t ROR(uint8_t argument);

    void RTI();

    void RTS();

    void SBC(uint8_t argument);

    void SEC();

    void SED();

    void SEI();

    void TAX();

    void TAY();

    void TSX();

    void TXA();

    void TXS();

    void TYA();

    int cycles = 0;
    long hz = 0;
    long total_cycles = 0;

    // registers
    uint16_t PC;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t S;

    // flags, can be only 1 or 0
    uint8_t N:1;
    uint8_t V:1;
    uint8_t U:1;
    uint8_t B:1;
    uint8_t D:1;
    uint8_t I:1;
    uint8_t Z:1;
    uint8_t C:1;
};


#endif //NESEMULATOR_CPU_H

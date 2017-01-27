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

protected:
private:
    Memory *m;

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

    // registers
    uint16_t PC;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t S;

    // flags, can be only 1 or 0
    uint8_t N;
    uint8_t V;
    uint8_t U;
    uint8_t B;
    uint8_t D;
    uint8_t I;
    uint8_t Z;
    uint8_t C;
};


#endif //NESEMULATOR_CPU_H

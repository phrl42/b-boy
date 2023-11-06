#pragma once
#include "Sandbox.h"
#include "gbc/bus.h"

namespace GBC
{

  // toggles higher_half, lower_half and 16-bit mode in opfunctions
  enum IMode
  {
    NONE=0x00, LOW, HIGH, ALL, MEM, N8, N16, A8, A16, E8
  };

  struct Instruction
  {
    std::string mnemonic;

    uint8_t cycle;

    uint8_t (*opfun)(uint16_t*, IMode, uint16_t*, IMode);
    IMode w;
    uint16_t *dest;

    IMode r;
    uint16_t *src;
  };

  
  struct CPU
  {
    uint16_t AF = 0; // Accumulator and Flags

    // switchable registers
    uint16_t BC = 0;
    uint16_t DE = 0;
    uint16_t HL = 0;

    uint16_t SP = 0; // Stack Pointer
    uint16_t PC = 0; // Program Counter
 
    void Validate_Opcode(Bus *bus);

    Instruction instructions[1] = 
    {{"NOP", 8, &GBC::CPU::nop, IMode::NONE, nullptr, IMode::NONE, nullptr}};
  private:
    void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val);
    uint16_t Get_Bit_N(uint16_t src, uint8_t n);
  
    void Set_Half_Carry(uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Plus(uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Minus(uint16_t src_register, uint16_t val);
  
    static inline uint8_t nop(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r){}

    static uint8_t LD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t INC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
  
    static uint8_t DEC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t ADD(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t SUB(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t AND(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t OR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    // keep exception at 0xAF in mind
    static uint8_t XOR(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    // keep exception at 0x9F in mind
    static uint8_t SBC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    static uint8_t ADC(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);

    // keep exception at 0xBF in mind
    static uint8_t CP(uint16_t *dest_register, IMode w, uint16_t *src_value, IMode r);
  };

};

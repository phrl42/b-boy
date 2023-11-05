#pragma once
#include "Sandbox.h"
#include "gbc/bus.h"

namespace GBC
{
  struct Instruction
  {
    std::string mnemonic;

    uint8_t cycle;

    uint8_t(*opfun)(uint16_t*, uint16_t, bool);

    uint16_t dest;
    uint16_t src;
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

    inline Instruction instructions[256] = {
      {"nop", 8, &nop, },
      {},
    };
  private:
    void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val);
    uint16_t Get_Bit_N(uint16_t src, uint8_t n);
  
    void Set_Half_Carry( uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Plus( uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Minus( uint16_t src_register, uint16_t val);
  
    // Exceptions are in the switch statement. Such as: 0x08 (LD [a16], SP)
    // Relative Jumps are also there
    // LoaD 8-Bit value from RAM into Register
    uint8_t LD8(uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // LoaD 16-Bit value from 
    uint8_t LD16(uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // Increment 8-Bit Register
    uint8_t INC8( uint16_t *dest_register, uint16_t src_value, bool higher_half);
  
    // Increment 16-Bit Register
    uint8_t INC16(uint16_t *dest_register, uint16_t src_value, bool higher_half);

    inline void nop(){}
  
    // Decrement 8-Bit Register
    uint8_t DEC8(uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // Decrement 16-Bit Register
    uint8_t DEC16(uint16_t *dest_register, uint16_t src_value, bool higher_half);

  
    // ADD 8-Bit value to Register
    uint8_t ADD8( uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // ADD 16-Bit value to Register
    uint8_t ADD16( uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // SUB 8-Bit value from Register
    uint8_t SUB8( uint16_t *dest_register, uint16_t src_value, bool higher_half);
  
    // These ones only modify Register 'A' so I could remove the general part
    // Bitwise AND Register with value
    uint8_t AND8( uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // Bitwise OR on Register with value
    uint8_t OR8( uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // Bitwise XOR on Register with value
    // keep exception at 0xAF in mind
    uint8_t XOR8( uint16_t *dest_register, uint16_t src_value, bool higher_half);

  
    // Custom operation on Register with 8-Bit value
    // keep exception at 0x9F in mind
    uint8_t SBC8( uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // Custom operation on Register with 8-Bit value
    uint8_t ADC8( uint16_t *dest_register, uint16_t src_value, bool higher_half);

    // Custom operation on Register with 8-Bit value
    // keep exception at 0xBF in mind
    uint8_t CP8( uint16_t *dest_register, uint16_t src_value, bool higher_half);
  };

};

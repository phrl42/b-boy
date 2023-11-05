#pragma once
#include "Sandbox.h"
#include "gbc/bus.h"

namespace GBC
{

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

  private:
    void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val);
    uint16_t Get_Bit_N(uint16_t src, uint8_t n);
  
    void Set_Half_Carry(uint16_t *flags_register, uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Plus(uint16_t *flags_register, uint16_t src_register, uint16_t val, bool bit8);
    void Set_Carry_Minus(uint16_t *flags_register, uint16_t src_register, uint16_t val);
  
    // Exceptions are in the switch statement. Such as: 0x08 (LD [a16], SP)
    // Relative Jumps are also there
    // LoaD 8-Bit value from RAM into Register
    void LD8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

    // LoaD 16-Bit value from 
    void LD16(uint16_t *dest_register, uint16_t src_value);

    // Increment 8-Bit Register
    void INC8(uint16_t *flags_register, uint16_t *dest_register, bool higher_half);
  
    // Increment 16-Bit Register
    void INC16(uint16_t *dest_register);

  
    // Decrement 8-Bit Register
    void DEC8(uint16_t *flags_register, uint16_t *dest_register, bool higher_half);

    // Decrement 16-Bit Register
    void DEC16(uint16_t *dest_register);

  
    // ADD 8-Bit value to Register
    void ADD8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half);

    // ADD 16-Bit value to Register
    void ADD16(uint16_t *flags_register, uint16_t *dest_register, uint16_t src_value);

    // SUB 8-Bit value from Register
    void SUB8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half);

  
    // These ones only modify Register 'A' so I could remove the general part
    // Bitwise AND Register with value
    void AND8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half);

    // Bitwise OR on Register with value
    void OR8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half);

    // Bitwise XOR on Register with value
    // keep exception at 0xAF in mind
    void XOR8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half);

  
    // Custom operation on Register with 8-Bit value
    // keep exception at 0x9F in mind
    void SBC8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half);

    // Custom operation on Register with 8-Bit value
    void ADC8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half);

    // Custom operation on Register with 8-Bit value
    // keep exception at 0xBF in mind
    void CP8(uint16_t *flags_register, uint16_t *dest_register, uint8_t src_value, bool higher_half);
  };
  
  static inline const uint8_t opcycle[256] =
    {
      4,
      12,
      8,
      8,
      4,
      4,
      8,
      4,
      20,
      8,
      8,
      8,
      4,
      4,
      8,
      4,
      4,
      12,
      8,
      8,
      4,
      4,
      8,
      4,
      12,
      8,
      8,
      8,
      4,
      4,
      8,
      4,
      12,
      12,
      8,
      8,
      4,
      4,
      8,
      4,
      12,
      8,
      8,
      8,
      4,
      4,
      8,
      4,
      12,
      12,
      8,
      8,
      12,
      12,
      12,
      4,
      12,
      8,
      8,
      8,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      8,
      8,
      8,
      8,
      8,
      8,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      4,
      4,
      4,
      4,
      4,
      4,
      8,
      4,
      20,
      12,
      16,
      16,
      24,
      16,
      8,
      16,
      20,
      16,
      16,
      4,
      24,
      24,
      8,
      16,
      20,
      12,
      16,
      4,
      24,
      16,
      8,
      16,
      20,
      16,
      16,
      4,
      24,
      4,
      8,
      16,
      12,
      12,
      8,
      4,
      4,
      16,
      8,
      16,
      16,
      4,
      16,
      4,
      4,
      4,
      8,
      16,
      12,
      12,
      8,
      4,
      4,
      16,
      8,
      16,
      12,
      8,
      16,
      4,
      4,
      4,
      8,
      16
    }; 
};

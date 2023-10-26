#pragma once
#include "Sandbox.h"

namespace GBC
{
  // Exceptions are in the switch statement. Such as: 0x08 (LD [a16], SP)
  // Relative Jumps are also there

  // LoaD 8-Bit value from RAM into Register
  void LD8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

  // LoaD 16-Bit value from 
  void LD16(uint16_t *dest_register, uint16_t src_value);


  // Increment 8-Bit Register
  void INC8(uint16_t *dest_register, bool higher_half);
  
  // Increment 16-Bit Register
  void INC16(uint16_t *dest_register);

  
  // Decrement 8-Bit Register
  void DEC8(uint16_t *dest_register, bool higher_half);

  // Decrement 16-Bit Register
  void DEC16(uint16_t *dest_register);

  
  // ADD 8-Bit value to Register
  void ADD8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

  // ADD 16-Bit value to Register
  void ADD16(uint16_t *dest_register, uint16_t src_value);

  // SUB 8-Bit value from Register
  void SUB8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

  
  // These ones only modify Register 'A' so I could remove the general part
  // Bitwise AND Register with value
  void AND8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

  // Bitwise OR on Register with value
  void OR8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

  // Bitwise XOR on Register with value
  // keep exception at 0xAF in mind
  void XOR8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

  
  // Custom operation on Register with 8-Bit value
  // keep exception at 0x9F in mind
  void SBC8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

  // Custom operation on Register with 8-Bit value
  void ADC8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

  // Custom operation on Register with 8-Bit value
  // keep exception at 0xBF in mind
  void CP8(uint16_t *dest_register, uint8_t src_value, bool higher_half);

};

#pragma once
#include "Sandbox.h"
#include "gbc/bus.h"

namespace GBC
{

  struct CPU
  {
    // Registers
    uint8_t A = 0;
    uint8_t F = 0; // Flags
    
    uint8_t B = 0;
    uint8_t C = 0;

    uint8_t D = 0;
    uint8_t E = 0;

    uint8_t H = 0;
    uint8_t L = 0;
    
    uint16_t SP = 0; // Stack Pointer
    uint16_t PC = 0; // Program Counter
 
    void Validate_Opcode(Bus *bus);
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

#pragma once
#include "Sandbox.h"
#include "gbc/ppu.h"

// 64 KiB address space
#define GBC_RAM_SIZE 32768 * 2
// 16 KiB for ROM Bank 00
// 16 KiB for ROM Bank 01 - NN (switchable)
// 8 KiB for VRAM
// 8 KiB for External RAM
// 4 KiB for WRAM (Work RAM) 
// 4 KiB for WRAM (switchable 0-7)

namespace GBC
{
  const uint16_t entry = 0;
  enum State
  {
    STOP, RUN, HALT
  };
  
  struct Spec
  {
    State state;               // Declare current gbc state

    std::array<uint8_t, GBC_RAM_SIZE> ram = {0}; // Emulate original gbc ram size

    // Registers
    uint16_t AF = 0;               // Accumulator Flags
    uint16_t BC = 0;               // BC
    uint16_t DE = 0;               // DE
    uint16_t HL = 0;               // HL
    uint16_t SP = 0;               // Stack Pointer
    uint16_t PC = 0;               // Program Counter

    const char *rom;               // Running rom

    GPU gpu;                       // PPU Area
  };

  bool Load_Rom(Spec *spec, const char* rom_path);

  void Init_Spec(Spec *spec, const char* rom_path);

  void Validate_Opcode(Spec *spec);
  void Update(Spec *spec);
  
};

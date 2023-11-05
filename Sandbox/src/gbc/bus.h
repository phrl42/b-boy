#pragma once
#include "Sandbox.h"

// 64 KiB address space
#define GBC_RAM_SIZE 32768 * 2
// 16 KiB for ROM Bank 00
// 16 KiB for ROM Bank 01 - NN (switchable)
// 8 KiB for VRAM
// 8 KiB for External RAM
// 4 KiB for WRAM (Work RAM) 
// 4 KiB for WRAM (switchable 0-7)

#define NORMAL_SPEED 4.2
#define DOUBLE_SPEED 8.4

namespace GBC
{
  struct Bus
  {
    uint8_t Read(uint16_t address);
    uint8_t Write(uint16_t address, uint8_t value);

    void Emulate_Cycle(uint8_t n, bool normal);

    // needed for accurate timing
    double begin;
    double end;
  private:
    uint8_t space[GBC_RAM_SIZE] = {0}; // Emulate original gbc ram size
  };
  
};

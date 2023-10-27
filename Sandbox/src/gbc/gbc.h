#pragma once
#include "Sandbox.h"
#include <stack>

#define GBC_LOG(x) std::cout << "[GameBoy] " << x << std::endl;

// this is haunting me in my dreams
#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15

#define TFT_WIDTH 160
#define TFT_HEIGHT 144

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

    uint8_t ram[GBC_RAM_SIZE]; // Emulate original gbc ram size

    // Registers
    uint16_t AF;               // Accumulator Flags
    uint16_t BC;               // BC
    uint16_t DE;               // DE
    uint16_t HL;               // HL
    uint16_t SP;               // Stack Pointer
    uint16_t PC;               // Program Counter

    const char *rom;           // Running rom

    // 15-Bit RGB Color
    uint16_t display[TFT_HEIGHT][TFT_WIDTH];
  };

  //void Load_Font(Spec *spec);
  //uint16_t Character_Address(uint8_t ch);

  bool Load_Rom(Spec *spec, const char* rom_path);

  uint16_t Combine(uint8_t first, uint8_t second);
  uint16_t Get_Value_N(uint16_t opcode, uint8_t n);
  
  void Init_Spec(Spec *spec, const char* rom_path);

  void Validate_Opcode(Spec *spec);
  void Update(Spec *spec);
  
};

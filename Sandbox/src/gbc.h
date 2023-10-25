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

// 32 KiB
#define GBC_RAM_SIZE 32768

namespace GBC
{
  enum State
  {
    RUN = 0, PAUSE, QUIT
  };
  
  struct Spec
  {
    State state;              // Declare current gbc state

    uint8_t ram[GBC_RAM_SIZE]; // Emulate original gbc ram size

    uint16_t I;               // Index register (used for sprites)
    uint16_t PC;              // Program Counter

    const char *rom;          // Running rom

    // read sprites from I and save them here
    bool display[TFT_HEIGHT][TFT_WIDTH];

    std::stack<uint16_t> stack; // Subroutine stack
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

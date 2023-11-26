#pragma once
#include "Sandbox.h"

#define GBC_LOG(x) std::cout << "[GameBoy] " << x << std::endl;

#define TFT_WIDTH 160
#define TFT_HEIGHT 144

namespace GBC
{
  uint16_t Combine(uint8_t first, uint8_t second);
  uint16_t Get_Value_N(uint16_t opcode, uint8_t n);

  void Set_Bit_N(uint8_t *reg, uint8_t n, uint8_t val);
  void Set_Bit_N(uint16_t *reg, uint8_t n, uint8_t val);
  uint16_t Get_Bit_N(uint16_t src, uint8_t n);

};

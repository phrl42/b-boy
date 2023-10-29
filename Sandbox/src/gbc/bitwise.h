#pragma once
#include "Sandbox.h"

#define GBC_LOG(x) std::cout << "[GameBoy] " << x << std::endl;

#define TFT_WIDTH 160
#define TFT_HEIGHT 144

namespace GBC
{
  uint16_t Combine(uint8_t first, uint8_t second);
  uint16_t Get_Value_N(uint16_t opcode, uint8_t n);
};

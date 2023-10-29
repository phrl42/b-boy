#pragma once
#include "Sandbox.h"
#include "gbc/bitwise.h"

namespace GBC
{
  struct GPU
  {
    // 15-Bit RGB Color
    uint16_t display[TFT_HEIGHT][TFT_WIDTH];
  };
};

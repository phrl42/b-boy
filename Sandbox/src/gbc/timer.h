#pragma once
#include "gbc/bitwise.h"

#define A_DIV 0xFF04
#define A_TIMA 0xFF05
#define A_TMA 0xFF06
#define A_TAC 0xFF07

namespace GBC
{
  struct Timer
  {
    Timer();

    void Tick();

  private:
    
  };
};

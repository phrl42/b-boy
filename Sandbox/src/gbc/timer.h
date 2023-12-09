#pragma once
#include "gbc/bitwise.h"
#include "gbc/interrupt.h"

#define A_DIV 0xFF04
#define A_TIMA 0xFF05
#define A_TMA 0xFF06
#define A_TAC 0xFF07

namespace GBC
{
  struct Timer
  {
    Timer(Interrupt *interrupt);

    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);

    void Tick();

    //private:
    Interrupt *interrupt;

    uint16_t DIV = 0;
    uint8_t TIMA = 0;
    uint8_t TMA = 0;
    uint8_t TAC = 0;
  };
};

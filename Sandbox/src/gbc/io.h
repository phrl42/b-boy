#pragma once
#include "gbc/bitwise.h"

#define A_JOYPAD 0xFF00
#define A_SB 0xFF01
#define A_SC 0xFF02

namespace GBC
{
  struct IO
  {
    IO();

    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);
    
    void Serial_Update();
    void Serial_Print();

  private:
    uint8_t JOYPAD = 0;
    uint8_t SB = 0;
    uint8_t SC = 0;	

    int serial_size = 0;
  public:
    char serial[1024] = {0};
  };
};

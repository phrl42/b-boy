#pragma once
#include "gbc/bitwise.h"
#include "gbc/interrupt.h"

#define A_JOYPAD 0xFF00
#define A_SB 0xFF01
#define A_SC 0xFF02

namespace GBC
{
  struct IO
  {
    IO(Interrupt *interrupt);

    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);
    
    void Serial_Update();
    void Serial_Print();
 
    enum class KEY
      {
	A=0x2E, B=0x2D, SELECT=0x2B, START=0x27,
	RIGHT=0x1E, LEFT=0x1D, UP=0x1B, DOWN=0x17
      };

    void Press(KEY key);
    void Release();
  private:
    uint8_t JOYPAD = 0xFF;
    uint8_t SB = 0;
    uint8_t SC = 0;	

    Interrupt *interrupt;
    
    int serial_size = 0;
  public:
    char serial[1024] = {0};
  };
};

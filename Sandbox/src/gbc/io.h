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
	START = 0x17,
	SELECT = 0x1B,
	B = 0x1D,
	A = 0x1E,
	DOWN = 0x27,
	UP = 0x2B,
	LEFT = 0x2D,
	RIGHT = 0x2E,
	NONE = 0xFF
      };

    uint8_t press_dpad = 0;
    uint8_t press_button = 0;
    void Press(KEY key);
    void Release(KEY key);
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

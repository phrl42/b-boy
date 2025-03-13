#include "gbc/io.h"

namespace GBC
{
  IO::IO(Interrupt *interrupt)
  {
    this->interrupt = interrupt;
  }

  void IO::Press(KEY key)
  {
    if(((uint8_t)(key) & 0x20) == 0x20)
    {
      press_dpad &= (uint8_t)key;
    }
    else if(((uint8_t)(key) & 0x10) == 0x10)
    {
      press_button &= (uint8_t)key;
    }
    interrupt->Request(INTERRUPT::JOYPAD);
  }

  void IO::Release(KEY key)
  {
    uint8_t mod = (~(uint8_t)(key)) & 0x0F;
    if(((uint8_t)(key) & 0x20) == 0x20)
    {
      press_dpad |= (uint8_t)mod;
    }
    else if(((uint8_t)(key) & 0x10) == 0x10)
    {
      press_button |= (uint8_t)mod;
    }
  }

  uint8_t IO::Read(uint16_t address)
  {
    switch(address)
    {
    case A_JOYPAD:
      return JOYPAD;
      break;

    case A_SB:
      return SB;
      break;

    case A_SC:
      return SC;
      break;

    default:
      GBC_LOG("[IO]: READ FAILED");
      break;
    }
    return 0;
  }

  void IO::Write(uint16_t address, uint8_t value)
  {
    switch(address)
    {
    case A_JOYPAD:
    {
      JOYPAD &= 0x0F;
      JOYPAD |= value & 0xF0;

      uint8_t joy = JOYPAD;
      uint8_t check = joy & 0x30;

      JOYPAD = (uint8_t)KEY::NONE;
      if(value == 0x10)
      {
	JOYPAD = press_button;
      }
      if(value == 0x20)
      {
	JOYPAD = press_dpad;
      }
      break;
    } 

    case A_SB:
      SB = value;
      break;

    case A_SC:
      SC = value;
      break;

    default:
      GBC_LOG("[IO]: READ FAILED");
      break;
    }

  }

  void IO::Serial_Update()
  {
    if(SC == 0x81)
    {
      char c = SB;
      serial[serial_size++] = c;
      SC = 0;
    }
  }
  
  void IO::Serial_Print()
  {
    static int vsize = serial_size;
    if(vsize != serial_size)
    {
      printf("[GameBoy] [SERIAL]: %s\n", serial);
      vsize = serial_size;
    }
  }
};

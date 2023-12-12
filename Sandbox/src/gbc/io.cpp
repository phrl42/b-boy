#include "gbc/io.h"

namespace GBC
{
  IO::IO(Interrupt *interrupt)
  {
    this->interrupt = interrupt;
  }

  void IO::Press(KEY key)
  {
    JOYPAD = (uint8_t)key;
    interrupt->Request(INTERRUPT::JOYPAD);
  }

  void IO::Release()
  {
    //JOYPAD = 0x0F;
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
      JOYPAD &= 0x0F;
      JOYPAD |= value & 0xF0;
      break;

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

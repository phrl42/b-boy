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
    press = key;
    interrupt->Request(INTERRUPT::JOYPAD);
  }

  void IO::Release()
  {
    press = KEY::NONE;
    JOYPAD = 0xFF;
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
	if(press == KEY::START) JOYPAD = (uint8_t)KEY::START;
	if(press == KEY::SELECT) JOYPAD = (uint8_t)KEY::SELECT;
	if(press == KEY::A) JOYPAD = (uint8_t)KEY::A;
	if(press == KEY::B) JOYPAD = (uint8_t)KEY::B;
      }
      if(value == 0x20)
      {
	if(press == KEY::DOWN) JOYPAD = (uint8_t)KEY::DOWN;
	if(press == KEY::UP) JOYPAD = (uint8_t)KEY::UP;
	if(press == KEY::LEFT) JOYPAD = (uint8_t)KEY::LEFT;
	if(press == KEY::RIGHT) JOYPAD = (uint8_t)KEY::RIGHT;
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

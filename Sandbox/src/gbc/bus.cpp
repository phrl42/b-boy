#include "gbc/bus.h"

#include "Application.hpp"

namespace GBC
{
  void Bus::Emulate_Cycle(uint8_t n, bool normal)
  {
    uint8_t cpu_speed = NORMAL_SPEED;
    if(!normal)
    {
      cpu_speed = DOUBLE_SPEED;
    }
  }

  Bus::Bus(PPU *ppu, Timer *timer, Interrupt *interrupt)
  {
    this->ppu = ppu;
    this->timer = timer;
    this->interrupt = interrupt;
  }

  uint8_t Bus::Read(uint16_t address)
  {
    if(address <= 0x7FFF)
    {
      // ROM reading
    }
    else if(address <= 0x9FFF)
    {
      // VRAM reading
    }
    else if(address <= 0xDFFF)
    {
      // WRAM
    }
    else if(address <= 0xFDFF)
    {
      // echo ram
      GBC_LOG("ECHO RAM READ UNSUPPORTED");
      return 0;
    }
    else if(address <= 0xFE9F)
    {
      // OAM reading
      GBC_LOG("OAM READING NOT IMPLEMENTED");
    }
    else if(address <= 0xFEFF)
    {
      // no use
      GBC_LOG("ILLEGAL READ");
      return 0;
    }
    else if(address <= 0xFFFF)
    {
      // IO / HRAM / Interrupt / Timer
      if(address == 0xFF44)
      {
	return 0x90;
      }
    }

    return space[address];
  }
  
  uint8_t Bus::Write(uint16_t address, uint8_t value)
  {
    if(address <= 0x7FFF)
    {
      // ROM writing
    }
    else if(address <= 0x9FFF)
    {
      // VRAM writing
    }
    else if(address <= 0xDFFF)
    {
      // WRAM writing
    }
    else if(address <= 0xFDFF)
    {
      // echo ram
      GBC_LOG("ECHO RAM WRITE UNSUPPORTED");
      return 0;
    }
    else if(address <= 0xFE9F)
    {
      // OAM writing
      GBC_LOG("OAM WRITING NOT IMPLEMENTED");
    }
    else if(address <= 0xFEFF)
    {
      // no use
      GBC_LOG("ILLEGAL WRITE");
      return 0;
    }
    else if(address <= 0xFFFF)
    {
      // IO / HRAM / Interrupt / Timer

    }

    space[address] = value;
    return 0;
  }
  
};

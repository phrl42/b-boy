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

  Bus::Bus(PPU *ppu, Timer *timer, Interrupt *interrupt, IO *io)
  {
    this->ppu = ppu;
    this->timer = timer;
    this->interrupt = interrupt;
    this->io = io;
  }

  uint8_t Bus::Read(uint16_t address)
  {
    if(address <= 0x7FFF)
    {
      return space[address];
      // ROM reading
    }
    else if(address <= 0x9FFF)
    {
      // VRAM reading
      return ppu->Read(address);
    }
    else if(address <= 0xDFFF)
    {
      return space[address];
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
      if(address <= A_SC && address >= A_JOYPAD)
      {
	return io->Read(address);
      }

      if(address >= A_LY && address <= A_WX)
      {
	return ppu->Read(address);
      }

      if(address == A_IF || address == A_IE)
      {
	return interrupt->Read(address);
      }

      if(address >= A_DIV && address <= A_TAC)
      {
	return timer->Read(address);
      }

      return space[address];
      
    }

  }
  
  void Bus::Write(uint16_t address, uint8_t value)
  {
    if(address <= 0x7FFF)
    {
      space[address] = value;
      // ROM writing
    }
    else if(address <= 0x9FFF)
    {
      // VRAM writing
      ppu->Write(address, value);
    }
    else if(address <= 0xDFFF)
    {
      space[address] = value;
      // WRAM writing
    }
    else if(address <= 0xFDFF)
    {
      // echo ram
      GBC_LOG("ECHO RAM WRITE UNSUPPORTED");
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
    }
    else if(address <= 0xFFFF)
    {
      // IO / HRAM / Interrupt / Timer
      if(address <= A_SC && address >= A_JOYPAD)
      {
	io->Write(address, value);
      }

      if(address >= A_LY && address <= A_WX)
      {
	ppu->Write(address, value);
      }

      if(address == A_IF || address == A_IE)
      {
	interrupt->Write(address, value);
      }

      if(address >= A_DIV && address <= A_TAC)
      {
	timer->Write(address, value);
      }

      if(address >= 0xFF80 && address <= 0xFFFE)
      {
	space[address] = value;
      }
    }

  }
  
};

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

    for(uint8_t i = 0; i < n; i++)
    {
      timer->Tick();
      ppu->Tick();
    }
  }

  Bus::Bus(PPU *ppu, Timer *timer, Interrupt *interrupt, IO *io, ROM *rom)
  {
    this->ppu = ppu;
    this->timer = timer;
    this->interrupt = interrupt;
    this->io = io;
    this->rom = rom;
  }

  uint8_t Bus::Read(uint16_t address, bool emu)
  {
    uint8_t value = 0;
    if(address <= 0x7FFF)
    {
      // ROM reading
      value = rom->Read(address);
    }
    else if(address <= 0x9FFF)
    {
      // VRAM reading
      value = ppu->Read(address);
    }
    else if(address <= 0xDFFF)
    {
      value = space[address];
      // WRAM
    }
    else if(address <= 0xFDFF)
    {
      // echo ram
      GBC_LOG("ECHO RAM READ UNSUPPORTED");
    }
    else if(address <= 0xFE9F)
    {
      // OAM reading
      value = ppu->Read(address);
    }
    else if(address <= 0xFEFF)
    {
      // no use
      GBC_LOG("ILLEGAL READ");
    }
    else if(address <= 0xFFFF)
    {
      // IO / HRAM / Interrupt / Timer
      if(address <= A_SC && address >= A_JOYPAD)
      {
	value = io->Read(address);
      }
      else if(address >= A_LCDC && address <= A_WX)
      {
	value = ppu->Read(address);
      }
      else if(address == A_IF || address == A_IE)
      {
	value = interrupt->Read(address);
      }
      else if(address >= A_DIV && address <= A_TAC)
      {
	value = timer->Read(address);
      }
      else
      {
	value = space[address];
      }
    }

    if(emu)
    {
      Emulate_Cycle(4, true);
    }

    return value;
  }
  
  void Bus::Write(uint16_t address, uint8_t value, bool emu)
  {
    if(address <= 0x7FFF)
    {
      // ROM writing
      rom->Write(address, value);
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
      ppu->Write(address, value);
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

      if(address >= A_LCDC && address <= A_WX)
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

      if(address == A_DMA)
      {
	ppu->DMATransfer(space + (value * 0x0100));
      }
    }

    if(emu)
    {
      Emulate_Cycle(4, true);
    }
  }
  
};

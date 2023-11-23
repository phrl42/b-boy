#pragma once
#include "gbc/bus.h"
#include "gbc/cpu.h"
#include "gbc/bitwise.h"

#define IE 0xFFFF
#define IF 0xFF0F

namespace GBC
{
  enum INTERRUPT 
  {
    VBLANK, LCD, TIMER, SERIAL, JOYPAD
  };
  
  struct Interrupt
  {
    inline Interrupt(Bus *bus, CPU *cpu)
    :bus(bus), cpu(cpu)
    {

    }

    void Handle();
    
  private:
    Bus *bus;
    CPU *cpu;
    
    bool Check_Type(INTERRUPT isr, uint16_t address);
    void Handle_Type(INTERRUPT isr, uint16_t address);
  };
  
};

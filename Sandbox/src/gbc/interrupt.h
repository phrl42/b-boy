#pragma once
#include "gbc/bitwise.h"

#define A_IE 0xFFFF
#define A_IF 0xFF0F

namespace GBC
{
  struct CPU;
  
  enum INTERRUPT 
  {
    VBLANK, LCD, TIMER, SERIAL, JOYPAD
  };
  
  struct Interrupt
  {
    Interrupt();

    void Read(uint16_t address);
    uint8_t Write(uint16_t address, uint8_t value);

    void Handle(CPU *cpu);
  private:
    uint8_t IE = 0x00;
    uint8_t IF = 0x00;
    
    CPU *cpu;
    
    bool Check_Type(INTERRUPT isr, uint16_t address);
    void Handle_Type(INTERRUPT isr, uint16_t address);
  };
  
};

#include "gbc/interrupt.h"

namespace GBC
{
  static uint16_t Get_Bit_N(uint16_t src, uint8_t n)
  {
    if(n > 15)
    {
      GBC_LOG("Could not get bit. n > 15");
      return 0;
    }

    src <<= 15 - n;
    src >>= 15;

    return src;
  }

  void Interrupt::Handle_Type(INTERRUPT isr, uint16_t address)
  {
    cpu->PUSH(&cpu->PC, IMode::NONE, nullptr, IMode::NONE);

    cpu->PC = address;
  }

  bool Interrupt::Check_Type(INTERRUPT isr, uint16_t address)
  {
    if(Get_Bit_N(bus->Read(IF), isr) && Get_Bit_N(bus->Read(IE), isr))
    {
      Handle_Type(isr, address);

      bus->Write(address, 0x0000);
      cpu->IME = false;
      return true;
    }

    return false;
  }
 
  
  void Interrupt::Handle()
  {
    if(Check_Type(INTERRUPT::VBLANK, 0x40))
    {
    }
    else if(Check_Type(INTERRUPT::LCD, 0x48))
    {

    }
    else if(Check_Type(INTERRUPT::TIMER, 0x50))
    {

    }
    else if(Check_Type(INTERRUPT::SERIAL, 0x58))
    {

    }
    else if(Check_Type(INTERRUPT::JOYPAD, 0x60))
    {

    }
    else
    {
      GBC_LOG("Unkown Interrupt");
    }
  }
};
